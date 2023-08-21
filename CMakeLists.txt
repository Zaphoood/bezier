cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Bezier)

add_executable(bezier
    src/main.cpp
    src/curve.cpp
    src/segment.cpp
    src/point.cpp
    src/vector2.cpp
    src/visuals.cpp
    src/window.cpp
)

target_link_libraries(bezier PUBLIC SDL2 SDL2_ttf)
target_include_directories(bezier PRIVATE
    include
    /usr/local/include/SDL3
    /usr/include/SDL2
)
