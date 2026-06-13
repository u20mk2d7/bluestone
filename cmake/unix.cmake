# cmake/unix.cmake

# Append the base paths where your custom-built libraries reside
list(APPEND CMAKE_PREFIX_PATH
  "${THIRD_PARTY_ROOT}"
)

include(${CMAKE_SOURCE_DIR}/cmake/libraries.cmake)
