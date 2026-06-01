#pragma once
#ifndef BLUESTONE_UTILS_CFG_UTIL_HPP
#define BLUESTONE_UTILS_CFG_UTIL_HPP
#include <string>
#include <string_view>
/*

{
    "host": "stream.binance.com",
    "port": "9443",
    "target": "/ws",
    "symbol": "btcusdt",
    "event_type": "trade",
    "api_key": "~/.ssh/binance/api.key",
    "private_key": "~/.ssh/binance/private_key.pem"
}
  */
namespace bluestone {

struct ExchangeConfig {
  std::string host;
  std::string port;
  std::string target;
  std::string symbol;
  std::string event_type;
  std::string api_key;
  std::string private_key;
};

class ConfigParser {
 public:
  // Reads a JSON file from .env/ and populates the struct
  static ExchangeConfig parse_json(std::string_view path_to_file);
};

}  // namespace bluestone

#endif  // BLUESTONE_UTILS_CFG_UTIL_HPP
