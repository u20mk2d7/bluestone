# src/common/CMakeLists.txt

message(STATUS "bluestone common")

add_library(bluestone_common STATIC
	# *.cpp
)


# add_subdirectory()

target_include_directories(bluestone_common PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bluestone_common PUBLIC
)

message(STATUS "bluestone common library")

