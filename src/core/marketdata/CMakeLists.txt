# src/core/marketdata/CMakeLists.txt # SPSC queues, memory arenas, TSC timers

add_library(bluestone_core_marketdata STATIC
	order_book.cpp
)

target_include_directories(bluestone_core_marketdata
	PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

find_package(Threads REQUIRED)
find_package(absl REQUIRED)
target_link_libraries(bluestone_core_marketdata
	PUBLIC 
	Threads::Threads 
	absl::absl
)

