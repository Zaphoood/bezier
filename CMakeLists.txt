cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the project name
project(Multibezier)

add_executable(multibez
    src/main.cpp
    src/curve.cpp
    src/segment.cpp
    src/point.cpp
    src/vector2.cpp
    src/visuals.cpp
    src/window.cpp
)

target_link_libraries(multibez PUBLIC SDL2 SDL2_ttf)
target_include_directories(multibez PRIVATE
    include
    /usr/local/include/SDL3
    /usr/include/SDL2
)
