
CXXFLAGS = -std=c++11 -W -Wall

.PHONY : all

all : startServer startClient


startClient : client/ClientCS.cpp
	$(CXX) $(CXXFLAGS) $< -o startClient

startServer : server/main.o server/RPCServer.o
	$(CXX) $(CXXFLAGS) $^ -o startServer

server/%.o : server/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

