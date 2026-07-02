
cmake_minimum_required(VERSION 3.15)

project(LetBuildAbot LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(vendors/raylib)

add_executable(LetBuildAbot 
    src/main.cpp
    src/helper.cpp
)

target_link_libraries(LetBuildAbot PRIVATE raylib)
target_compile_definitions(LetBuildAbot PRIVATE RAYMATH_IMPLEMENTATION)
target_precompile_headers(
    LetBuildAbot PRIVATE
    <stdint.h>
    <raylib.h>
    <raymath.h>
    <string>
)
