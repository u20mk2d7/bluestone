# src/gateway/lmax/CMakeLists.txt

# 1. Create an isolated static library just for LMAX
add_library(bluestone_gateway_lmax STATIC lmax_connector.cpp)

# 2. Link it to the pure polymorphic interface
target_link_libraries(bluestone_gateway_lmax PUBLIC bluestone_gateway_gateway_interface)

# 3. Allow it to find the header files starting from the src/ root
target_include_directories(bluestone_gateway_lmax PUBLIC ${CMAKE_SOURCE_DIR}/src)
