cmake_minimum_required(VERSION 3.15)
project(2D_Adventures)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(INPUT_FILE_PATH ${CMAKE_SOURCE_DIR}/res/icons.png)
set(OUTPUT_FILE_PATH ${CMAKE_SOURCE_DIR}/src/icons.h)
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

# Embed PNG file into a header
find_package(Python3 REQUIRED)

# Run the Python script as part of the build configuration
execute_process(
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/generate_headers.py
        ${INPUT_FILE_PATH} ${OUTPUT_FILE_PATH}
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error
)

if(result)
    message(FATAL_ERROR "Python script failed with error: ${error}")
else()
    message(STATUS "Python script output: ${output}")
endif()
# Add your executable
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Player.cpp
        src/Player.h
        src/Enemy.cpp
        src/Enemy.h
)

# Ensure the headers are generated before building the executable
add_custom_target(GenerateHeaders DEPENDS ${CMAKE_SOURCE_DIR}/icons.h)

# Link with raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Ensure the build works on macOS
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

