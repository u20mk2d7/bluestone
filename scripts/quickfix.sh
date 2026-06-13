#!/bin/bash
set -e

# Clean and recreate build directory
rm -rf build/
mkdir -p build/

echo "Configuring QuickFIX with CMake..."

# Ask Homebrew for the base OpenSSL directory (drops the hardcoded @3)
OPENSSL_PATH=$(brew --prefix openssl)

cmake -S . -B build -G Ninja \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_BUILD_TYPE=Release \
  -DHAVE_SSL=ON \
  -DOPENSSL_ROOT_DIR="$OPENSSL_PATH" \
  -DQUICKFIX_EXAMPLES=OFF \
  -DQUICKFIX_TESTS=OFF \
  -DCMAKE_INSTALL_PREFIX="$HOME/repos/third_party/libs/quickfix/install"

# Determine available cores dynamically
CORES=$(($(sysctl -n hw.ncpu 2>/dev/null || nproc) - 2))
if [ "$CORES" -lt 1 ]; then CORES=1; fi

echo "Building QuickFIX using $CORES cores..."
cmake --build build -j $CORES

echo "Installing QuickFIX..."
cmake --install build

