# cmake/get_cpm.cmake
message(STATUS "cmake/get_cpm.cmake")

set(CPM_DOWNLOAD_VERSION 0.42.3) # Use the latest version
set(CPM_DOWNLOAD_LOCATION "${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")

if(NOT EXISTS "${CPM_DOWNLOAD_LOCATION}")
  message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
  file(DOWNLOAD
    "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake"
    "${CPM_DOWNLOAD_LOCATION}"
  )
endif()

include("${CPM_DOWNLOAD_LOCATION}")
