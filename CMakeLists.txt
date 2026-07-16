
cmake_minimum_required(VERSION 3.15)

project(LetBuildAbot LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(vendors/raylib)

add_executable(LetBuildAbot 
    src/pch.h
    src/main.cpp
    src/helper.cpp
    src/Player.h
    src/Player.cpp
    src/Runner.h
    src/Runner.cpp
    src/Instruction/Instruction.h
    src/Instruction/MoveInstruction.h
    src/Instruction/RotateInstruction.h
    src/Instruction/MoveInstruction.cpp
    src/Instruction/RotateInstruction.cpp
)

target_link_libraries(LetBuildAbot PRIVATE raylib)
target_compile_definitions(LetBuildAbot PRIVATE RAYMATH_IMPLEMENTATION)
target_precompile_headers(
    LetBuildAbot PRIVATE
    "src/pch.h"
)
