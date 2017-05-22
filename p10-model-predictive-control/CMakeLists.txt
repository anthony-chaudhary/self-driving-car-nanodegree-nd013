project(MPC)

cmake_minimum_required (VERSION 3.5)

add_definitions()

set(CXX_FLAGS "-W1")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")


include_directories(LIBRARY_ 
	src/COIN-OR-1.8.0-win32-msvc12/COIN-OR/win32-msvc12/include
	src/COIN-OR-1.8.0-win32-msvc12/COIN-OR/win32-msvc12/bin
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    
    set(sources src/MPC.cpp src/main.cpp
	src/COIN-OR-1.8.0-win32-msvc12/COIN-OR/win32-msvc12/include/cppad/cppad.hpp
	src/COIN-OR-1.8.0-win32-msvc12/COIN-OR/win32-msvc12/include/cppad/base_require.hpp 
	src/COIN-OR-1.8.0-win32-msvc12/COIN-OR/win32-msvc12/include/cppad/ipopt/solve.hpp
	src/uWS/Extensions.cpp src/uWS/Group.cpp src/uWS/WebSocketImpl.cpp src/uWS/Networking.cpp src/uWS/Hub.cpp src/uWS/Node.cpp src/uWS/WebSocket.cpp src/uWS/HTTPSocket.cpp src/uWS/Socket.cpp src/uWS/uUV.cpp)
    set_source_files_properties(${sources} PROPERTIES COMPILE_FLAGS "-D_USE_MATH_DEFINES")

else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(sources src/PID.cpp src/main.cpp)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


link_directories(LIBRARY_ src/COIN-OR-1.8.0-win32-msvc12/COIN-OR/win32-msvc12/lib)


# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)
# include_directories(src/Eigen-3.3)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

include_directories(/usr/local/opt/openssl/include)
link_directories(/usr/local/opt/openssl/lib)
link_directories(/usr/local/Cellar/libuv/1.11.0/lib)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_executable(mpc ${sources})

# target_link_libraries(mpc ipopt z ssl uv uWS)
if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_link_libraries(mpc z ssl uv uWS)
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
