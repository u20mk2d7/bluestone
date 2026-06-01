# ==============================================================================
# src/utils/CMakeLists.txt (The Infrastructure)
# ==============================================================================

add_library(bluestone_core_utils INTERFACE)

# Tell anyone who links utils to look in the main src/ directory for includes
target_include_directories(bluestone_core_utils INTERFACE ${CMAKE_SOURCE_DIR}/src)

# Threading is a system-level requirement, so we attach it to the utils backpack
find_package(Threads REQUIRED)
target_link_libraries(bluestone_core_utils INTERFACE 
    Threads::Threads 
    Boost::boost
)

