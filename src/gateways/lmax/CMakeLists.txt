# src/gateways/lmax/CMakeLists.txt

add_library(bluestone_gateway_lmax INTERFACE)

# Expose the include path to everything that links to this interface
target_include_directories(bluestone_gateway_lmax INTERFACE 
    ${CMAKE_SOURCE_DIR}/src
)
