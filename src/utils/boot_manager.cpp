#include "boot_manager.hpp"

#include <iostream>
#include <cstdlib>
#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <simdjson.h>

// 1. Define Abseil Flags at the top of the file
ABSL_FLAG(std::string, mode, "", "Execution mode (e.g., fix, run, test)");
ABSL_FLAG(std::string, config, "", "Path to the JSON config file");
ABSL_FLAG(uint32_t, instance, 0, "Bot instance ID for core pinning");

namespace Bluestone {
namespace Utils {

ExchangeConfig BootManager::initialize(int argc, char** argv) {
    ExchangeConfig cfg;

    // 1. Parse CLI via Abseil
    absl::ParseCommandLine(argc, argv);
    cfg.mode = absl::GetFlag(FLAGS_mode);
    cfg.instance_id = absl::GetFlag(FLAGS_instance);
    std::string config_path = absl::GetFlag(FLAGS_config);

    if (config_path.empty()) {
        std::cerr << "[Boot] FATAL: Missing --config flag.\n";
        std::exit(EXIT_FAILURE);
    }

    // 2. Load the JSON file into a padded SIMD string
    simdjson::padded_string json_data;
    simdjson::error_code load_error = simdjson::padded_string::load(config_path).get(json_data);
    
    if (load_error) {
        std::cerr << "[Boot] FATAL: Cannot open config file: " << config_path 
                  << " | Error: " << simdjson::error_message(load_error) << '\n';
        std::exit(EXIT_FAILURE);
    }

    // 3. Parse JSON using the ultra-fast On-Demand API
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc; // ✅ CORRECT: Must be ondemand::document
    
    simdjson::error_code parse_error = parser.iterate(json_data).get(doc);
    if (parse_error) {
        std::cerr << "[Boot] FATAL: Invalid JSON syntax in file: " << config_path 
                  << " | Error: " << simdjson::error_message(parse_error) << '\n';
        std::exit(EXIT_FAILURE);
    }

    // 4. Extract fields using a safe lambda
    auto extract_string = [&doc](std::string_view key) -> std::string {
        std::string_view val;
        // Search for the key and verify it is a string
        if (doc[key].get_string().get(val) == simdjson::SUCCESS) {
            return std::string(val);
        }
        return ""; 
    };

    cfg.host = extract_string("host");
    cfg.port = extract_string("port");
    cfg.target = extract_string("target");
    cfg.symbol = extract_string("symbol");
    cfg.event_type = extract_string("event_type");
    cfg.api_key = extract_string("api_key");
    cfg.private_key = extract_string("private_key");

    std::cout << "[Boot] Configuration loaded successfully for symbol: " << cfg.symbol << "\n";

    return cfg;
}

} // namespace Utils
} // namespace Bluestone
