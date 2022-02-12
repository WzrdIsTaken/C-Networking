#------------------------------------------------------------------------------
# Reliable UDP networking library
# ENet's purpose is to provide a relatively thin, simple and robust network
# communication layer on top of UDP (User Datagram Protocol).The primary feature
# it provides is optional reliable, in-order delivery of packets.
#
# ENet omits certain higher level networking features such as authentication,
# lobbying, server discovery, encryption, or other similar tasks that are particularly
# application specific so that the library remains flexible, portable, and
# easily embeddable.
# https://github.com/lsalzman/enet
#------------------------------------------------------------------------------
OPTION(ENABLE_ENET "Adds Networking to the Project" OFF)
if(ENABLE_ENET)
  message(STATUS  "ENET UDP NETWORKING:")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  message(VERBOSE "Reliable UDP networking library")
  message(STATUS "+ FETCHING ENET UDP NETWORKING....")
  
  # fetch project
  include(FetchContent)
  FetchContent_Declare(
      enet
      GIT_REPOSITORY https://github.com/lsalzman/enet
      GIT_TAG        0eaf48eeb0d94a18d079378d8b76d588832ce838)
  
  FetchContent_GetProperties(enet)
  if(NOT enet_POPULATED)
    message(STATUS "+ CONFIGURING ENET UDP NETWORKING....")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    FetchContent_Populate(enet)
    add_subdirectory(${enet_SOURCE_DIR} ${enet_BINARY_DIR})
    list(POP_BACK CMAKE_MESSAGE_INDENT)
  endif()
  
  message(STATUS "+ FETCHING ENET C++ WRAPPER....")
  FetchContent_Declare(
      enetpp
      GIT_REPOSITORY https://github.com/HuxyUK/enetpp
      GIT_TAG        1f4ab305cfa96c4d506486affcc3acaf64992c50)
  
  FetchContent_GetProperties(enetpp)
  if(NOT enetpp_POPULATED)
    FetchContent_Populate(enetpp)
    add_library(enetpp INTERFACE)
    target_include_directories(
        enetpp
        SYSTEM INTERFACE
        ${enet_SOURCE_DIR}/include
        ${enetpp_SOURCE_DIR}/include)
    add_dependencies(enetpp enet)
    target_link_libraries(enetpp INTERFACE enet)
  endif()
  
  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()
