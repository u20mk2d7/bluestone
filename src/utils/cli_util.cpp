#include "utils/cli_util.hpp"

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>

#include <string>

// 1. Define the Abseil flags
ABSL_FLAG(std::string, mode, "", "Execution mode (e.g., fix, run, test)");
ABSL_FLAG(std::string, config, "", "Path to the JSON config file");
namespace bluestone {

void CLI::parse(int argc, char** argv) {
  // Pass raw terminal inputs to Abseil
  absl::ParseCommandLine(argc, argv);
}

std::string CLI::get_mode() const { return absl::GetFlag(FLAGS_mode); }

std::string CLI::get_config_path() const { return absl::GetFlag(FLAGS_config); }

}  // namespace bluestone
