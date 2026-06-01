# cmake/quickfix.cmake

message(STATUS "Loading QuickFIX...")

set(QUICKFIX_ROOT "${THIRD_PARTY_ROOT}/quickfix")

find_library(QUICKFIX_LIBRARY NAMES quickfix 
    PATHS "${QUICKFIX_ROOT}/lib" 
    REQUIRED NO_DEFAULT_PATH)

find_path(QUICKFIX_INCLUDE_DIR NAMES quickfix/Application.h 
    PATHS "${QUICKFIX_ROOT}/include" 
    REQUIRED NO_DEFAULT_PATH)

if(QUICKFIX_LIBRARY AND QUICKFIX_INCLUDE_DIR)
    add_library(quickfix::quickfix UNKNOWN IMPORTED)
    set_target_properties(quickfix::quickfix PROPERTIES
        IMPORTED_LOCATION "${QUICKFIX_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${QUICKFIX_INCLUDE_DIR}"
    )
    message(STATUS "Found QuickFIX: ${QUICKFIX_LIBRARY}")
else()
    message(FATAL_ERROR "QuickFIX not found in ${QUICKFIX_ROOT}")
endif()
