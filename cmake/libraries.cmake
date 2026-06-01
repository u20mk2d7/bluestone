# cmake/libraries.cmake
message(STATUS "Loading Conan / system libraries...")

# Core Utilities & Logic
find_package(Boost REQUIRED)
find_package(simdjson REQUIRED)
find_package(absl REQUIRED)
find_package(flatbuffers REQUIRED)
find_package(protobuf REQUIRED)

# Math & Science
find_package(Eigen3 REQUIRED)

# Security & Network
find_package(OpenSSL REQUIRED)

# Testing & Benchmarking
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

find_package(QuantLib QUIET)
if(QuantLib_FOUND)
    message(STATUS "=== QuantLib found ===")
endif()


message(STATUS "All libraries loaded successfully.")
