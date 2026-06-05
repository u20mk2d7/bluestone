#!/bin/bash
set -e  # Exit immediately if a command fails
#git pull 
#git add . && git commit -m "update" && git push origin main


# 2. Clean previous build (optional but recommended during heavy changes)
#rm -rf build/
#mkdir -p build/

echo "=== bluestone Build & Run Script ===" 

echo "Installing Conan dependencies..."
#conan install . --output-folder=build --build=missing --update -s build_type=Release


# 4. CMake Configuration
echo "Configuring CMake with Ninja..."
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=build/build/Release/generators/conan_toolchain.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release

# 5. Compilation
echo "Building project..."
cmake --build build -j 4

# 6. Synchronize Runtime Assets into Build Folder
echo "Syncing FIX configuration assets..."
cp .env/initiator.cfg build/
cp .env/FIX44.xml build/

echo "Running bluestone..." 
./build/bluestone "$@"
echo "=== Execution finished ==="
