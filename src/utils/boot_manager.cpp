#include "boot_manager.hpp"

#include <iostream>
#include <cstdlib>
#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <simdjson.h>

// 1. Define Abseil Flags
ABSL_FLAG(std::string, mode, "run", "Execution mode (e.g., fix, run, test)");
ABSL_FLAG(std::string, config, "", "Optional: Path to the JSON config file (e.g., binance_ws.json)");
ABSL_FLAG(std::string, fix_cfg, "", "Optional: Path to the QuickFIX config file (e.g., ctrader_fix.cfg)");
ABSL_FLAG(uint32_t, instance, 0, "Bot instance ID for core pinning");

namespace Bluestone {
namespace Utils {

ExchangeConfig BootManager::initialize(int argc, char** argv) {
    ExchangeConfig cfg;

    // 1. Parse CLI via Abseil
    absl::ParseCommandLine(argc, argv);
    
    // Extract standard CLI arguments
    cfg.mode = absl::GetFlag(FLAGS_mode);
    cfg.instance_id = absl::GetFlag(FLAGS_instance);
    cfg.quickfix_config_path = absl::GetFlag(FLAGS_fix_cfg);
    
    std::string config_path = absl::GetFlag(FLAGS_config);

    // 2. 🚀 THE BYPASS LOGIC
    // If no JSON config is provided, we just return the CLI variables.
    if (config_path.empty()) {
        std::cout << "[Boot] INFO: No JSON config provided. Booting via CLI parameters only.\n";
        return cfg; 
    }

    // 3. Load the JSON file (Only executes if --config was provided)
    simdjson::padded_string json_data;
    simdjson::error_code load_error = simdjson::padded_string::load(config_path).get(json_data);
    
    if (load_error) {
        std::cerr << "[Boot] FATAL: Cannot open config file: " << config_path 
                  << " | Error: " << simdjson::error_message(load_error) << '\n';
        std::exit(EXIT_FAILURE);
    }

    // 4. Parse JSON using the ultra-fast On-Demand API
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc; 
    
    simdjson::error_code parse_error = parser.iterate(json_data).get(doc);
    if (parse_error) {
        std::cerr << "[Boot] FATAL: Invalid JSON syntax in file: " << config_path 
                  << " | Error: " << simdjson::error_message(parse_error) << '\n';
        std::exit(EXIT_FAILURE);
    }

    // 5. Safely extract JSON fields
    auto extract_string = [&doc](std::string_view key) -> std::string {
        std::string_view val;
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
    cfg.api_passphrase = extract_string("api_passphrase");
    cfg.private_key = extract_string("private_key");

    // If the JSON contains a FIX config path, it overrides the CLI flag
    std::string json_fix_cfg = extract_string("quickfix_config_path");
    if (!json_fix_cfg.empty()) {
        cfg.quickfix_config_path = json_fix_cfg;
    }

    std::cout << "[Boot] JSON Configuration loaded successfully.\n";

    return cfg;
}

} // namespace Utils
} // namespace Bluestone
