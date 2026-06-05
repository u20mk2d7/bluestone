#include "utils/boot_manager.hpp"

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <simdjson.h>

#include <cstdlib>
#include <iostream>

// CLI Flag: ABSL_FLAG(type, name, default, help-text)
// ./app --exchange binance --config .env/binance.json --instance 1
// ./app --exchange ctrader --config .env/initiator.cfg --instance 2

ABSL_FLAG(std::string, exchange, "", "Exchange (e.g., ctrader, binance,...)");
ABSL_FLAG(std::string, config, "", "Path to the Json, cfg file");
ABSL_FLAG(uint32_t, instance, 0, "Bot instance ID for core pinning");

namespace bluestone {
  bluestone::ExchangeConfig bluestone::BootManager::initialize(int argc,
                                                               char** argv) {
    std::cout << "[BootManager] initialize ===\n";
    ExchangeConfig exchange_cfg;

    absl::ParseCommandLine(argc, argv);
    exchange_cfg.exchange = absl::GetFlag(FLAGS_exchange);
    exchange_cfg.cfg_file = absl::GetFlag(FLAGS_config);
    exchange_cfg.instance_id = absl::GetFlag(FLAGS_instance);

    if (exchange_cfg.exchange.empty()) {
      std::cout << "[BootManager]: CLI Exchange does not have val\n";
      std::exit(EXIT_FAILURE);
    } else if (exchange_cfg.cfg_file.empty()) {
      std::cout << "[BootManager]: CLI config file does not have val\n";
      std::exit(EXIT_FAILURE);
    } else if (exchange_cfg.instance_id == 0) {
      std::cout << "[BootManager]: CLI instance id cpu does not have val\n";
      std::exit(EXIT_FAILURE);
    } else {
    }

    // 1. This loads the file from the hard drive into a buffer in RAM.
    // It creates a "padded_string" (a special string with extra empty space at
    // the end to prevent the CPU from reading past the end of the data).
    simdjson::padded_string json_data;

    simdjson::error_code load_error =
        simdjson::padded_string::load(exchange_cfg.cfg_file).get(json_data);
    if (load_error) {
      std::cerr << "[BootManager] FATAL: Cannot open config file: "
                << simdjson::error_message(load_error) << '\n';
    }

    // 2. The parser links the json_data buffer to the document object (doc).
    // It doesn't convert the whole file at once. Instead, it prepares to scan
    // it.
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc = parser.iterate(json_data);
    // Loop through the file exactly once in a forward direction (No Segfaults)
    for (auto field : doc.get_object()) {
      std::string_view key = field.unescaped_key();
      std::string_view val;

      if (field.value().get_string().get(val) == simdjson::SUCCESS) {
        if (key == "host") {
          exchange_cfg.host = std::string(val);
        } else if (key == "port") {
          exchange_cfg.port = std::string(val);
        } else if (key == "target") {
          exchange_cfg.target = std::string(val);
        } else if (key == "symbol") {
          exchange_cfg.symbol = std::string(val);
        } else if (key == "event_type") {
          exchange_cfg.event_type = std::string(val);
        } else if (key == "api_key") {
          exchange_cfg.api_key = std::string(val);
        } else if (key == "private_key") {
          exchange_cfg.private_key = std::string(val);
        }
      }
    }

    return exchange_cfg;
  }

}  // namespace bluestone
