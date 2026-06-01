#pragma once
#ifndef DATAGATEWAYENGINE_HPP
#define DATAGATEWAYENGINE_HPP

#include <atomic>
#include <memory>

#include "exchange/i_exchange_connector.hpp"
#include "trade_queue.hpp"

namespace bluestone {
  class DataGatewayEngine {
   private:
    std::unique_ptr<bluestone::IExchangeConnector> exchange_;
    std::shared_ptr<bluestone::TradeQueue> queue_;
    std::atomic<bool> running_;

   public:
    // Inject both the exchange and the queue
    explicit DataGatewayEngine(
        std::unique_ptr<bluestone::IExchangeConnector> exchange,
        std::shared_ptr<bluestone::TradeQueue> queue);
    void run();
    void stop();
  };

}  // namespace bluestone
#endif  // DATAGATEWAYENGINE_HPP
