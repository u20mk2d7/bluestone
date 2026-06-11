# test/CMakeLists.txt

# 1. Initialize and find all required packages for the Beast examples
# Conan's CMakeDeps generator will automatically expose targets like Boost::beast
find_package(Boost REQUIRED COMPONENTS system thread)
# 2. Find OpenSSL (This sets the OPENSSL_FOUND variable required by Beast subdirectories)
find_package(OpenSSL REQUIRED)

# 3. Find standard threading libraries
find_package(Threads REQUIRED)

if(NOT TARGET Boost::beast)
    add_library(Boost::beast INTERFACE IMPORTED)
    set_target_properties(Boost::beast PROPERTIES INTERFACE_LINK_LIBRARIES Boost::boost)
endif()
# ========================================================
# Your Custom Tests (Optional)
# ========================================================
# add_executable(my_custom_test custom_test.cpp)
# target_link_libraries(my_custom_test PRIVATE Boost::boost OpenSSL::SSL OpenSSL::Crypto Threads::Threads)
# set_target_properties(my_custom_test PROPERTIES CXX_STANDARD 23)

# ========================================================
# Ignite the Official Beast Examples Tree
# ========================================================
# Because OPENSSL_FOUND is now set to TRUE in this scope, the beast/advanced 
# folder will successfully generate the server-flex and server-flex-awaitable targets.
add_subdirectory(beast)
