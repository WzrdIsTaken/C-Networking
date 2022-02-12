cmake_minimum_required(VERSION 3.14)
option(ENABLE_TMXLITE "Adds TmxLite library" OFF)

if(ENABLE_TMXLITE)
    message(STATUS  "TMXLITE:")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    message(VERBOSE "A lightweight C++14 parsing library for tmx map files created with the Tiled map editor. ")
    message(STATUS "+ FETCHING TMXLITE TILED IMPORTER....")

    if(BUILD_SHARED_LIBS)
        set (TMXLITE_STATIC_LIB "OFF" CACHE INTERNAL "OFF")
    else()
        set (TMXLITE_STATIC_LIB "ON"  CACHE INTERNAL "ON")
    endif()

    include(FetchContent)
    FetchContent_Declare(
            tmxlite
            GIT_REPOSITORY https://github.com/HuxyUK/tmxlite.git
            GIT_TAG        origin/master)

    message(STATUS "+ CONFIGURING TMXLITE....")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    FetchContent_GetProperties(tmxlite)
    if(NOT tmxlite_POPULATED)
        # Fetch the content using previously declared details
        FetchContent_Populate(tmxlite)

        # Set custom variables, policies, etc.
        # ...

        # Bring the populated content into the build
        add_subdirectory(${tmxlite_SOURCE_DIR}/tmxlite ${tmxlite_BINARY_DIR})
    endif()

    list(POP_BACK CMAKE_MESSAGE_INDENT)

    set_target_properties(tmxlite PROPERTIES CXX_CLANG_TIDY "")
    set_target_properties(tmxlite PROPERTIES CXX_CPPCHECK   "")
    set_target_properties(tmxlite PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:tmxlite,INTERFACE_INCLUDE_DIRECTORIES>)

    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND ("x${CMAKE_CXX_SIMULATE_ID}" STREQUAL "xMSVC"))
        target_compile_options(tmxlite PRIVATE /EHsc)
    endif()

    message(STATUS "+ DONE")
    list(POP_BACK CMAKE_MESSAGE_INDENT)

endif()