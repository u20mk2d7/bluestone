# cmake/lld.cmake
message(STATUS "Configuring LLD Interprocedural Optimization (IPO) / Link Time Optimization (LTO)")

# Use LLVM LLD linker (much faster linking)
add_link_options("-fuse-ld=lld")

# Check and enable Interprocedural Optimization (IPO) / Link Time Optimization (LTO)
include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)
if(supported)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  message(STATUS "LTO/IPO enabled for better performance")
  
  # ThinLTO for Clang (best balance)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options("-flto=thin")
    add_link_options("-flto=thin")
  endif()
endif()

message(STATUS "LLD Interprocedural Optimization (IPO) / Link Time Optimization (LTO)")
