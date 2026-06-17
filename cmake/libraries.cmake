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

#find_package(ROOT REQUIRED COMPONENTS RIO Net Hist) # https://root.cern/
#include(${ROOT_USE_FILE}) # no need

message(STATUS "All libraries loaded successfully.")
