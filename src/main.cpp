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
  bluestone::ExchangeConfig exchange_cfg =
      bluestone::ConfigLoader::load(argc, argv);

  std::unique_ptr<bluestone::IExchangeConnector> gateway =
      std::make_unique<bluestone::LMAXConnector>(exchange_cfg.cfg_file);

  gateway->connect();
  return 0;
}
