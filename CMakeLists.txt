cmake_minimum_required(VERSION 3.0)

project("cgol")

add_executable(cgol cgol.c)

find_package(SDL2 REQUIRED)

target_link_libraries(cgol SDL2::SDL2 SDL2::SDL2main)