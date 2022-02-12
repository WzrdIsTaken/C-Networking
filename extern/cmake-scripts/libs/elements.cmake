#------------------------------------------------------------------------------
# Elements C++ GUI library
# Elements is a lightweight, fine-grained, resolution independent, modular GUI library.
# Elements is designed with these requirements in mind:
# - It should be open source with a liberal, non-viral license.
# - It should be usable in any application and should play well with other GUI libraries and frameworks.
# - Corollary to the second requirement is that it can also be used to develop plugins.
# - It should not own the event loop and should be able to co-exist with components within a plugin host such as VST and AU.
# - It should be resolution independent and allow for HDPI displays.
# - Should not rely on a “visual” GUI editor or code generator.
# - It should have a declarative API using modern C++. Declarative C++ code tells you what rather than how (imperative).
# - The GUI should be declared in C++ code.
# - Keep it simple. Make it easy to use right out of the box, with a lot of examples.
# - Porting to a new host target should be as easy as possible, requiring porting of only a few files.
# https://github.com/cycfi/elements
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)
option(ENABLE_ELEMENTS "Adds Elements C++ GUI library" OFF)
if(ENABLE_ELEMENTS)
  message(STATUS  "ELEMENTS C++ GUI:")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  message(VERBOSE "Elements is a lightweight, fine-grained, resolution independent, modular GUI library")
  message(STATUS "+ FETCHING ELEMENTS C++ GUI....")
  
  set (ELEMENTS_BUILD_EXAMPLES    "OFF"  CACHE INTERNAL "OFF")
  include(FetchContent)
  FetchContent_Declare(
      elements
      GIT_REPOSITORY https://github.com/cycfi/elements.git
      GIT_TAG        origin/master)
  
  message(STATUS "+ CONFIGURING ELEMENTS C++ GUI....")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  FetchContent_MakeAvailable(elements)
  list(POP_BACK CMAKE_MESSAGE_INDENT)
  
  set_target_properties(elements PROPERTIES CXX_CLANG_TIDY "")
  set_target_properties(elements PROPERTIES CXX_CPPCHECK   "")
  
  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()