#pragma once
#ifndef BLUESTONE_UTILS_BOOT_MANAGER_HPP
#define BLUESTONE_UTILS_BOOT_MANAGER_HPP

#include <cstdint>
#include <string>
#include <string_view>

namespace bluestone {

  struct ExchangeConfig {
    std::string exchange;  // e.g., "binance", "ctrader", "ibkr"
    std::string host;
    std::string port;
    std::string target;
    std::string symbol;
    std::string event_type;
    std::string api_key;
    std::string private_key;
    std::string cfg_file;  // .env/abc.json
    uint32_t instance_id = 0;
  };

  class BootManager {
   public:
    static ExchangeConfig initialize(int argc, char** argv);
  };

}  // namespace bluestone

#endif  // BLUESTONE_UTILS_BOOT_MANAGER_HPP
