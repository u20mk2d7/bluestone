# src/utils/CMakeLists.txt

message(STATUS "Configuring utilities...")

add_library(bluestone_utils STATIC
    event_loop_worker.cpp
    config_loader.cpp
)

target_include_directories(bluestone_utils PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bluestone_utils PUBLIC 
    bluestone_pch
    Boost::boost
    simdjson::simdjson
    absl::flags  
    absl::flags_parse
    
)

message(STATUS "Utils library (bluestone_utils) built successfully.")
