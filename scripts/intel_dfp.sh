cd LIBRARY
mkdir build && cd build

# Compile all .c files directly into ARM64 object files (.o)
$(brew --prefix llvm)/bin/clang -arch arm64 -O3 \
  -D__WANT_DEC_FLOAT__=1 \
  -Dnn_64_bit_pointer=1 \
  -DCALL_BY_REF=0 \
  -DGLOBAL_RND=0 \
  -DGLOBAL_FLAGS=0 \
  -DUNCHANGED_BINARY_FLAGS=0 \
  -I../src -c ../src/*.c
ar rcs libbid.a *.o

# Archive the object files into the static library

# Create the local folders in your project
# mkdir -p ~/path/to/blackstone/external/intel_dfp/lib
# mkdir -p ~/path/to/blackstone/external/intel_dfp/include

# Copy the library and headers directly into your project
#cp libbid.a ~/repos/bluestone/third_party/intel_dfp/lib 
#cp ../src/*.h ~/repos/bluestone/third_party/intel_dfp/include
#
echo "Build complete! libbid.a generated successfully."
