#[[ A specially curated CMAKE file designed to handle
    importing of all the necessary libraries to run ASGE games.
    It's important to note that due to the way CMake operates,
    static libraries requires linking to all dependencies in
    the build chain. Using shared, only requires the use of a
    single DLL.

    Shared Build    => Executable
                    => asge.dll

    Static Build    => Executable   static app
                    => OpenGL       static library, provided by OS
                    => GLAD         static library
                    => GLFW         static library
                    => Freetype     static library
                    => PhysFS       static library
                    => PhysFS++     static library
                    => GameEngine   static library

    NB: You should not need to edit this file, unless adding new libraries to your game ]]
OPTION(ENABLE_ASGE "Adds ASGE support to the Project" OFF)
if (ENABLE_ASGE AND NOT TARGET ASGE)
  message(STATUS "##### ASGE SETUP #####")
  add_library(ASGE UNKNOWN IMPORTED)
  set_target_properties(ASGE PROPERTIES IMPORTED_GLOBAL TRUE)

  if(NOT ASGE_VERSION)
    set(ASGE_VERSION "2.0.0-alpha")
    set(ASGE_VERSION_EXTRA "+5")
  endif()

  set(ASGE_BASEURL "https://github.com/HuxyUK/ASGE/releases/download/v${ASGE_VERSION}")
  
  if (NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    set(ASGE_URL "${ASGE_BASEURL}/ASGE-${ASGE_VERSION}${ASGE_VERSION_EXTRA}-${CMAKE_CXX_PLATFORM_ID}.zip")
  else ()
    set(ASGE_URL "${ASGE_BASEURL}/ASGE-devel-${ASGE_VERSION}${ASGE_VERSION_EXTRA}-${CMAKE_CXX_PLATFORM_ID}.zip")
  endif ()
  
  message(STATUS ${ASGE_URL})
  
  ## download ASGE ##
  include(FetchContent)
  fetchcontent_declare(
      ASGE
      URL ${ASGE_URL}
      # URL_HASH MD5=67855acdfb6dcc922bd0c08add137e75
      SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/asge-src
  )
  
  fetchcontent_getproperties(ASGE)
  if (NOT asge_POPULATED)
    fetchcontent_populate(asge)
  endif ()
  
  ## set the library directories ##
  if (WIN32)
    set(ASGE_BIN_DIRECTORY ${asge_SOURCE_DIR}/bin)
    set(ASGE_LIB_DIRECTORY ${asge_SOURCE_DIR}/lib)
  else ()
    set(ASGE_BIN_DIRECTORY ${asge_SOURCE_DIR}/lib)
    set(ASGE_LIB_DIRECTORY ${asge_SOURCE_DIR}/lib)
  endif ()
  
  ## postfix to ensure we link to the correct libs ##
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(POSTFIX "d")
  else ()
    set(POSTFIX "")
  endif ()
  
  if (BUILD_SHARED_LIBS)
    ## shared libraries only require a single link to game engine
    find_library(libGameEngine REQUIRED
                 NAMES
                 libGameEngine${POSTFIX}.dll.a
                 libGameEngine${POSTFIX}.so
                 libGameEngine${POSTFIX}.dylib
                 GameEngine${POSTFIX}.lib
                 asge${POSTFIX}
                 asge${POSTFIX}.dll.a
                 asge${POSTFIX}.so
                 asge${POSTFIX}.dylib
                 PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)
    message(STATUS "libGameEngine: " ${libGameEngine})
    
    if(WIN32)
      find_file(GameEngineDLL
                NAMES GameEngine${POSTFIX}.dll asge${POSTFIX}.dll
                PATHS ${ASGE_BIN_DIRECTORY}
                REQUIRED
                NO_DEFAULT_PATH)
      message(STATUS "GameEngineDLL: " ${GameEngineDLL})
      set_property(TARGET ASGE PROPERTY IMPORTED_LOCATION ${GameEngineDLL})
    else()
      set_property(TARGET ASGE PROPERTY IMPORTED_LOCATION ${libGameEngine})
    endif()
  else()
    ## static builds are hideous
    find_library(libFreetype REQUIRED NAMES freetype${POSTFIX} PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)
    find_library(libPhysFS REQUIRED NAMES physfs${POSTFIX} physfs-static${POSTFIX} PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)
    find_library(libPhysFS++ REQUIRED NAMES physfscpp${POSTFIX} physfscpp-static${POSTFIX} PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)
    find_library(libGLFW REQUIRED NAMES glfw3${POSTFIX} PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)
    find_library(libGLAD REQUIRED NAMES glad${POSTFIX} PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)
    
    set(OpenGL_GL_PREFERENCE "GLVND")
    find_package(OpenGL REQUIRED)
    message(STATUS "libOpenGL:     " ${OPENGL_LIBRARIES})
    
    find_library(libGameEngine REQUIRED
                 NAMES libGameEngine${POSTFIX}.a libGameEngine${POSTFIX}.lib GameEngine${POSTFIX}.lib asge${POSTFIX}
                 PATHS ${ASGE_LIB_DIRECTORY} NO_DEFAULT_PATH)
    set_property(TARGET ASGE PROPERTY IMPORTED_LOCATION ${libGameEngine})
    
    message(STATUS "libGLAD:       " ${libGLAD})
    message(STATUS "libGLFW:       " ${libGLFW})
    message(STATUS "libFreetype:   " ${libFreetype})
    message(STATUS "libPhysFS:     " ${libPhysFS})
    message(STATUS "libPhysFS++:   " ${libPhysFS++})
    message(STATUS "libGameEngine: " ${libGameEngine})
    
    ## all platforms need these libraries
    list(APPEND LINK_LIBS ${libGLAD} ${libGLFW} ${libFreetype} ${libPhysFS++} ${libPhysFS} ${OPENGL_LIBRARIES})
    
    # mac specific libraries
    if (PLATFORM MATCHES "OSX")
      find_library(IOKIT IOKit)
      find_library(COCOA Cocoa)
      find_library(COREVIDEO CoreVideo)
      
      message(STATUS "IOKit:        " ${IOKIT})
      message(STATUS "Cocoa:        " ${COCOA})
      message(STATUS "CoreVideo:    " ${COREVIDEO})
      
      list(APPEND LINK_LIBS
           -lz -lbz2
           ${AUDIO_UNIT}
           ${AUDIO_TBOX}
           ${COCOA}
           ${COREVIDEO}
           ${IOKIT} )
    endif ()
    
    # linux specific libraries
    if (PLATFORM MATCHES "LINUX")
      find_package(X11 REQUIRED)
      find_package(ZLIB REQUIRED)
      find_package(BZip2 REQUIRED)
      find_package(PNG REQUIRED)

      message(STATUS "libX11:        " ${X11_LIBRARIES})
      message(STATUS "libZLIB:       " ${ZLIB_LIBRARIES})
      message(STATUS "libBZip2:      " ${BZIP2_LIBRARIES})
      message(STATUS "libPNG:        " ${PNG_LIBRARIES})

      list(APPEND LINK_LIBS
           ${X11_LIBRARIES}
           ${X11_Xi_LIB}
           ${X11_Xrandr_LIB}
           ${X11_Xxf86vm_LIB}
           ${X11_Xinerama_LIB}
           ${X11_Xcursor_LIB}
           ${ZLIB_LIBRARIES}
           ${BZIP2_LIBRARIES}
           ${PNG_LIBRARIES}
           ${CMAKE_DL_LIBS}
           harfbuzz brotlidec pthread )   #threading
    endif ()
    
    set_property(
        TARGET ASGE PROPERTY
        INTERFACE_LINK_LIBRARIES
        ${LINK_LIBS})
  endif ()
  
  target_include_directories(
      ASGE
      SYSTEM INTERFACE
      ${asge_SOURCE_DIR}/include)
  
  set_target_properties(
      ASGE PROPERTIES
      URL https://github.com/HuxyUK/ASGE.git
      DESCRIPTION "An OpenGL Gaming Framework"
      PURPOSE "Enables ASGE Games")
endif ()

function(add_asge_dependency NAME)
  add_dependencies(${NAME} ASGE)
  target_link_libraries(${NAME} PRIVATE ASGE)

  if (BUILD_SHARED_LIBS AND WIN32)
    get_target_property(ASGE_DLL ASGE IMPORTED_LOCATION)
    add_custom_command(
        TARGET ${NAME} PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ASGE_DLL}"
        $<TARGET_FILE_DIR:${NAME}>)
  endif ()
endfunction()
