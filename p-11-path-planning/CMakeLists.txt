project(Path_Planning)

cmake_minimum_required (VERSION 3.5)

add_definitions(-std=gnu++14)

set(CXX_FLAGS "-W1")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    
    set_source_files_properties(${sources} PROPERTIES COMPILE_FLAGS "-D_USE_MATH_DEFINES")
	set(sources src/main.cpp src/path.cpp src/classifier.cpp src/behavior_planner.cpp src/uWS/Extensions.cpp src/uWS/Group.cpp src/uWS/WebSocketImpl.cpp src/uWS/Networking.cpp src/uWS/Hub.cpp src/uWS/Node.cpp src/uWS/WebSocket.cpp src/uWS/HTTPSocket.cpp src/uWS/Socket.cpp src/uWS/uUV.cpp)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

	include_directories(/usr/local/include)
	include_directories(/usr/local/opt/openssl/include)
	link_directories(/usr/local/lib)
	link_directories(/usr/local/opt/openssl/lib)
	link_directories(/usr/local/Cellar/libuv/1.11.0/lib)
	
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 


if (UNIX)
set(sources src/main.cpp src/path.cpp src/classifier.cpp src/behavior_planner.cpp)

endif (UNIX)

add_executable(path_planning ${sources})

if (UNIX)

target_link_libraries(path_planning z ssl uv uWS)
endif (UNIX)