cmake_minimum_required(VERSION 3.0)

project("cgol")

add_executable(cgol cgol.c)

find_package(SDL2 REQUIRED)

if (MSVC)
    set_target_properties(cgol PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
endif(MSVC)

target_link_libraries(cgol SDL2::SDL2 SDL2::SDL2main)
