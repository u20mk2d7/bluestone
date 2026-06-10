# test/CMakeLists.txt

# 1. Find the required packages
# Note: You may not need these if they are already exported globally 
# in your 'cmake/dependencies.cmake' file.
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

# ========================================================
# Example 1: Boost Beast (e.g., an HTTP/WebSocket client)
# ========================================================
add_executable(beast_example
	http_client_sync.cpp
)

# Link Boost, OpenSSL, and standard Threads
target_link_libraries(beast_example PRIVATE 
    Boost::boost
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
)

# Optional: Set C++ standard just for safety, though it inherits C++23 from root
set_target_properties(beast_example PROPERTIES CXX_STANDARD 23)

# ========================================================
# Example 2: Pure OpenSSL (e.g., encryption/decryption)
# ========================================================
add_executable(openssl_example 
	websocket.cpp
)

target_link_libraries(openssl_example PRIVATE 
    OpenSSL::SSL
    OpenSSL::Crypto
)
