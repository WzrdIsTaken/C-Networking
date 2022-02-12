cmake_minimum_required(VERSION 3.12...3.17)

#------------------------------------------------------------------------------
# 3rd party library support
#------------------------------------------------------------------------------
set(ENABLE_ASGE          ON  CACHE BOOL "Adds ASGE to the Project" FORCE)
set(ENABLE_CLANG_FORMAT  ON  CACHE BOOL "Adds Clang-Format Hooks" FORCE)
set(ENABLE_JSON          OFF CACHE BOOL "Adds YAML to the Project" FORCE)
set(ENABLE_KISSNET       ON  CACHE BOOL "Adds Kissnet Sockets to Project" FORCE)
set(ENABLE_LIVE_ANALYSIS ON  CACHE BOOL "Adds live Clang-Tidy static analysis" FORCE)
set(ENABLE_MAGIC_ENUM    ON  CACHE BOOL "Adds reflection support for ENUMS" FORCE)
set(ENABLE_SOUND         OFF CACHE BOOL "Adds SoLoud Audio" FORCE)
set(ENABLE_TMXLITE       ON CACHE BOOL "Adds TmxLite library" FORCE)


#------------------------------------------------------------------------------
# Project settings
#------------------------------------------------------------------------------
project(ASGENetGame
        VERSION 1.0
        DESCRIPTION "ASGENetGame: a most excellent networked game!!!"
        LANGUAGES CXX)

message(STATUS "")
message(STATUS "##### ${PROJECT_NAME} Project configuration #####")
message(STATUS "")

set(ASGE_VERSION "3.1.1")


#------------------------------------------------------------------------------
# General settings
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/extern/cmake-scripts" ${CMAKE_MODULE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${PROJECT_NAME}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${PROJECT_NAME}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build/${PROJECT_NAME}/bin")


#------------------------------------------------------------------------------
# Static and shared build configurations
#------------------------------------------------------------------------------
option(STATIC_APP "Build a static application" ON)
message(STATUS " ")

if( STATIC_APP )
    message(STATUS "APPLICATION STATIC BUILD: \t YES")
    message(STATUS "APPLICATION SHARED BUILD: \t NO")
    set(BUILD_SHARED_LIBS false)
else()
    message(STATUS "APPLICATION STATIC BUILD: \t NO")
    message(STATUS "APPLICATION SHARED BUILD: \t YES")
    set(BUILD_SHARED_LIBS true)
endif()


#------------------------------------------------------------------------------
# Add the libraries and executable projects
#------------------------------------------------------------------------------
include(submodule)
include(tools/cppcheck)
include(tools/clang-tidy)
include(tools/clang-format)
include(tools/gamedata)
add_subdirectory(extern/cmake-scripts)
add_subdirectory(src)
add_subdirectory(app)
