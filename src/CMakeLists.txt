## local variable for managing the gamelib target
set(GAMELIB "ASGEGameLib")

## glob is useful but not reliant in compilation so just use it for headers ##
file(GLOB_RECURSE HEADER_LIST
     LIST_DIRECTORIES FALSE
     CONFIGURE_DEPENDS
     ${CMAKE_SOURCE_DIR}/include/${GAMELIB}/*.hpp
     ${CMAKE_SOURCE_DIR}/include/${GAMELIB}/*.h
     *.hpp *.h)

## cpp files go here, [h]pp files are globbed so don't need to be explicitly added ##
set(GAMELIB_SOURCES
        # Driver
        ${GAMELIB}/Driver.cpp
        ${GAMELIB}/Scene.cpp
        ${GAMELIB}/Component.cpp

        # Driver Handlers
        ${GAMELIB}/Handlers/InputHandler.cpp
        ${GAMELIB}/Handlers/SceneHandler.cpp
        ${GAMELIB}/Handlers/ThreadHandler.cpp
        ${GAMELIB}/Handlers/RenderHandler.cpp

        # Networking Magic
        ${GAMELIB}/Components/Networking/NetServer.cpp
        ${GAMELIB}/Components/Networking/NetClient.cpp
        ${GAMELIB}/Components/Networking/NetServerDebugger.cpp
        ${GAMELIB}/Components/Networking/Packet.cpp

        # Components
        ${GAMELIB}/Components/TileMap/TileMap.cpp
        ${GAMELIB}/Components/TileMap/PathFinder.cpp

        ${GAMELIB}/Components/Animator/Animator.cpp

        # Containers
        ${GAMELIB}/Containers/Vector2.cpp
        ${GAMELIB}/Containers/Float4.cpp)

## create the game library
add_library(${GAMELIB} STATIC ${GAMELIB_SOURCES} "${HEADER_LIST}")
add_library(libs::${GAMELIB} ALIAS ${GAMELIB})
target_include_directories(${GAMELIB} PRIVATE ${CMAKE_SOURCE_DIR}/include/${GAMELIB} ${CMAKE_SOURCE_DIR}/src/${GAMELIB})
target_compile_options(${GAMELIB} PUBLIC $<$<STREQUAL:x${CMAKE_CXX_SIMULATE_ID},xMSVC>:/EHsc>)

## set the build target properties ##
set(LINK_LIBS "kissnet::kissnet" "magic_enum::magic_enum" "tmxlite" )
configure_target(${GAMELIB} ${CMAKE_SOURCE_DIR}/include "${HEADER_LIST}" "${LINK_LIBS}" )
add_asge_dependency(${GAMELIB})
