# src/core/utils/CMakeLists.txt # SPSC queues, memory arenas, TSC timers

add_library(bluestone_core_utils INTERFACE)

target_include_directories(bluestone_core_utils
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

find_package(Threads REQUIRED)
target_link_libraries(bluestone_core_utils INTERFACE 
    Threads::Threads 
)

