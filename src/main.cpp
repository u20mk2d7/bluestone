#include <absl/flags/flag.h>
#include <absl/flags/parse.h>

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "core/exchange/i_exchange_connector.hpp"
#include "core/trade_engine.hpp"
#include "gateways/binance/binance_connector.hpp"
#include "gateways/binance/fix_binance_connector.hpp"
#include "utils/utils_cli.hpp"
#include "utils/utils_cfg.hpp"
#include "utils/event_loop_worker.hpp"

int main(int argc, char** argv) {  //
  //
  // read cli
  bluestone::CLI cli;
  cli.parse(argc, argv);

  std::string file_cfg = cli.get_config_path();
  std::string mode = cli.get_mode();

  std::cout << "--mode " << mode << " --config_path " << file_cfg << '\n';

  // parse json file
bluestone::ExchangeConfig exchange_cfg = bluestone::ConfigParser::parse_json(file_cfg);
// If the host is empty, it means the JSON parser failed or the file was missing
  if (exchange_cfg.host.empty()) {
      std::cerr << "[Main] Critical Error: Failed to load configuration. Exiting.\n";
      return 1;
  } else {
  std::cout << "[Config] Loaded successfully: "
            << (exchange_cfg.host.empty() ? "Unknown" : exchange_cfg.host)
            << " | Port: "
            << (exchange_cfg.port.empty() ? "Unknown" : exchange_cfg.port)
            << "\n";
  }
  

  return 0;
}
