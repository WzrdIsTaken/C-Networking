## local variable for managing the game target ##
set(GAME ASGENetServer)

## glob is useful but not reliant in compilation so just use it for headers ##
file(GLOB_RECURSE HEADER_LIST
        LIST_DIRECTORIES FALSE
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

## source files get listed here ##
set(GAME_SOURCES
        # The carry
        main.cpp

        # Server
        Server.cpp
        ServerDebugger.cpp)

## the ASGE Game, which spawns the Game window ##
add_executable(${GAME} ${HEADER_LIST} ${GAME_SOURCES})

## place in its own directory
set_target_properties(
        ${GAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/server)

## configure the target. Dependency targets go in the LIBS variable i.e. soloud ##
list(APPEND LIBS "ASGEGameLib" "kissnet::kissnet" "magic_enum::magic_enum" "tmxlite")
configure_target(${GAME} "${CMAKE_CURRENT_SOURCE_DIR}" "${HEADER_LIST}" "${LIBS}")
add_asge_dependency(${GAME})

## game data ##
add_gamedata(${GAME} ${CMAKE_CURRENT_SOURCE_DIR}/data)