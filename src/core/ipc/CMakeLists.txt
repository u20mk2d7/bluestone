# Inter-Process Communication
# src/core/ipc/CMakeLists.txt

find_package(Boost REQUIRED COMPONENTS system thread)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

# 2. CRITICAL: Create the executable FIRST
add_executable(bluestone_core_ipc
)

# 3. Link the necessary libraries (Removed GTest because you have your own main)
target_link_libraries(bluestone_core_ipc
    PRIVATE 
    Boost::boost
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
    #ROOT::Core ROOT::Hist
)

target_include_directories(bluestone_core_pic PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
)
