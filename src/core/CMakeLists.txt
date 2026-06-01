# src/core/CMakeLists.txt

message(STATUS "Configuring Bluestone Core Engine...")

add_library(bluestone_core STATIC
    trade_engine.cpp
    # Add new .cpp files here when you create them
)

# Add subdirectories
add_subdirectory(exchange)
add_subdirectory(fix)
add_subdirectory(marketdata)
add_subdirectory(execution)
add_subdirectory(risk)
add_subdirectory(strategy)
add_subdirectory(tws)
add_subdirectory(utils)

target_include_directories(bluestone_core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bluestone_core PUBLIC 
    bluestone_core_utils
    bluestone_pch
    Boost::boost
    quickfix::quickfix
)

message(STATUS "Core library (bluestone_core) built successfully.")
