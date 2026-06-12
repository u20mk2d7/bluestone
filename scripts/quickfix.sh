#!/bin/bash
set -e # Exit on any error
mkdir -p build/ && cd build/

cmake .. \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_BUILD_TYPE=Release \
    -DQUICKFIX_ENABLE_SSL=ON \
    -DOPENSSL_ROOT_DIR=/usr/bin/openssl \
    -DCMAKE_INSTALL_PREFIX="/home/hatd6/repo/bluestone/third_party/quickfix"

make -j$(($(nproc) - 2))
make install
