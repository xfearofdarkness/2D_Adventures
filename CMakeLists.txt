cmake_minimum_required(VERSION 3.00)
project(2D_Adventures)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/res")
file(GLOB RESOURCES_FILES "${RESOURCE_DIR}/*")

if(WIN32)
    if(MSVC)
        message(STATUS "Configuring MSVC build")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MD")


        add_definitions(-D_CRT_SECURE_NO_WARNINGS)

        # Enable exception handling and other flags
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W4")
    endif()
endif()

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # Find raylib (quietly)
if (NOT raylib_FOUND) # If raylib is not found, fetch and build it
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Download raylib if not already fetched
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # Disable raylib examples
    endif()
endif()

# Add your executable
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Player.cpp
        src/Player.h
        src/Enemy.cpp
        src/Enemy.h
        src/Level.cpp
        src/Level.h
        src/Tile.h
        src/raygui.h
        src/Inventory.cpp
        src/Inventory.h
        src/Stringify.h
        src/Crafting.h
        src/Slime.cpp
        src/Slime.h
        src/ChestUI.cpp
        src/ChestUI.h
        src/EndBoss.cpp
        src/EndBoss.h
        src/DroppedItem.h
        src/Functional.h
        src/Functional.h
        src/icon.rc
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

# Link with raylib
if (WIN32)
    if(MSVC)
        # MSVC linker flags (no need for -mwindows)
        target_link_libraries(${PROJECT_NAME} raylib gdi32 winmm)
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    else()
        # MinGW-specific flags
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
        target_link_libraries(${PROJECT_NAME} raylib)
    endif()
else()
    target_link_libraries(${PROJECT_NAME} raylib)
endif()

# Ensure the build works on macOS
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCE_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)

