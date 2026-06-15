#!/bin/bash
set -e

# Clean and recreate build directory
rm -rf build/
mkdir -p build/

echo "Configuring QuickFIX with CMake..."
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
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_BUILD_TYPE=Release \
  -DHAVE_SSL=ON \
  -DOPENSSL_ROOT_DIR="$OPENSSL_PATH" \
  -DQUICKFIX_EXAMPLES=OFF \
  -DQUICKFIX_TESTS=OFF \
  -DCMAKE_INSTALL_PREFIX="$HOME/repos/bluestone/third_party"

# Determine available cores dynamically
CORES=$(($(sysctl -n hw.ncpu 2>/dev/null || nproc) - 2))
if [ "$CORES" -lt 1 ]; then CORES=1; fi

echo "Building QuickFIX using $CORES cores..."
cmake --build build -j $CORES

echo "Installing QuickFIX..."
cmake --install build

