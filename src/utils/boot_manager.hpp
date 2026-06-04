#pragma once
#ifndef BLUESTONE_UTILS_BOOT_MANAGER_HPP
#define BLUESTONE_UTILS_BOOT_MANAGER_HPP

#include <string>
#include <string_view>
#include <cstdint>

namespace Bluestone {
namespace Utils {

struct ExchangeConfig {
    std::string mode;
    std::string host;
    std::string port;
    std::string target;
    std::string symbol;
    std::string event_type;
    std::string api_key;
    std::string private_key;
    uint32_t instance_id = 0;
};

class BootManager {
public:
    // Parses the CLI arguments, finds the JSON file, parses it, and returns the strict config object.
    static ExchangeConfig initialize(int argc, char** argv);
};

} // namespace Utils
} // namespace Bluestone

#endif // BLUESTONE_UTILS_BOOT_MANAGER_HPP
