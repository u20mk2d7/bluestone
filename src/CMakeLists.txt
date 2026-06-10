# src/CMakeLists.txt

message(STATUS "Configuring src/ directory...")

# Add submodules
add_subdirectory(utils)
add_subdirectory(core)
add_subdirectory(gateways)

# Main executable
add_executable(bluestone main.cpp)

target_include_directories(bluestone PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bluestone PRIVATE 
    bluestone_core_exchange
    bluestone_utils
    bluestone_gateways
    bluestone_pch
    quickfix::quickfix
    Boost::boost
    abseil::abseil
)

# Enable Unity Build for faster compilation
set_target_properties(bluestone PROPERTIES UNITY_BUILD ON)

message(STATUS "Main executable 'bluestone' configured.")

#configure_file(
#    "${CMAKE_CURRENT_SOURCE_DIR}/.env/lmax.cfg"
#    COPYONLY
#)
