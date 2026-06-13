#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "core/exchange/i_exchange_connector.hpp"
#include "gateways/lmax/lmax_connector.hpp"
#include "utils/config_loader.hpp"

int main(int argc, char** argv) {
  bluestone::ExchangeConfig cfg = bluestone::ConfigLoader::load(argc, argv);
  std::cout << "=================================================\n";
  std::cout << " Trading is Running with Exchange: " << cfg.exchange << "\n";
  std::cout << "=================================================\n";

  return 0;
}
