#------------------------------------------------------------------------------
# spdlog
# spdlog is a library to parse and emit YAML, and do it fast
# https://github.com/gabime/spdlog.git
#------------------------------------------------------------------------------
option(ENABLE_SPDLOG "Adds SPD Logging Support" OFF)
if(ENABLE_SPDLOG)
  message(STATUS  "SPDLOG:")
  message(VERBOSE "spdlog is a library to parse and emit YAML, and do it fast.")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  
  include(FetchContent)
  message(STATUS "+ FETCHING SPDLOG....")
  FetchContent_Declare(
      spdlog
      GIT_REPOSITORY https://github.com/gabime/spdlog.git
      GIT_TAG        origin/v1.x)
  
  FetchContent_GetProperties(spdlog)
  if(NOT spdlog_POPULATED)
    message(STATUS "+ CONFIGURING SPDLOG....")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    FetchContent_Populate(spdlog)
    
    # create a header only library
    add_library(spdlog INTERFACE)
    add_library(spdlog::libspdlog ALIAS spdlog)
    target_include_directories(
        spdlog
        SYSTEM INTERFACE
        ${spdlog_SOURCE_DIR}/include)
    message(DEBUG "SPDLOG INCLUDE DIR:  ${spdlog_SOURCE_DIR}/include")
    
    list(POP_BACK CMAKE_MESSAGE_INDENT)
  endif()
  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()