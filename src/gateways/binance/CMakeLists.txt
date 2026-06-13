# src/gateways/binance/CMakeLists.txt

add_library(bluestone_gateway_binance INTERFACE)

# Expose the include path to everything that links to this interface
target_include_directories(bluestone_gateway_binance INTERFACE 
    ${CMAKE_SOURCE_DIR}/src
)
