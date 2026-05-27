cmake_minimum_required(VERSION 3.15..4.2.3)

project(
    3D 
    VERSION 0.1.0
    DESCRIPTION "3D graphics demo"
    LANGUAGES C
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(BINARY_NAME "bin")
set(INCLUDE_DIR "include")
set(RAYLIB "raylib")

# Include headers by typing `foo.h` instead of `../include/foo.h`.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    ${RAYLIB}
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(${RAYLIB})

# `CONFIGURE_DEPENDS` tells the build tool to re-run CMake if the glob result changes.
# New files will get picked up automatically on the next build.
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.c)

add_executable(
    ${BINARY_NAME}
    ${SRC_FILES}
)

target_compile_options(
    ${BINARY_NAME}
    PRIVATE
    -Wall
    -Werror
    -std=c99
    $<$<CONFIG:Debug>:-ggdb>
)

target_include_directories(${BINARY_NAME} PRIVATE ${INCLUDE_DIR})

target_link_libraries(
    ${BINARY_NAME}
    PRIVATE
    m
    ${RAYLIB}
)

