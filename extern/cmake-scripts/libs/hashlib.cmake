#------------------------------------------------------------------------------
# Portable C++ Hashing Library
# - computes CRC32, MD5, SHA1 and SHA256 (most common member of the SHA2 functions), Keccak and its SHA3 sibling
# - optional HMAC (keyed-hash message authentication code)
# - no external dependencies, small code size
# - can work chunk-wise (for example when reading streams block-by-block)
# - portable: supports Windows and Linux, tested on Little Endian and Big Endian CPUs
# - roughly as fast as Linux core hashing functions
# - open source, zlib license
# https://github.com/stbrumme/hash-library
#------------------------------------------------------------------------------
option(ENABLE_HASHLIB "Adds a portable C++ hashing library" OFF)
if(ENABLE_HASHLIB)
  message(STATUS "HASH-LIBRARY:")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  
  message(STATUS "+ FETCHING HASH-LIBRARY....")
  include(FetchContent)
  FetchContent_Declare(
      hashlib
      GIT_REPOSITORY https://github.com/stbrumme/hash-library.git
      GIT_TAG origin/master)
  
  FetchContent_GetProperties(hashlib)
  if (NOT hashlib_POPULATED)
    message(STATUS "+ CONFIGURING HASH-LIBRARY....")
    FetchContent_Populate(hashlib)
    
    # create a library from source files
    add_library(hashlib
                ${hashlib_SOURCE_DIR}/crc32.cpp
                ${hashlib_SOURCE_DIR}/digest.cpp
                ${hashlib_SOURCE_DIR}/keccak.cpp
                ${hashlib_SOURCE_DIR}/md5.cpp
                ${hashlib_SOURCE_DIR}/sha3.cpp
                ${hashlib_SOURCE_DIR}/sha256.cpp)
    
    add_library(hashlib::hashlib ALIAS hashlib)
    
    target_include_directories(
        hashlib
        SYSTEM INTERFACE
        ${hashlib_SOURCE_DIR})
    
    message(DEBUG "HASH-LIBRARY SOURCE DIR: ${hashlib_SOURCE_DIR}")
  endif ()
  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()
