#------------------------------------------------------------------------------
# Eventbus is a simple, header only C++17 event bus library that doesn't require
# you to inherit from any sort of event class.
#
# eventbus implements the "Mediator" pattern. This pattern is useful when you
# want components to communicate to each other without necessarily "knowing" about
# each other. Effectively, this is a thread safe event dispatcher with a list of
# callbacks.
# https://github.com/DeveloperPaul123/eventbus
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)
option(ENABLE_DPEVENTBUS "Adds EventBus library" OFF)
if(ENABLE_DPEVENTBUS)
  message(STATUS  "DP-EVENTBUS C++:")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  
  message(VERBOSE "Eventbus is a simple, header only C++17 event bus library")
  message(STATUS "+ FETCHING DP-EVENTBUS....")
  set(ENABLE_TEST OFF CACHE INTERNAL "Enable Test")
  
  # fetch project
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  include(FetchContent)
  FetchContent_Declare(
      DPEventBus
      GIT_REPOSITORY https://github.com/DeveloperPaul123/eventbus.git
      GIT_TAG        1bee8b301a3fdf136864f96d43c202613471637d)
  
  FetchContent_MakeAvailable(DPEventBus)
  list(POP_BACK CMAKE_MESSAGE_INDENT)
  
  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()