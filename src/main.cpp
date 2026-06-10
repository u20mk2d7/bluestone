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
  std::unique_ptr<bluestone::IExchangeConnector> gateway =
      std::make_unique<bluestone::LMAXConnector>(".env/lmax.cfg");

  // 2. Ignite connection
  gateway->connect();

  std::cout << "[Main] Initiator started. Engine is live.\n";
  std::cout << "[Main] Press Ctrl+C to stop.\n\n";

  return 0;
}