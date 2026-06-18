#pragma once
#ifndef BINANCE_CONNECTOR_HPP
#define BINANCE_CONNECTOR_HPP

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <memory>
#include <string>

#include "core/ipc/trade_queue.hpp"
#include "gateway/interface/i_exchange_connector.hpp"
#include "utils/config_loader.hpp"

namespace bluestone {

  class BinanceConnector : public bluestone::IExchangeConnector {
   private:
    bluestone::ExchangeConfig* cfg_;  // Non-owning pointer to config
    bluestone::TradeQueue* queue_;    // Lock-free queue (non-owning)
    boost::asio::io_context* ioc_;    // Worker thread's io_context

    void read_next_message();

    // WebSocket connection (managed inside async context)
    std::unique_ptr<boost::beast::websocket::stream<
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>>
        ws_;

    bool is_connected_ = false;

    void start_websocket_session();
    void on_read(boost::beast::flat_buffer& buffer);
    void process_market_data(const std::string& json_data);

   public:
    explicit BinanceConnector(bluestone::ExchangeConfig* cfg,
                              bluestone::TradeQueue* queue,
                              boost::asio::io_context* ioc);

    ~BinanceConnector() override;
    BinanceConnector(BinanceConnector& other);
    BinanceConnector& operator=(const BinanceConnector& other);
    BinanceConnector(BinanceConnector&& other) noexcept;
    BinanceConnector& operator=(BinanceConnector&& other) noexcept;

    // === IExchangeConnector interface ===
    void connect() override;
    void reconnect() override;
    void disconnect() override;
    void subscribe_market_data(int req_id, const std::string& symbol) override;
  };

}  // namespace bluestone

#endif  // BINANCE_CONNECTOR_HPP
