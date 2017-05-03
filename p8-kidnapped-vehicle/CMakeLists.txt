cmake_minimum_required(VERSION 3.5)
project(PARTICLE_FILTER)


# Build the particle filter project and solution.
# Use C++11
set(SRCS src/main.cpp src/particle_filter.cpp)
set_source_files_properties(${SRCS})
add_definitions(-Wall)

# Create the executable
add_executable(particle_filter ${SRCS})

# Use C++11
#if [ ! -f ./src/particle_filter_sol.cpp]; then
#	echo "No solution file."
#else
#	echo "Solution file exists"
#fi

add_custom_command(TARGET particle_filter 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:particle_filter> C:/Users/Anthony/Documents/GitHub/self-driving-car-nanodegree-nd013/p8-kidnapped-vehicle/build)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/particle_filter_sol.cpp")
	set(SRCS src/main.cpp src/particle_filter_sol.cpp)
	set_source_files_properties(${SRCS} PROPERTIES COMPILE_FLAGS -std=c++0x)

	# Create the executable
	add_executable(particle_filter_solution ${SRCS})
endif()









