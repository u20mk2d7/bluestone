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
    std::cout << "[BootManager] initialize ===";
    ExchangeConfig exchange_cfg;

    absl::ParseCommandLine(argc, argv);
    exchange_cfg.exchange = absl::GetFlag(FLAGS_exchange);
    exchange_cfg.cfg_file = absl::GetFlag(FLAGS_config);
    exchange_cfg.instance_id = absl::GetFlag(FLAGS_instance);

    // 1. This loads the file from the hard drive into a buffer in RAM.
    // It creates a "padded_string" (a special string with extra empty space at
    // the end to prevent the CPU from reading past the end of the data).
    simdjson::padded_string json_data;
    simdjson::error_code load_error =
        simdjson::padded_string::load(exchange_cfg.cfg_file).get(json_data);

    // 2. The parser links the json_data buffer to the document object (doc).
    // It doesn't convert the whole file at once. Instead, it prepares to scan
    // it.
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc;
    parser.iterate(json_data).get(doc);

    // auto extract_string = [...](...) -> std::string { ... };
    auto extract_string = [&doc](std::string_view key) -> std::string {
      std::string_view val;
      if (doc[key].get_string().get(val) == simdjson::SUCCESS) {
        return std::string(val);
      }
      return "";
    };

    exchange_cfg.host = extract_string("host");
    exchange_cfg.port = extract_string("port");
    exchange_cfg.target = extract_string("target");
    exchange_cfg.symbol = extract_string("symbol");
    exchange_cfg.event_type = extract_string("event_type");
    exchange_cfg.api_key = extract_string("api_key");
    exchange_cfg.private_key = extract_string("private_key");

    return exchange_cfg;
  }

}  // namespace bluestone
