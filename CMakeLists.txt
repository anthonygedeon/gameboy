# setting the lowest version of cmake that this project will support.
cmake_minimum_required(VERSION 3.12)

# declaring the project
# naming a baby
project(bmo VERSION 0.1)

include(${CMAKE_SOURCE_DIR}/cmake/FindSDL2.cmake)

# find the SDL to use
# EXACT - find only 2.0.17
# REQ. - baby dies
find_package(SDL2 2.0.17 EXACT REQUIRED)

# SDL2 libs/headers
message("found sdl2 headers at " ${SDL2_INCLUDE_DIR})
message("found sdl2 libs at " ${SDL2_LIBRARY})

# src code babyyyyy
# find all *c(pp) file and put in the variable called BMO_SOURCES
file(GLOB_RECURSE BMO_SOURCES "src/*.cpp" "src/*.c")

# setup all includes
include_directories(
    ${SDL2_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/include/imgui/
)

# making the binary that the computer will exec.
add_executable(bmo ${BMO_SOURCES})

# telling cmake what libs to link against your project
target_link_libraries(bmo ${SDL2_LIBRARY})