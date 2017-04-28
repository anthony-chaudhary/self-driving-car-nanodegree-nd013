project(UnscentedKF)

cmake_minimum_required (VERSION 3.5)

add_definitions(-Wall)

set(sources
   src/ukf.cpp
   src/main.cpp
   src/tools.cpp)

add_executable(UnscentedKF ${sources})

add_executable(UnscentedKFdebug ${sources})
add_custom_command(TARGET UnscentedKFdebug 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:UnscentedKFdebug> C:/Users/Anthony/Documents/GitHub/self-driving-car-nanodegree-nd013/p7-Unscented-Kalman-Filter/buildsx86-Debug)