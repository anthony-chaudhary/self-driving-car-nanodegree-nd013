project(Path_Planning)

cmake_minimum_required (VERSION 3.5)

add_definitions(-std=c++11)

set(CXX_FLAGS "-W1")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")


if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    
    set_source_files_properties(${sources} PROPERTIES COMPILE_FLAGS "-D_USE_MATH_DEFINES")
	set(sources src/main.cpp src/path.cpp src/classifier.cpp src/behavior_planner.cpp src/uWS/Extensions.cpp src/uWS/Group.cpp src/uWS/WebSocketImpl.cpp src/uWS/Networking.cpp src/uWS/Hub.cpp src/uWS/Node.cpp src/uWS/WebSocket.cpp src/uWS/HTTPSocket.cpp src/uWS/Socket.cpp src/uWS/uUV.cpp)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")



if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

set(sources src/main.cpp src/path.cpp src/classifier.cpp src/behavior_planner.h)

include_directories(/usr/local/include)
include_directories(/usr/local/opt/openssl/include)
link_directories(/usr/local/lib)
link_directories(/usr/local/opt/openssl/lib)
link_directories(/usr/local/Cellar/libuv/1.11.0/lib)


endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 


add_executable(path_planning ${sources})

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(path_planning z ssl uv uWS)
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")


