#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "core/exchange/i_exchange_connector.hpp"
#include "gateways/lmax/lmax_connector.hpp"
#include "utils/config_loader.hpp"

int main(int argc, char** argv) {
  std::cout << "=================================================\n";
  std::cout << " Project Bluestone HFT Engine\n";
  std::cout << "=================================================\n";

  // 1. Initialize Gateway via polymorphic interface
  std::unique_ptr<bluestone::IExchangeConnector> gateway_lmax =
      std::make_unique<bluestone::LMAXConnector>(".env/lmax.cfg");

  // 2. Ignite connection
  gateway_lmax->connect();

  std::cout << "[Main] Initiator started. Engine is live.\n";
  std::cout << "[Main] Press Ctrl+C to stop.\n\n";

  // 1. Setup all the heavy dependencies
  ExchangeConfig cfg = load_config();
  auto queue = std::make_unique<TradeQueue>();
  auto risk = std::make_unique<RiskEngine>();

  // 2. The Polymorphic Pointer
  std::unique_ptr<bluestone::IExchangeConnector> gateway;

  // 3. Feed the IBKR constructor its "many, many arguments"
  gateway = std::make_unique<bluestone::IBKRConnector>(
      cfg, queue.get(), "127.0.0.1", 999, risk.get());

  // 4. The rest of your program only sees the simple interface!
  gateway->connect();

  return 0;
}