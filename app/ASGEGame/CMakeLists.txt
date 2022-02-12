## local variable for managing the game target ##
set(GAME ${PROJECT_NAME})

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

        # Scenes
        MainMenuScene.cpp
        LobbyScene.cpp
        GameScene.cpp

        # Bloody threading
        GigaScene.cpp

        # Gameobjects
        gameobjects/actors/units/Unit.cpp
        gameobjects/actors/units/Archer.cpp
        gameobjects/actors/units/Knight.cpp
        gameobjects/actors/units/Mage.cpp
        gameobjects/actors/units/UnitRangeHighlight.cpp

        gameobjects/actors/projectiles/Projectile.cpp

        gameobjects/network/Client.cpp

        # Testing
        NetworkingTest.cpp GigaScene.cpp GigaScene.hpp)

## the ASGE Game, which spawns the Game window ##
add_executable(${GAME} ${HEADER_LIST} ${GAME_SOURCES})

## place in its own directory
set_target_properties(
        ${GAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/client)

## configure the target. Dependency targets go in the LIBS variable i.e. soloud ##
list(APPEND LIBS "ASGEGameLib" "kissnet::kissnet" "magic_enum::magic_enum" "tmxlite")
configure_target(${GAME} "${CMAKE_CURRENT_SOURCE_DIR}" "${HEADER_LIST}" "${LIBS}")
add_asge_dependency(${GAME})

## icon support, because why not ##
if (WIN32)
    set(ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/game.rc)
elseif (APPLE)
    set(ICON ${CMAKE_CURRENT_SOURCE_DIR}/resources/game.icns)
    set_source_files_properties(
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/game.icns PROPERTIES
            MAXOSX_PACKAGE_LOCATION "resources")
endif()

## game data ##
add_gamedata(${GAME} ${CMAKE_CURRENT_SOURCE_DIR}/data)