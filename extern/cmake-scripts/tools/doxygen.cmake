#------------------------------------------------------------------------------
# Enables support for Doxygen builds.. Hurrah!
# It assumes the Doxygen related content to be stored in a folder named docs
# v0.1.0
#------------------------------------------------------------------------------
find_package(Doxygen)
if(Doxygen_FOUND)
  add_subdirectory(docs)
else()
  message(STATUS "Doxygen not found, not building docs")
endif()
