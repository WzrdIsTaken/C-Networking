#------------------------------------------------------------------------------
# STDUUID
# C++17 IMPLEMENTATION OF PROPOSAL P0959
# https://github.com/mariusbancila/stduuid.git
#------------------------------------------------------------------------------

option(ENABLE_STDUUID "Adds UUID support" OFF)
if(ENABLE_STDUUID)
  message(STATUS  "STDUUID:")
  message(VERBOSE "STDUUID C++17 IMPLEMENTATION OF PROPOSAL P0959" )
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  
  # fetch project
  include(FetchContent)
  message(STATUS "+ FETCHING STDUUID....")
  FetchContent_Declare(
      stduuid
      GIT_REPOSITORY https://github.com/mariusbancila/stduuid.git
      GIT_TAG        origin/master)
  
  FetchContent_GetProperties(stduuid)
  if(NOT stduuid_POPULATED)
    message(STATUS "+ CONFIGURING STDUUID....")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    FetchContent_Populate(stduuid)
    
    # create a header only library
    add_library(stduuid INTERFACE)
    add_library(stduuid::stduuid ALIAS stduuid)
    target_include_directories(
        stduuid
        SYSTEM INTERFACE
        ${stduuid_SOURCE_DIR}/include)
    message(DEBUG "READER_WRITER_QUEUE INCLUDE DIR: ${stduuid_SOURCE_DIR}/include")
    
    # apparently this helps with some of dem IDEs
    target_sources(stduuid INTERFACE ${stduuid_SOURCE_DIR}/include/uuid.h)
    
    list(POP_BACK CMAKE_MESSAGE_INDENT)
  endif()
  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()