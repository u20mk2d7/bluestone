# cmake/unix.cmake
# include(${CMAKE_SOURCE_DIR}/cmake/lld.cmake)

list(APPEND CMAKE_PREFIX_PATH
	"${THIRD_PARTY_ROOT}/quantlib"
	"${THIRD_PARTY_ROOT}/quickfix"
)

include(${CMAKE_SOURCE_DIR}/cmake/libraries.cmake)

