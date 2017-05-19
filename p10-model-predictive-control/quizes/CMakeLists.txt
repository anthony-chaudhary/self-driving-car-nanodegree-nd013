project(mpc-quiz)

cmake_minimum_required (VERSION 3.5)

add_definitions(-W3)

set(sources fit_polynomials.cpp)

add_executable(mpc-quiz ${sources})

add_executable(mpc-quiz-debug ${sources})
add_custom_command(TARGET mpc-quiz-debug 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mpc-quiz-debug> C:/Users/Anthony/Documents/GitHub/self-driving-car-nanodegree-nd013/p10-model-predictive-control)