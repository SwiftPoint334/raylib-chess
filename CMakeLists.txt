cmake_minimum_required(VERSION 3.16)
project(assignment_1)

set(CMAKE_CXX_STANDARD 20)

# Ensure Raylib is found via Homebrew
if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/raylib")
endif()

find_package(raylib CONFIG REQUIRED)

add_executable(assignment_1
    main.cpp
    chess_analysis_program.cpp
    chess_game.h
    chess_game.cpp
)

target_link_libraries(assignment_1 PRIVATE raylib)
