# cmake/performance.cmake

message(STATUS "Loading performance optimizations...")

# Enforce Release build if this file is explicitly included
if(NOT CMAKE_BUILD_TYPE MATCHES "Release")
    message(WARNING "Performance optimizations loaded, but CMAKE_BUILD_TYPE is '${CMAKE_BUILD_TYPE}'. Forcing Release mode.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_compile_options(
    -O3 
    -march=native         # Optimizes strictly for the host architecture (implies -mtune=native)
    
    # -------------------------------------------------------------------------
    # DANGEROUS IN QUANT FINANCE - USE WITH EXTREME CAUTION
    # -------------------------------------------------------------------------
    # -ffast-math         # Breaks IEEE 754 compliance. Can cause NaN/Inf checks to silently fail.
    
    # -------------------------------------------------------------------------
    # Temporarily Disabled Flags (Tier 1 Hot Path Optimizations)
    # -------------------------------------------------------------------------
    # -funroll-loops      # Unroll loops for hot-path speed
    # -fno-exceptions     # Disabled to allow building; strictly avoids exceptions in Tier 1
    # -fno-rtti           # Disable RTTI overhead (avoids dynamic_cast in hot path)
    
    # -------------------------------------------------------------------------
    # Strict Warnings (Currently Disabled)
    # -------------------------------------------------------------------------
    # -Wall -Wextra -Wpedantic -Werror 
)

# Include LLD + LTO
include(${CMAKE_SOURCE_DIR}/cmake/lld.cmake)
