# https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  option(GIT_SUBMODULE "Check submodules during build" ON)
  message(STATUS "SUBMODULE UPDATE\t\t\t\t ON")
    if (GIT_SUBMODULE)
      file(LOCK ${CMAKE_SOURCE_DIR} DIRECTORY GUARD FILE)
      list(APPEND CMAKE_MESSAGE_INDENT "  ")
      execute_process(
          COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --remote
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
          RESULT_VARIABLE GIT_SUBMOD_RESULT)

      if (NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
      endif ()

      execute_process(
          COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
          WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/extern/cmake-scripts"
          OUTPUT_VARIABLE PACKAGE_GIT_VERSION
          ERROR_QUIET
          OUTPUT_STRIP_TRAILING_WHITESPACE)
      message(STATUS "COMMIT ID \t\t\t\t\t ${PACKAGE_GIT_VERSION}")
      message(STATUS "HOOKS \t\t\t\t\t\t ENABLED")
      execute_process(
          COMMAND ${GIT_EXECUTABLE} config core.hooksPath ${PROJECT_SOURCE_DIR}/extern/cmake-scripts/githooks
          WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
          OUTPUT_QUIET
          OUTPUT_STRIP_TRAILING_WHITESPACE)

      find_program(
          CFE NAMES clang-format clang-format-12 clang-format-11 clang-format-10
          HINTS
          "${CMAKE_SOURCE_DIR}/tools/*/${PLATFORM}/"
          "$ENV{ProgramFiles}\\LLVM\\bin")

      OPTION(ENABLE_CLANG_FORMAT "Adds clang-format as a hook" OFF)
      if(ENABLE_CLANG_FORMAT AND CFE)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} config hooks.clangFormatExe ${CFE}
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            OUTPUT_QUIET)
        message(STATUS "CLANG FORMAT HOOK \t\t\t ENABLED")
      else()
        execute_process(
            COMMAND ${GIT_EXECUTABLE} config --unset hooks.clangFormatExe
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            OUTPUT_QUIET)
      endif()

      file(LOCK ${CMAKE_SOURCE_DIR} DIRECTORY RELEASE)
      list(POP_BACK CMAKE_MESSAGE_INDENT "  ")
    endif()
  endif ()

if (NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/cmake-scripts/CMakeLists.txt")
  message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif ()

