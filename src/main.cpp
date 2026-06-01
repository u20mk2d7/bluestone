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
#include "utils/config.hpp"
#include "utils/cli.hpp"
#include "utils/event_loop_worker.hpp"

int main(int argc, char** argv) {  //
                                   //
Bluestone::CLI cli;
    cli.parse(argc, argv);

    std::string config_path = cli.get_config_path();
    std::string mode = cli.get_mode();
 
  std::cout << "config_path: " << config_path << "\nmode: " << mode << '\n';
  return 0;
}
