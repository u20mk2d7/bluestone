#!/bin/bash
set -e # Exit on any error
mkdir -p build && cd build

conan install . --build=missing -s build_type=Release

cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=../build/conan_toolchain.cmake \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_BUILD_TYPE=Release \
    -DQL_BUILD_BENCHMARK=OFF \
    -DQL_BUILD_EXAMPLES=OFF \
    -DQL_BUILD_TEST_SUITE=OFF \
    -DOPENSSL_ROOT_DIR=/usr/bin/openssl \
    -DCMAKE_INSTALL_PREFIX="/home/hatd6/repo/bluestone/third_party/quantlib"

make -j$(($(nproc) - 2))
make install