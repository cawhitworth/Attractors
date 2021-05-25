cmake_minimum_required(VERSION 3.8)
project(attractors)
set(CMAKE_CXX_STANDARD 11)

add_executable(attractors
    Attractors.cpp
    Bitmap.cpp
    Functions.cpp
    lodepng.cpp
    Options.cpp
    Palettes.cpp
    )