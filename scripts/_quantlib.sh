#!/bin/bash
set -e

# Clean and recreate build directory
rm -rf build/
mkdir -p build/

# Added build_type=Release to ensure Conan fetches the optimized packages
conan install . --output-folder=build --build=missing -s build_type=Release 

OPENSSL_PATH=""
if command -v brew &> /dev/null; then
    OPENSSL_PATH=$(brew --prefix openssl)
elif command -v openssl &> /dev/null; then
    # Linux: If openssl is in path, assume standard system location (/usr)
    # CMake usually finds it automatically in /usr/lib and /usr/include, or ~/conan/.../openssl
    OPENSSL_PATH=$(which openssl)
    echo "OpenSSL detected at: $OPENSSL_PATH. Letting CMake find it automatically."
else
    echo "Warning: OpenSSL not found via brew or system path."
fi

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

  
# Determine available cores dynamically (works on macOS and Linux)
if [[ "$OSTYPE" == "darwin"* ]]; then
    CORES=$(($(sysctl -n hw.ncpu) - 2))
else
    # Linux approach using nproc
    CORES=$(($(nproc) - 2))
fi
if [ "$CORES" -lt 1 ]; then CORES=1; fi

cmake --build build -j $CORES 

cmake --install build

