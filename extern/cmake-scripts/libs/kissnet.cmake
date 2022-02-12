#------------------------------------------------------------------------------
# Keep It Simple Stupid NETwork.
# https://github.com/Ybalrid/kissnet
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)

## To enable KISSNET support override the default option setting
OPTION(ENABLE_KISSNET "Adds C++17 Network Sockets" OFF)
if (ENABLE_KISSNET)
  #message(STATUS "##### KISSNET SETUP #####:")
  message(STATUS "KISSNET:")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  message(VERBOSE "A lightweight, header only, crossplatform C++17 socket library.")

  include(FetchContent)
  FetchContent_Declare(
      kissnet
      GIT_REPOSITORY https://github.com/HuxyUK/kissnet
      GIT_TAG master)

  FetchContent_GetProperties(kissnet)
  if(NOT kissnet_POPULATED)
    message(STATUS "+ CONFIGURING KISSNET....")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    FetchContent_Populate(kissnet)

    # create a header only library
    add_library(kissnet INTERFACE)
    add_library(kissnet::kissnet ALIAS kissnet)
    target_include_directories(
        kissnet
        SYSTEM INTERFACE
        ${kissnet_SOURCE_DIR})
    message(STATUS "INCLUDE DIR: ${kissnet_SOURCE_DIR}")

    # apparently this helps with some of dem IDEs
    target_sources(kissnet INTERFACE ${kissnet_SOURCE_DIR}/kissnet.hpp)

    list(POP_BACK CMAKE_MESSAGE_INDENT)
  endif()
  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()

