# src/gateway/CMakeLists.txt

# 1. Add the new interface subdirectory
add_subdirectory(interface)
# 2. Add your specific exchange implementations
add_subdirectory(binance)
add_subdirectory(lmax)

# 3. Create the master gateways library
add_library(bluestone_gateway STATIC "")

# 4. Link the interface and implementations into the master library
target_link_libraries(bluestone_gateway PUBLIC
    bluestone_gateway_interface    # The new interface target
    bluestone_gateway_binance
    bluestone_gateway_lmax
)
