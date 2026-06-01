#pragma once
#ifndef CLI_HPP
#define CLI_HPP

#include <string>
namespace Bluestone {
  class CLI {
   public:
    CLI() = default;
    ~CLI() = default;
    CLI(const CLI& other) = default;
    CLI(CLI&& other) = default;
    CLI& operator=(const CLI& other) = default;
    CLI& operator=(const CLI&& other) = delete;

    void parser(int argc, char** argv);
    // Retrieve the parsed values
    [[nodiscard]] std::string get_mode() const;
    [[nodiscard]] std::string get_config_path() const;
  };
}  // namespace Bluestone
#endif  // BLUESTONE_UTILS_CONFIG_HPP
