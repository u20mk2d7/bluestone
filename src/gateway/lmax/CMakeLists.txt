# src/gateway/lmax/CMakeLists.txt

add_library(bluestone_gateway_lmax STATIC lmax_connector.cpp)

# Link the polymorphic interface, your new QuickFIX target, and QuantLib
target_link_libraries(bluestone_gateway_lmax PUBLIC 
    bluestone_gateway_interface
    quickfix::quickfix
    QuantLib::QuantLib
    OpenSSL::SSL
    OpenSSL::Crypto
)

target_include_directories(bluestone_gateway_lmax PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
