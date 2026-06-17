# src/core/CMakeLists.txt

message(STATUS "Configuring bluestone Core Engine...")

add_library(bluestone_core STATIC
    trade_engine.cpp
    # Add new .cpp files here when you create them
)

# Add subdirectories
add_subdirectory(fix)
add_subdirectory(marketdata)
add_subdirectory(risk)
add_subdirectory(strategy)
add_subdirectory(utils)
add_subdirectory(ipc) # Inter-Process Communication

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
