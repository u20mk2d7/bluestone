#include <string>

#include "core/exchange/i_exchange_connector.hpp"

namespace bluestone {
class IBKRConnector final : public IExchangeConnector {
 private:
  ExchangeConfig cfg_;
  TradeQueue* queue_;
  std::string tws_ip_;
  int client_id_;
  RiskEngine* risk_engine_;

 public:
  // The Constructor takes whatever it needs to survive
  IBKRConnector(ExchangeConfig cfg, TradeQueue* queue, std::string tws_ip,
                int client_id, RiskEngine* risk_engine)
      : cfg_(cfg),
        queue_(queue),
        tws_ip_(tws_ip),
        client_id_(client_id),
        risk_engine_(risk_engine) {
    // Setup IBKR specific stuff here
  }

  // Fulfilling the interface contract
  void connect() override {
    // IBKR specific connection logic
  }

  void disconnect() override {
    // IBKR specific disconnection logic
  }
};
}  // namespace bluestone