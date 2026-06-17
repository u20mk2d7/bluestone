#!/bin/bash
set -e  # Exit immediately if a command fails

git pull 
git add . && git commit -m "update" && git push origin main

#rm -rf build/
#mkdir -p build/
#conan install . --output-folder=build --build=missing --update -s build_type=Release

cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=build/build/Release/generators/conan_toolchain.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release

# Determine available cores dynamically (works on macOS and Linux)
if [[ "$OSTYPE" == "darwin"* ]]; then
    CORES=$(($(sysctl -n hw.ncpu) - 4))
else
    # Linux approach using nproc
    CORES=$(($(nproc) - 4))
fi
if [ "$CORES" -lt 1 ]; then CORES=1; fi

cmake --build build -j $CORES 

#./build/bluestone --exchange lmax --config .env/app.cfg --instance 1

./build/tests/bluestone_tests
