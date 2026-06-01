# src/core/fix/CMakeLists.txt

add_library(bluestone_fix STATIC
    fix_session.cpp
)

target_include_directories(bluestone_fix PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bluestone_fix PUBLIC 
    bluestone_core
    bluestone_pch
    quickfix::quickfix
    Boost::boost
)

message(STATUS "FIX engine library built.")
