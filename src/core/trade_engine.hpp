#pragma once
#ifndef DATAGATEWAYENGINE_HPP
#define DATAGATEWAYENGINE_HPP

#include <atomic>
#include <memory>

#include "exchange/i_exchange_connector.hpp"
#include "trade_queue.hpp"

namespace Bluestone {
  class DataGatewayEngine {
   private:
    std::unique_ptr<Bluestone::IExchangeConnector> exchange_;
    std::shared_ptr<Bluestone::TradeQueue> queue_;
    std::atomic<bool> running_;

   public:
    // Inject both the exchange and the queue
    explicit DataGatewayEngine(
        std::unique_ptr<Bluestone::IExchangeConnector> exchange,
        std::shared_ptr<Bluestone::TradeQueue> queue);
    void run();
    void stop();
  };

}  // namespace Bluestone
#endif  // DATAGATEWAYENGINE_HPP
