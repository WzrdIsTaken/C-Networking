#------------------------------------------------------------------------------
# SoLoud sound engine
# https://github.com/HuxyUK/soloud.git
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)
OPTION(ENABLE_SOUND "Adds SoLoud to the Project" OFF)

if(ENABLE_SOUND)
  message(STATUS  "SOLOUD:")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  
  # Enable a native-ish build of Audio Engine
  if (WIN32)
    set(SOLOUD_BACKEND_WASAPI    "ON" CACHE INTERNAL "ON")
    set(SOLOUD_BACKEND_WINMM     "ON" CACHE INTERNAL "ON")
  elseif(APPLE)
    set(SOLOUD_BACKEND_COREAUDIO "ON" CACHE INTERNAL "ON")
  else()
    find_package(ALSA QUIET)
    if (ALSA_FOUND)
      set(SOLOUD_BACKEND_ALSA  "ON" CACHE INTERNAL "ON")
    endif()
  endif()
  
  if(BUILD_SHARED_LIBS)
    set(SOLOUD_DYNAMIC           "ON"  CACHE INTERNAL "ON")
    set(SOLOUD_STATIC            "OFF" CACHE INTERNAL "OFF")
  else()
    set(SOLOUD_DYNAMIC           "OFF" CACHE INTERNAL "OFF")
    set(SOLOUD_STATIC            "ON"  CACHE INTERNAL "ON")
  endif()
  
  include(FetchContent)
  message(STATUS "+ FETCHING SOLOUD....")
  FetchContent_Declare(
      soloud
      GIT_REPOSITORY https://github.com/HuxyUK/soloud.git
      GIT_TAG        master)
  
  message(STATUS "+ CONFIGURING SOLOUD....")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  FetchContent_GetProperties(soloud)
  if(NOT soloud_POPULATED)
    FetchContent_Populate(soloud)
    add_subdirectory(${soloud_SOURCE_DIR}/contrib ${soloud_BINARY_DIR})
  endif()
  list(POP_BACK CMAKE_MESSAGE_INDENT)
  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()
