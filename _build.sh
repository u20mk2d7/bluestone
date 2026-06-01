#!/bin/bash
set -e  # Exit immediately if a command fails
# 2. Clean previous build (optional but recommended during heavy changes)
#rm -rf build/
mkdir -p build/
echo "=== Bluestone Build & Run Script ===" 

# 1. Sync Code (Optional - uncomment when needed)
# git pull 
# git add . && git commit -m "update" && git push origin main

# 3. Conan Dependency Management
echo "Installing Conan dependencies..."

#conan install . --output-folder=build --build=missing --update -s build_type=Release


# 4. CMake Configuration
echo "Configuring CMake with Ninja..."

cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=build/build/Release/generators/conan_toolchain.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release

# 5. Compilation
echo "Building project..."

cmake --build build -j 6 

# 6. Run the executable
echo "Running Bluestone..." 

./build/bluestone "$@"

echo "=== Execution finished ==="
