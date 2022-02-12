function(configure_target NAME INCLUDE_DIR HEADER_LIST LINK_LIBS)
    ## check if already exists
    set_target_properties(${NAME} PROPERTIES LINKER_LANGUAGE           CXX)
    set_target_properties(${NAME} PROPERTIES CXX_EXTENSIONS            OFF)
   #set_target_properties(${GAME} PROPERTIES POSITION_INDEPENDENT_CODE FALSE)

    target_include_directories(${NAME} INTERFACE ${INCLUDE_DIR})
    target_compile_features(${NAME} PUBLIC cxx_std_20)
    source_group(TREE "${INCLUDE_DIR}" PREFIX "Header Files" FILES ${HEADER_LIST})

    ## logging levels for target ##
    target_compile_definitions(
            ${NAME} PRIVATE
            "$<$<CONFIG:Debug>:LOGGING_LEVEL_DEBUG>"
            "$<$<CONFIG:Release>:LOGGING_LEVEL_ERROR>")

    ## enable compiler warnings and debug flags ##
    set_target_build_flags(${NAME})

    foreach(LIBRARY ${LINK_LIBS})
        add_dependencies(${NAME} ${LIBRARY})
        target_link_libraries(${NAME} PRIVATE ${LIBRARY})
    endforeach()

    if(WIN32)
        if (CMAKE_COMPILER_IS_GNUCC AND NOT CMAKE_BUILD_TYPE STREQUAL  "Debug")
            target_link_libraries(${NAME} PRIVATE -static)
            target_compile_options(${NAME} PRIVATE -mwindows)
        endif()
    endif()

    ## fix issue with arch enabling gcc pie by default
    ## https://stackoverflow.com/questions/43367427/32-bit-absolute-addresses-no-longer-allowed-in-x86-64-linux
    if(UNIX AND CMAKE_COMPILER_IS_GNUCC)
        target_link_libraries(${NAME} PRIVATE -no-pie pthread)
    endif()

endfunction()