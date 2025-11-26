cmake_minimum_required(VERSION 3.16)
project(assignment_1)

set(CMAKE_CXX_STANDARD 20)

# Raylib Homebrew path
set(RAYLIB_PATH "/opt/homebrew/Cellar/raylib/5.5")

# Include header files
include_directories(${RAYLIB_PATH}/include)

# Link to the library directory
link_directories(${RAYLIB_PATH}/lib)

add_executable(assignment_1
        main.cpp
        chess_analysis_program.cpp
        chess_game.h
        chess_game.cpp
)

# Link libraries
target_link_libraries(assignment_1
        raylib
        m
        pthread
        dl
)
