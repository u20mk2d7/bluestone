# root CMakeList.txt
cmake_minimum_required(VERSION 3.28)

project(bluestone LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(HAVE_SSL=1)

# ==================== PERFORMANCE & BUILD FLAGS ====================
include(${CMAKE_SOURCE_DIR}/cmake/performance.cmake)

# ==================== DEPENDENCIES & HELPERS ====================
# include(${CMAKE_SOURCE_DIR}/cmake/get_cpm.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)

# ==================== PLATFORM SPECIFIC ====================
if(APPLE)
    include(${CMAKE_SOURCE_DIR}/cmake/unix.cmake)
    add_compile_definitions(OS_MACOS=1)
    message(STATUS "Platform: macOS")
elseif(UNIX AND NOT APPLE)
    include(${CMAKE_SOURCE_DIR}/cmake/linux.cmake)
    add_compile_definitions(OS_LINUX=1)
    message(STATUS "Platform: Linux")
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

# ==================== SUBDIRECTORIES ====================
add_subdirectory(third_party/twsapi)
add_subdirectory(third_party/intel_dfp)

add_subdirectory(src/core)
add_subdirectory(src/utils)
add_subdirectory(src/gateways)

#add_subdirectory(test)

# ==================== MAIN EXECUTABLE ====================
add_executable(bluestone src/main.cpp)

target_include_directories(bluestone PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(bluestone PRIVATE 
    bluestone_core
    bluestone_utils
    bluestone_gateways
    bluestone_pch
    quickfix::quickfix
)

set_target_properties(bluestone PROPERTIES UNITY_BUILD ON)
message(STATUS "bluestone configured successfully!")
