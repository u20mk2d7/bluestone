#pragma once
#ifndef BLUESTONE_UTILS_CONFIG_HPP
#define BLUESTONE_UTILS_CONFIG_HPP

#include <string>

namespace Bluestone {

  struct ExchangeConfig {
    std::string exchange;
    std::string host;
    std::string port;
    std::string target;
    std::string symbol;
    std::string event_type;
    std::string api_key;
    std::string secret;
  };

  class ConfigParser {
   public:
    // Reads a JSON file from .env/ and populates the struct
    static ExchangeConfig parse_json(const std::string& filepath);

    static void cli_parse_text();
  };

}  // namespace Bluestone

#endif  // CONFIG_HPP
