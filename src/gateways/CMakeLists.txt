# src/gateways/CMakeLists.txt

message(STATUS "Configuring exchange gateways...")

add_library(bluestone_gateways STATIC
    # Binance WebSocket
    binance/binance_connector.cpp
    
    # Other exchanges later
    # interactivebrokers/tws_connector.cpp
)

target_include_directories(bluestone_gateways PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(bluestone_gateways PUBLIC 
    bluestone_core
    bluestone_utils
    bluestone_pch
    quickfix::quickfix
    Boost::boost
    simdjson::simdjson
    OpenSSL::SSL
    OpenSSL::Crypto
    Boost::program_options
    intel_dfp # intel decimal floating point math library
    abseil::abseil
)

set_target_properties(bluestone_gateways PROPERTIES UNITY_BUILD ON)

message(STATUS "Gateways library built successfully (WebSocket + FIX).")
