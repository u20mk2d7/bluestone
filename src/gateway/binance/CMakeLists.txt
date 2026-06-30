# src/gateway/binance/CMakeLists.txt

add_library(bluestone_gateway_binance STATIC binance_connector.cpp)

# Link the polymorphic interface AND the external networking libraries
target_link_libraries(bluestone_gateway_binance
  PUBLIC 
  bluestone_gateway_interface
  Boost::boost
  OpenSSL::SSL
  OpenSSL::Crypto
  simdjson::simdjson
)

target_include_directories(bluestone_gateway_binance
  PUBLIC
  ${CMAKE_SOURCE_DIR}/src
)
