#include "boot_manager.hpp"

#include <iostream>
#include <cstdlib>
#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <simdjson.h>

// CLI Flag: ABSL_FLAG(type, name, default, help-text)
// ./app --exchange binance --config .env/binance.json --instance 1
// ./app --exchange ctrader --config .env/initiator.cfg --instance 2

namespace bluestone {
ExchangeConfig BootManager::initialize(int argc, char** argv) {
    
} // namespace Bluestone
