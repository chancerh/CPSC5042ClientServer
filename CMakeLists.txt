cmake_minimum_required(VERSION 3.20)
project(CPSC5042ClientServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -W -Wall")

include_directories(server)
include_directories(client)

find_package( Threads )

add_executable(startClient client/ClientCS.cpp)

add_executable(startServer server/main.cpp server/RPCServer.h server/RPCServer.cpp server/Calculator.h server/Calculator.cpp server/ClientHandler.h server/ClientHandler.cpp)
add_executable(startClientDriver client/ClientTestDriver.cpp)
target_link_libraries( startServer ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries( startClientDriver ${CMAKE_THREAD_LIBS_INIT} )