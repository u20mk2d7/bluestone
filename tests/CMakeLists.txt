# tests/CMakeLists.txt

# 1. Find packages
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

if(NOT TARGET Boost::beast)
    add_library(Boost::beast INTERFACE IMPORTED)
    set_target_properties(Boost::beast PROPERTIES INTERFACE_LINK_LIBRARIES Boost::boost)
endif()

# 2. CRITICAL: Create the executable FIRST
add_executable(bluestone_tests
    http_sync_client.cpp
)

# 3. Link the necessary libraries (Removed GTest because you have your own main)
target_link_libraries(bluestone_tests 
    PRIVATE 
    Boost::boost
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
    #ROOT::Core ROOT::Hist
)
