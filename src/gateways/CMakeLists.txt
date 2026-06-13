# src/gateways/CMakeLists.txt

# 1. Add the new interface subdirectory
add_subdirectory(interface)

# 2. Add your specific exchange implementations
add_subdirectory(binance)
add_subdirectory(lmax)
# add_subdirectory(ibkr) # If you have it

# 3. Create the master gateways library
add_library(bluestone_gateways STATIC "")

# 4. Link the interface and implementations into the master library
target_link_libraries(bluestone_gateways PUBLIC
    bluestone_gateway_interface    # The new interface target
    bluestone_binance
    bluestone_lmax
)
