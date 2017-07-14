project(behave)

cmake_minimum_required (VERSION 3.5)

# -g allows for gdb debugging
# turn on -03 for best performance
add_definitions(-std=c++11 -O3)

set(CXX_FLAGS "-w3")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set(sources road.cpp main.cpp vehicle.cpp)

add_executable(bp ${sources})


