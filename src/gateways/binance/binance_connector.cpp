#include "binance_connector.hpp"

#include <openssl/err.h>
#include <simdjson.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>

namespace bluestone {

  BinanceConnector::BinanceConnector(bluestone::ExchangeConfig* cfg,
                                     bluestone::TradeQueue* queue,
                                     boost::asio::io_context* ioc)
      : cfg_(cfg), queue_(queue), ioc_(ioc) {
    std::cout << "[BinanceConnector] Initialized for " << cfg->symbol << "\n";
  }

  BinanceConnector::~BinanceConnector() { disconnect(); }

  void BinanceConnector::connect() {
    std::cout << "[Binance] Delegating connection to worker thread...\n";

    boost::asio::post(*ioc_, [this]() {
      std::cout << "[Binance] Starting WebSocket session...\n";
      start_websocket_session();
    });
  }

  void BinanceConnector::start_websocket_session() {
    boost::system::error_code ec;

    // Create SSL context
    boost::asio::ssl::context ctx{boost::asio::ssl::context::tls_client};
    ctx.set_verify_mode(boost::asio::ssl::verify_none, ec);

    // Create WebSocket stream
    ws_ = std::make_unique<boost::beast::websocket::stream<
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>>(*ioc_, ctx);

    // DNS resolution (FORCE IPv4 to prevent macOS Error 49)
    boost::asio::ip::tcp::resolver resolver{*ioc_};
    auto endpoints = resolver.resolve(boost::asio::ip::tcp::v4(), cfg_->host,
                                      cfg_->port, ec);
    if (ec) {
      std::cerr << "[Binance] DNS Resolution failed: " << ec.message() << '\n';
      return;
    }

    // TCP Connect
    boost::asio::connect(boost::beast::get_lowest_layer(*ws_), endpoints, ec);
    if (ec) {
      std::cerr << "[Binance] TCP Connect failed: " << ec.message() << '\n';
      return;
    }

    // SNI (Server Name Indication) - Required for Binance/Cloudflare
    if (!SSL_set_tlsext_host_name(ws_->next_layer().native_handle(),
                                  cfg_->host.c_str())) {
      std::cerr << "[Binance] SNI Setup failed.\n";
      return;
    }

    // SSL Handshake
    ws_->next_layer().handshake(boost::asio::ssl::stream_base::client, ec);
    if (ec) {
      std::cerr << "[Binance] SSL Handshake failed: " << ec.message() << '\n';
      return;
    }

    // WebSocket Handshake
    std::string target = cfg_->target + cfg_->symbol + '@' + cfg_->event_type;
    ws_->handshake(cfg_->host, target, ec);
    if (ec) {
      std::cerr << "[Binance] WebSocket Handshake failed: " << ec.message()
                << '\n';
      return;
    }

    is_connected_ = true;
    std::cout << "[Binance] Successfully connected to " << cfg_->symbol
              << " stream\n";

    // Start async read loop
    read_next_message();
  }

  void BinanceConnector::read_next_message() {
    if (!is_connected_ || !ws_) return;

    auto buffer = std::make_shared<boost::beast::flat_buffer>();

    ws_->async_read(*buffer, [this, buffer](boost::beast::error_code ec,
                                            std::size_t bytes_transferred) {
      if (ec) {
        std::cerr << "[Binance] Async read failed: " << ec.message() << '\n';
        is_connected_ = false;
        return;
      }

      // Convert the flat_buffer to a std::string
      std::string json_data = boost::beast::buffers_to_string(buffer->data());

      // Send it to your parser
      process_market_data(json_data);

      // Queue the next read immediately
      read_next_message();
    });
  }

  void BinanceConnector::on_read(boost::beast::flat_buffer& buffer) {
    std::string raw_data = boost::beast::buffers_to_string(buffer.data());
    buffer.consume(buffer.size());

    process_market_data(raw_data);

    // Continue reading
    read_next_message();
  }

  void BinanceConnector::reconnect() {
    std::cout << "[Binance] Reconnecting...\n";
    disconnect();
    connect();
  }

  void BinanceConnector::disconnect() {
    if (is_connected_ && ws_) {
      boost::beast::error_code ec;
      ws_->close(boost::beast::websocket::close_code::normal, ec);
      is_connected_ = false;
      std::cout << "[Binance] Disconnected\n";
    }
  }

  void BinanceConnector::subscribe_market_data(int req_id,
                                               const std::string& symbol) {
    std::cout << "[Binance] Subscribe requested: " << symbol
              << " (req_id: " << req_id << ")\n";
  }
  void BinanceConnector::process_market_data(const std::string& json_data) {
    // We use a thread_local parser for HFT performance
    thread_local simdjson::ondemand::parser parser;

    try {
      simdjson::padded_string padded_json(json_data);
      auto doc = parser.iterate(padded_json);

      // Check if it's an event payload
      if (doc.find_field("e").error() != simdjson::SUCCESS) {
        return;
      }

      std::string_view event_type = doc["e"].get_string().value_unsafe();

      // Ensure we are processing a depth update
      if (event_type != "depthUpdate") {
        std::cout << "[Binance] Ignoring non-depth event: " << event_type
                  << "\n";
        return;
      }

      std::string_view symbol_str = doc["s"].get_string().value_unsafe();

      // Process Bids ("b")
      auto bids = doc["b"].get_array();
      if (bids.error() == simdjson::SUCCESS) {
        for (auto bid : bids) {
          auto bid_arr = bid.get_array();
          auto it = bid_arr.begin();

          // First element is price, second is quantity
          std::string_view price_str = (*it).get_string().value_unsafe();
          ++it;
          std::string_view qty_str = (*it).get_string().value_unsafe();

          double price = std::stod(std::string(price_str));
          double qty = std::stod(std::string(qty_str));

          // In a real HFT system, a quantity of 0 means "remove this price
          // level"
          if (qty > 0) {
            std::cout << "[BID] " << symbol_str << " | Price: " << price
                      << " | Qty: " << qty << '\n';
          }
        }
      }

      // Process Asks ("a")
      auto asks = doc["a"].get_array();
      if (asks.error() == simdjson::SUCCESS) {
        for (auto ask : asks) {
          auto ask_arr = ask.get_array();
          auto it = ask_arr.begin();

          std::string_view price_str = (*it).get_string().value_unsafe();
          ++it;
          std::string_view qty_str = (*it).get_string().value_unsafe();

          double price = std::stod(std::string(price_str));
          double qty = std::stod(std::string(qty_str));

          if (qty > 0) {
            std::cout << "[ASK] " << symbol_str << " | Price: " << price
                      << " | Qty: " << qty << '\n';
          }
        }
      }

    } catch (const simdjson::simdjson_error& e) {
      // Ignore structural JSON errors (e.g., partial websocket reads)
    } catch (const std::exception& e) {
      std::cerr << "[Binance] Parse exception: " << e.what() << '\n';
    }
  }
}  // namespace bluestone
