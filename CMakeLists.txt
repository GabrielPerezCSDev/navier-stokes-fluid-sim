cmake_minimum_required(VERSION 3.10)
project(fluid_sim)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})

add_executable(fluid_sim
    src/main.cpp
    src/visual/SDLRenderer.cpp
    src/core/MACGrid.cpp
    src/core/MACCell.cpp
    src/core/Solver.cpp
)

target_link_libraries(fluid_sim ${SDL2_LIBRARIES})