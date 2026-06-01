# cmake/dependencies.cmake

message(STATUS "Loading dependencies.cmake ...")

# Third-party root
set(DEFAULT_LIBS_DIR "${CMAKE_SOURCE_DIR}/../third_party/libs")
get_filename_component(DEFAULT_LIBS_DIR "${DEFAULT_LIBS_DIR}" ABSOLUTE)
set(THIRD_PARTY_ROOT "${DEFAULT_LIBS_DIR}" CACHE PATH "Third-party libraries root")

if(DEFINED ENV{THIRD_PARTY_ROOT})
    set(THIRD_PARTY_ROOT "$ENV{THIRD_PARTY_ROOT}")
endif()

message(STATUS "Third-party root: ${THIRD_PARTY_ROOT}")

include(${CMAKE_SOURCE_DIR}/cmake/pch.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/quickfix.cmake)
