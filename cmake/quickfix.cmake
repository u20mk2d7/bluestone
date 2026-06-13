# cmake/quickfix.cmake
message(STATUS "Loading QuickFIX...")

# The build script installed directly into third_party, not a quickfix subdirectory
set(QUICKFIX_ROOT "${THIRD_PARTY_ROOT}")

find_library(QUICKFIX_LIBRARY NAMES quickfix 
  PATHS "${QUICKFIX_ROOT}/lib" 
  REQUIRED NO_DEFAULT_PATH)

find_path(QUICKFIX_INCLUDE_DIR NAMES quickfix/Application.h 
  PATHS "${QUICKFIX_ROOT}/include" 
  REQUIRED NO_DEFAULT_PATH)

if(QUICKFIX_LIBRARY AND QUICKFIX_INCLUDE_DIR)
  # Create the imported target
  add_library(quickfix::quickfix UNKNOWN IMPORTED)
  
  set_target_properties(quickfix::quickfix PROPERTIES
    IMPORTED_LOCATION "${QUICKFIX_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${QUICKFIX_INCLUDE_DIR}"
  )
  
  # CRITICAL FIX: QuickFIX requires OpenSSL. 
  # By adding it as an INTERFACE link, any target (like blackstone) 
  # that links against quickfix::quickfix will automatically link OpenSSL.
  find_package(OpenSSL REQUIRED)
  target_link_libraries(quickfix::quickfix INTERFACE OpenSSL::SSL OpenSSL::Crypto)

  message(STATUS "Found QuickFIX: ${QUICKFIX_LIBRARY}")
else()
  message(FATAL_ERROR "QuickFIX not found in ${QUICKFIX_ROOT}")
endif()
