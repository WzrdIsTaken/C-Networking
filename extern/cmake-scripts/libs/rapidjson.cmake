#------------------------------------------------------------------------------
# Rapid JSON
# ryml is a library to parse and emit YAML, and do it fast
# https://github.com/Tencent/rapidjson
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)
option(ENABLE_RAPID_JSON "Adds RapidJSON support" OFF)
if(ENABLE_RAPID_JSON)

    set(RAPIDJSON_BUILD_DOC         OFF CACHE BOOL "Builds rapidjson documentation"             FORCE)
    set(RAPIDJSON_BUILD_EXAMPLES    OFF CACHE BOOL "Builds rapidjson examples"                  FORCE)
    set(RAPIDJSON_BUILD_TESTS       OFF CACHE BOOL "Builds rapidjson perftests and unittests"   FORCE)

    message(STATUS  "RAPID_JSON:")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    message(VERBOSE "RapidJSON is a library to parse and emit JSON")
    message(STATUS "+ FETCHING APP....")
    include(FetchContent)
    FetchContent_Declare(
            rapidjson
            GIT_REPOSITORY https://github.com/Tencent/rapidjson
            GIT_TAG        master)
    FetchContent_Populate(rapidjson)

    message(STATUS "+ CONFIGURING RAPID_JSON....")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    # create a header only library
    add_library(rapidjson INTERFACE)
    target_include_directories(
            rapidjson
            SYSTEM INTERFACE
            ${rapidjson_SOURCE_DIR}/include)

    message(DEBUG "JSON INCLUDE DIR:${rapidjson_SOURCE_DIR}")
    list(POP_BACK CMAKE_MESSAGE_INDENT)

    message(STATUS "+ DONE")
    list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()