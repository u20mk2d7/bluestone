#pragma once
#ifndef BLUESTONE_UTILS_BOOT_MANAGER_HPP
#define BLUESTONE_UTILS_BOOT_MANAGER_HPP

#include <cstdint>
#include <string>
#include <string_view>

namespace bluestone {

  struct ExchangeConfig {
    std::string exchange;  // e.g., "binance", "ctrader", "ibkr"
    std::string cfg_file;
    uint32_t instance_id = 0;

    std::string host;
    std::string port;
    std::string target;
    std::string symbol;
    std::string event_type;
    std::string public_key;

    std::string user_id;
    std::string password;
    std::string exchange_cfg;
  };

  class ConfigLoader {
   public:
    static ExchangeConfig load(int argc, char** argv);
    static constexpr std::string_view trim(std::string_view sv) noexcept;
    static std::string read_key_file(const std::string& filepath) noexcept;
  };

}  // namespace bluestone

#endif  // BLUESTONE_UTILS_BOOT_MANAGER_HPP
