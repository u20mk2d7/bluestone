# cmake/linux.cmake
include(${CMAKE_SOURCE_DIR}/cmake/lld.cmake)

list(APPEND CMAKE_PREFIX_PATH "${THIRD_PARTY_ROOT}/quantlib")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPERFTOOLS REQUIRED libtcmalloc)

find_package(CUDAToolkit QUIET)

include(${CMAKE_SOURCE_DIR}/cmake/libraries.cmake)
