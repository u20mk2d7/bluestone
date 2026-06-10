#include "config_loader.hpp"

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <simdjson.h>

#include <cstdlib>
#include <iostream>

// CLI Flag: ABSL_FLAG(type, name, default, help-text)
// ./app --exchange binance --config .env/binance.cfg --instance 1
// ./app --exchange ctrader --config .env/lmax.cfg --instance 2

ABSL_FLAG(std::string, exchange, "", "Exchange (e.g., ctrader, binance,...)");
ABSL_FLAG(std::string, config, "", "Path to the Json, cfg file");
ABSL_FLAG(uint32_t, instance, 0, "Bot instance ID for core pinning");

namespace bluestone {
  constexpr std::string_view bluestone::ConfigLoader::trim(
      std::string_view sv) noexcept {
    //
    size_t start = sv.find_first_not_of(" \t\r\n");
    size_t end = sv.find_last_not_of(" \t\r\n");
    return (start == std::string_view::npos)
               ? ""
               : sv.substr(start, end - start + 1);
  }

  bluestone::ExchangeConfig bluestone::ConfigLoader::load(int argc,
                                                          char** argv) {
    std::cout << "[ConfigLoader] initialize ===\n";
    ExchangeConfig exchange_cfg;

    absl::ParseCommandLine(argc, argv);
    exchange_cfg.exchange = absl::GetFlag(FLAGS_exchange);
    exchange_cfg.cfg_file = absl::GetFlag(FLAGS_config);
    exchange_cfg.instance_id = absl::GetFlag(FLAGS_instance);

    if (exchange_cfg.exchange.empty()) {
      std::cout << "[ConfigLoader]: CLI Exchange does not have val\n";
    }

    if (exchange_cfg.cfg_file.empty()) {
      std::cout << "[ConfigLoader]: CLI config file does not have val\n";
    }

    if (exchange_cfg.instance_id == 0) {
      std::cout << "[ConfigLoader]: CLI instance id cpu does not have val\n";
    }

    std::ifstream file(exchange_cfg.cfg_file);
    if (!file.is_open()) {
      std::cout << "[ERROR]: Can not open file: " << exchange_cfg.cfg_file
                << '\n';
      // Terminates quickly without causing race conditions in destructors
      std::quick_exit(EXIT_FAILURE);
    }

    std::string line;
    while (std::getline(file, line)) {
      // skip empty lines or full comment lines
      if (line.empty() || line[0] == '#' || line[0] == ';') continue;
      auto delimiter_pos = line.find('=');
      std::string_view key = trim(std::string_view(line).substr(
          0, delimiter_pos));  // start from 0 index to pos = minus 1
                               //
      std::string_view value =
          trim(std::string_view(line).substr(delimiter_pos + 1));
      // Map the parsed keys directly into the C++ struct
      if (key == "host") {
        exchange_cfg.host = std::string(value);
      } else if (key == "port") {
        exchange_cfg.port = std::string(value);
      } else if (key == "target") {
        exchange_cfg.target = std::string(value);
      } else if (key == "symbol") {
        exchange_cfg.symbol = std::string(value);
      } else if (key == "event_type") {
        exchange_cfg.event_type = std::string(value);
      } else if (key == "api_key") {
        exchange_cfg.api_key = std::string(value);
      } else if (key == "private_key") {
        exchange_cfg.private_key = std::string(value);
      } else if (key == "password") {
        exchange_cfg.password = std::string(value);
      }
    }

    file.close();

    return exchange_cfg;
  }

  std::string ConfigLoader::read_key_file(
      const std::string& filepath) noexcept {
    std::ifstream file(filepath);
    if (!file.is_open()) {
      std::cout << "[ERROR]: Can not open file: " << filepath << '\n';
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }

}  // namespace bluestone
