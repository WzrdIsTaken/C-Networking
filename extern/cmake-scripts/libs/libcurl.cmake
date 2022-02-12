#------------------------------------------------------------------------------
# Libcurl is a library for accessing, downloading and uploading content.
# This script current only supports pre-compiled windows builds stored
# in external/curl
#------------------------------------------------------------------------------
OPTION(ENABLE_LIBCURL "Adds LIBCURL to the Project" OFF)

if(ENABLE_LIBCURL)
  message(STATUS  "LIBCURL LIBRARY:")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  
  # create an imported library to use for linking
  add_library(LIBCURL::LIBCURL STATIC IMPORTED)
  
  ## set the library directories ##
  if(WIN32)
    set(CURL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/curl/include)
    set(CURL_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/external/curl/build/lib/x64)
  endif()
  
  ## postfix to ensure we link to the correct libs ##
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(POSTFIX "d")
  else()
    set(POSTFIX "")
  endif()
  
  message(STATUS "+ SEARCHING FOR LIB....")
  find_library(
      CURL_LIBRARY REQUIRED
      NAMES   libcurl${POSTFIX}.lib
      PATHS   ${CURL_LIBRARY_DIR} NO_DEFAULT_PATH )
  
  message(STATUS "+ CURL_LIBRARY: " ${CURL_LIBRARY})
  
  set_target_properties(LIBCURL::LIBCURL PROPERTIES IMPORTED_LOCATION  ${CURL_LIBRARY})
  set_target_properties(LIBCURL::LIBCURL PROPERTIES DEBUG_POSTFIX      "d")
  set_target_properties(LIBCURL::LIBCURL PROPERTIES CXX_CLANG_TIDY     " ")
  set_target_properties(LIBCURL::LIBCURL PROPERTIES CXX_CPPCHECK       " ")
  
  ## library includes
  target_include_directories(
      LIBCURL::LIBCURL
      SYSTEM
      INTERFACE
      "${CURL_INCLUDE_DIR}")
  
  message(DEBUG "CURL_LIBRARY INCLUDE DIR: ${CURL_INCLUDE_DIR}")
  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()