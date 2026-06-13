# src/gateway/CMakeLists.txt

add_subdirectory(interface)
add_subdirectory(binance)
add_subdirectory(lmax)

add_library(bluestone_gateway INTERFACE)

target_link_libraries(bluestone_gateway PUBLIC
    bluestone_gateway_interface
    bluestone_gateway_binance
    bluestone_gateway_lmax
)
