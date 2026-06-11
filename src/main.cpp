#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "core/exchange/i_exchange_connector.hpp"
#include "gateways/lmax/lmax_connector.hpp"
#include "utils/config_loader.hpp"

int main(int argc, char** argv) {
  std::cout << "=================================================\n";
  std::cout << " Trading is Running ...\n";
  std::cout << "=================================================\n";
  bluestone::ExchangeConfig cfg = bluestone::ConfigLoader::load(argc, argv);

  std::unique_ptr<bluestone::IExchangeConnector> gateway =
      std::make_unique<bluestone::LMAXConnector>(cfg);

  gateway->connect();
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}
