#------------------------------------------------------------------------------
# A single-producer, single-consumer lock-free queue for C++
# This mini-repository has my very own implementation of a lock-free queue for C++.
#
# It only supports a two-thread use case (one consuming, and one producing).
# The threads can't switch roles, though you could use this queue completely from
# a single thread if you wish (but that would sort of defeat the purpose!)
# https://github.com/cameron314/readerwriterqueue
#------------------------------------------------------------------------------
option(ENABLE_CONCURRENT_QUEUE "Adds a single-producer, single-consumer queue library" OFF)
if(ENABLE_CONCURRENT_QUEUE)
  message(STATUS  "READER-WRITER-QUEUE:")
  message(VERBOSE "A single-producer, single-consumer lock-free queue for C++")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")
  
  message(STATUS "+ FETCHING READER-WRITER-QUEUE....")
  include(FetchContent)
  FetchContent_Declare(
      readerwriterqueue
      GIT_REPOSITORY https://github.com/cameron314/readerwriterqueue.git
      GIT_TAG        origin/master)
  
  FetchContent_GetProperties(readerwriterqueue)
  if(NOT readerwriterqueue_POPULATED)
    FetchContent_Populate(readerwriterqueue)
    message(STATUS "+ CONFIGURING READER-WRITER-QUEUE....")
    
    # create a header only library
    add_library(readerwriterqueue INTERFACE)
    add_library(moodycamel::readerwriterqueue ALIAS readerwriterqueue)
    target_include_directories(
        readerwriterqueue
        SYSTEM INTERFACE
        ${readerwriterqueue_SOURCE_DIR})
    message(DEBUG "READER_WRITER_QUEUE INCLUDE DIR: ${readerwriterqueue_SOURCE_DIR}")
    
    # apparently this helps with some of dem IDEs
    target_sources(readerwriterqueue INTERFACE
                   ${readerwriterqueue_SOURCE_DIR}/atomicops.h
                   ${readerwriterqueue_SOURCE_DIR}/readerwriterqueue.h)
    message(DEBUG "READER_WRITER_QUEUE SOURCE DIR: ${readerwriterqueue_SOURCE_DIR}")
  
  endif()
  message(STATUS "+ DONE")
  list(POP_BACK CMAKE_MESSAGE_INDENT)
endif()