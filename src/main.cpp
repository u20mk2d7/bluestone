#include <iostream>

#include "utils/boot_manager.hpp"

int main(int argc, char** argv) {
  bluestone::BootManager::initialize(argc, argv);
  // 1. Initialize and capture the returned config object
  bluestone::ExchangeConfig cfg =
      bluestone::BootManager::initialize(argc, argv);

  // 2. Print the parsed values to verify the Abseil and simdjson logic
  std::cout << "\n=== [TEST] Parsed Configuration ===\n";
  std::cout << "Exchange    : " << cfg.exchange << "\n";
  std::cout << "Config File : " << cfg.cfg_file << "\n";
  std::cout << "Instance ID : " << cfg.instance_id << "\n";
  std::cout << "Host        : " << cfg.host << "\n";
  std::cout << "Port        : " << cfg.port << "\n";
  std::cout << "Target      : " << cfg.target << "\n";
  std::cout << "Symbol      : " << cfg.symbol << "\n";
  std::cout << "Event Type  : " << cfg.event_type << "\n";
  std::cout << "API Key     : " << cfg.api_key << "\n";
  std::cout << "Private Key : " << cfg.private_key << "\n";
  std::cout << "===================================\n\n";
  return 0;
}
