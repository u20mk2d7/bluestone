#include "cli.hpp"

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>

#include <string>

ABSL_FLAG(std::string, mode, "", "Execution mode");
