# src/gateway/lmax/CMakeLists.txt

add_library(bluestone_gateway_lmax STATIC lmax_connector.cpp)

# Link the polymorphic interface AND the QuickFIX engine
target_link_libraries(bluestone_gateway_lmax PUBLIC 
    bluestone_gateway_interface
    quickfix
)

target_include_directories(bluestone_gateway_lmax PUBLIC ${CMAKE_SOURCE_DIR}/src)
