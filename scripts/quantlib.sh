#!/bin/bash
set -e # Exit on any error

rm -rf build/
mkdir -p build

# Added build_type=Release to ensure Conan fetches the optimized packages
conan install . --output-folder=build --build=missing -s build_type=Release
OPENSSL_PATH=$(brew --prefix openssl)
cmake -S . -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="build/Release/generators/conan_toolchain.cmake" \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_BUILD_TYPE=Release \
  -DQL_BUILD_BENCHMARK=OFF \
  -DQL_BUILD_EXAMPLES=OFF \
  -DQL_BUILD_TEST_SUITE=OFF \
  -DOPENSSL_ROOT_DIR="$OPENSSL_PATH" \
  -DCMAKE_INSTALL_PREFIX="$HOME/repos/bluestone/third_party"

# Determine available cores dynamically natively on macOS
CORES=$(($(sysctl -n hw.ncpu) - 2))
if [ "$CORES" -lt 1 ]; then CORES=1; fi

cmake --build build -j $CORES 

cmake --install build

