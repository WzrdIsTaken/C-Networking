#------------------------------------------------------------------------------
# Rapid YAML
# ryml is a library to parse and emit YAML, and do it fast
# https://github.com/biojppm/rapidyaml
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)
option(ENABLE_RAPID_YAML "Adds RapidYAML support" OFF)
if(ENABLE_RAPID_YAML)
  message(STATUS  "RAPID_YAML:")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  
  message(VERBOSE "ryml is a library to parse and emit YAML, and do it fast")
  message(STATUS "+ FETCHING APP....")
  include(FetchContent)
  FetchContent_Declare(
      rapidyaml
      GIT_REPOSITORY https://github.com/biojppm/rapidyaml
      GIT_TAG        5390bb9329b4034e1caea408faae24399b5bc7a8)
  
  message(STATUS "+ CONFIGURING RAPID_YAML....")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  FetchContent_MakeAvailable(rapidyaml)
  list(POP_BACK CMAKE_MESSAGE_INDENT)
  
  set_target_properties(ryml PROPERTIES CXX_CLANG_TIDY "")
  set_target_properties(ryml PROPERTIES CXX_CPPCHECK   "")
  
  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()