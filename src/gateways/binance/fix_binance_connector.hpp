#pragma once
#ifndef FIX_BINANCE_CONNECTOR_HPP
#define FIX_BINANCE_CONNECTOR_HPP

#include <quickfix/Application.h>
#include <quickfix/Log.h>           // Added
#include <quickfix/MessageStore.h>  // Added
#include <quickfix/SSLSocketInitiator.h>

#include <memory>
#include <string>

#include "core/exchange/i_exchange_connector.hpp"
#include "core/trade_queue.hpp"
#include "utils/config.hpp"

namespace Bluestone {
  class BinanceFixApplication;
}

namespace Bluestone {

  class FixBinanceConnector : public IExchangeConnector {
   private:
    ExchangeConfig* cfg_;
    TradeQueue* queue_;

    std::unique_ptr<FIX::SSLSocketInitiator> initiator_;
    std::unique_ptr<BinanceFixApplication> application_;

    // Added: Memory ownership for the QuickFIX factories
    std::unique_ptr<FIX::MessageStoreFactory> store_factory_;
    std::unique_ptr<FIX::LogFactory> log_factory_;

   public:
    explicit FixBinanceConnector(ExchangeConfig* cfg, TradeQueue* queue);

    ~FixBinanceConnector() override;

    void connect() override;
    void reconnect() override;
    void disconnect() override;
    void subscribe_market_data(int req_id, const std::string& symbol) override;

    void sendNewOrderSingle(const std::string& symbol, double qty, double price,
                            char side);
  };

}  // namespace Bluestone

#endif
