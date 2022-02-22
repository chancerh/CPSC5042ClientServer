cmake_minimum_required(VERSION 3.20)
project(CPSC5042ClientServer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wall")

include_directories(server)
include_directories(client)

add_executable(startClient client/ClientCS.cpp)

add_executable(startServer server/main.cpp server/RPCServer.cpp server/RPCServer.h server/Calculator.h server/Calculator.cpp)
