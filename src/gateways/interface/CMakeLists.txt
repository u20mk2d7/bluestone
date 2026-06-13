# src/gateways/interface/CMakeLists.txt

add_library(bluestone_gateways_interface INTERFACE)

target_include_directories(bluestone_gateways_interface INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
