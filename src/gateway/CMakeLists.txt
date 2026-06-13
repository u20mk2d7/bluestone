# src/gateway/CMakeLists.txt

# 1. Add the subdirectories
add_subdirectory(interface)
add_subdirectory(binance)
add_subdirectory(lmax)

# 2. THE FIX: Create the master gateway library as an INTERFACE (Virtual) target
add_library(bluestone_gateway INTERFACE)

# 3. Link the sub-libraries into the virtual master target
target_link_libraries(bluestone_gateway INTERFACE
    bluestone_gateway_interface
    bluestone_binance
    bluestone_lmax
)
