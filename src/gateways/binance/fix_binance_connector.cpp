#include "fix_binance_connector.hpp"

#include <quickfix/FileStore.h>
#include <quickfix/Log.h>
#include <quickfix/SSLSocketInitiator.h>
#include <quickfix/SessionSettings.h>
#include <quickfix/fix44/Logon.h>

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "binance_fix_application.hpp"

namespace Bluestone {

  FixBinanceConnector::FixBinanceConnector(ExchangeConfig* cfg,
                                           TradeQueue* queue)
      : cfg_(cfg), queue_(queue) {
    std::cout << "[FixBinance] Connector initialized for " << cfg->exchange
              << "\n";
  }

  FixBinanceConnector::~FixBinanceConnector() { disconnect(); }

  std::string read_credential_file(const std::string& path_or_key,
                                   bool is_pem) {
    // If it is already a raw PEM key (backwards compatibility check)
    if (path_or_key.find("-----BEGIN") != std::string::npos) {
      return path_or_key;
    }

    std::string actual_path = path_or_key;

    // Expand '~' to the user's home directory (e.g., /Users/localhost)
    if (!path_or_key.empty() && path_or_key[0] == '~') {
      const char* home = std::getenv("HOME");
      if (home) {
        actual_path = std::string(home) + path_or_key.substr(1);
      }
    }

    std::ifstream file(actual_path);
    if (!file.is_open()) {
      // If we can't open the file, fallback to treating it as a raw string
      // (useful if you pass a raw API key directly in the JSON later)
      return path_or_key;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    // If it is an API Key (not a PEM file), trim any accidental trailing
    // newlines or spaces. Binance will reject the API key if it has a hidden
    // '\n' at the end.
    if (!is_pem) {
      content.erase(content.find_last_not_of(" \n\r\t") + 1);
    }

    return content;
  }
  // -----------------------------------------------------
  void FixBinanceConnector::connect() {
    std::cout << "[FixBinance] Starting FIX connection to Binance...\n";

    try {
      // 1. Read credentials from the file paths provided in fix_binance.json
      std::string actual_api_key = read_credential_file(cfg_->api_key, false);
      std::string actual_secret = read_credential_file(cfg_->secret, true);

      // 2. Initialize application with the raw file contents
      application_ = std::make_unique<BinanceFixApplication>(actual_api_key,
                                                             actual_secret);
      application_ = std::make_unique<BinanceFixApplication>(actual_api_key,
                                                             actual_secret);
      std::string fix_config_path = ".env/binance_testnet.cfg";
      std::ifstream settings_file(fix_config_path);

      if (!settings_file.is_open()) {
        std::cerr << "[FixBinance] ERROR: Cannot open FIX config file: "
                  << fix_config_path << "\n";
        return;
      }

      FIX::SessionSettings settings(settings_file);
      std::cout << "[FixBinance] Loaded FIX settings from " << fix_config_path
                << "\n";

      // 1. Allocate the factories to the class members so they stay alive
      store_factory_ = std::make_unique<FIX::FileStoreFactory>(settings);
      log_factory_ = std::make_unique<FIX::ScreenLogFactory>(settings);

      // 2. Dereference the pointers to pass lvalue references to the
      // constructor
      initiator_ = std::make_unique<FIX::SSLSocketInitiator>(
          *application_, *store_factory_, settings, *log_factory_);

      initiator_->start();
      std::cout
          << "[FixBinance] SSLSocketInitiator started successfully. Waiting "
             "for Logon...\n";

    } catch (const FIX::ConfigError& e) {
      std::cerr << "[FixBinance] QuickFIX Configuration Error: " << e.what()
                << '\n';
    } catch (const std::exception& e) {
      std::cerr << "[FixBinance] Failed to start initiator: " << e.what()
                << '\n';
    }
  }

  void FixBinanceConnector::reconnect() {
    std::cout << "[FixBinance] Reconnecting...\n";
    disconnect();
    connect();
  }

  void FixBinanceConnector::disconnect() {
    if (initiator_) {
      initiator_->stop();
      initiator_.reset();
    }
    // Also clean up factories to ensure proper shutdown sequence
    log_factory_.reset();
    store_factory_.reset();
    application_.reset();

    std::cout << "[FixBinance] Disconnected from FIX session.\n";
  }

  void FixBinanceConnector::subscribe_market_data(int req_id,
                                                  const std::string& symbol) {
    std::cout << "[FixBinance] Market data subscription not supported via FIX. "
                 "Use WebSocket.\n";
  }

  void FixBinanceConnector::sendNewOrderSingle(const std::string& symbol,
                                               double qty, double price,
                                               char side) {
    std::cout << "[FixBinance] NewOrderSingle -> " << symbol
              << " | Side: " << side << " | Qty: " << qty
              << " | Price: " << price << "\n";
  }

}  // namespace Bluestone
