# src/core/ipc/CMakeLists.txt

# Inter-Process Communication
add_library(bluestone_core_ipc INTERFACE)

find_package(Boost REQUIRED)

# Link the necessary libraries 
target_link_libraries(bluestone_core_ipc
    INTERFACE
    Boost::boost
    #ROOT::Core ROOT::Hist
)

# FIXED: Changed PUBLIC to INTERFACE and removed the trailing keyword
target_include_directories(bluestone_core_ipc 
    INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
