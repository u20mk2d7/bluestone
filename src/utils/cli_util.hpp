#pragma once
#ifndef BLUESTONE_UTILS_CLI_HPP
#define BLUESTONE_UTILS_CLI_HPP

#include <string>
namespace bluestone {
class CLI {
 public:
  CLI() = default;
  ~CLI() = default;
  CLI(const CLI& other) = default;
  CLI(CLI&& other) = default;
  CLI& operator=(const CLI& other) = default;
  CLI& operator=(CLI&& other) = default;

  void parse(int argc, char** argv);
  // Retrieve the parsed values
  [[nodiscard]] std::string get_mode() const;
  [[nodiscard]] std::string get_config_path() const;
};
}  // namespace bluestone
#endif  // BLUESTONE_UTILS_CLI_HPP
