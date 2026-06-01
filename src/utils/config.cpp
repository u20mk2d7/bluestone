
#include "utils/config.hpp"

#include <simdjson.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace Bluestone {

  Bluestone::ExchangeConfig ConfigParser::parse_json(
      const std::string& file_path) {
    //
    Bluestone::ExchangeConfig cfg;
    boost::property_tree::ptree pt;

    try {
      // Read the JSON file into the property tree
      boost::property_tree::read_json(file_path, pt);

      // Extract values (using a default fallback "" avoids exceptions if a key
      // is missing)
      cfg.exchange = pt.get<std::string>("exchange", "");
      cfg.host = pt.get<std::string>("host", "");
      cfg.port = pt.get<std::string>("port", "");
      cfg.target = pt.get<std::string>("target", "");
      cfg.symbol = pt.get<std::string>("symbol", "");
      cfg.event_type = pt.get<std::string>("event_type", "");

      cfg.api_key = pt.get<std::string>("api_key", "");
      cfg.secret = pt.get<std::string>("secret", "");

      std::cout << "[Config] Loaded successfully: " << cfg.exchange
                << " | Symbol: " << cfg.symbol << "\n";

    } catch (const boost::property_tree::json_parser_error& e) {
      std::cerr << "[Config] JSON parse failed at " << file_path << ":"
                << e.line() << " - " << e.message() << '\n';
    } catch (const std::exception& e) {
      std::cerr << "[Config] Cannot open or read file: " << file_path
                << " | Error: " << e.what() << '\n';
    }

    return cfg;
  }

}  // namespace Bluestone
