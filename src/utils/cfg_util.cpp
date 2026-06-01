
#include "utils/cfg_util.hpp"

#include <simdjson.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace bluestone {

bluestone::ExchangeConfig ConfigParser::parse_json(
    std::string_view path_to_file) {
  //
  bluestone::ExchangeConfig cfg;

  // 1. Load the file using padded_string for SIMD safety
  simdjson::padded_string json_data;

  // 2. load() reads the file path and returns
  // simdjson_result<simdjson::padded_string>
  // 3. .get() takes the empty json_data and moves the file contents into it
  simdjson::error_code load_error =
      simdjson::padded_string::load(std::string(path_to_file)).get(json_data);
  if (load_error) {
    std::cerr << "[Config] Cannot open or read file: " << path_to_file
              << " | Error: " << simdjson::error_message(load_error) << '\n';
    return cfg;
  }

  simdjson::dom::parser parser;  // simdjson::ondemand::parser parser;
  simdjson::dom::element doc; // simdjson::ondemand::document doc;
  simdjson::error_code parse_error = parser.iterate(json_data).get(doc);
  if (parse_error) {
    std::cerr << "[Config] Invalid JSON syntax in file: " << path_to_file 
              << " | Error: " << simdjson::error_message(parse_error) << '\n';
    return cfg;
  }

  // 3. Helper lambda: Pass in the 'doc' object we just created
  auto extract_string = [&doc](const char* key) -> std::string {
    std::string_view val;
    // Safely check if the key exists and successfully extract it
    if (doc[key].get_string().get(val) == simdjson::SUCCESS) {
      return std::string(val);
    }
    return "";  // Fallback if key is missing or not a string
  };

  // 4. Extract all fields
  cfg.host        = extract_string("host");
  cfg.port        = extract_string("port");
  cfg.target      = extract_string("target");
  cfg.symbol      = extract_string("symbol");
  cfg.event_type  = extract_string("event_type");
  cfg.api_key     = extract_string("api_key");
  cfg.private_key = extract_string("private_key");

  std::cout << "[Config] Loaded successfully: "
            << (cfg.host.empty() ? "Unknown" : cfg.host)
            << " | Port: " << (cfg.port.empty() ? "Unknown" : cfg.port) << "\n";

  return cfg;
  }

}  // namespace bluestone
