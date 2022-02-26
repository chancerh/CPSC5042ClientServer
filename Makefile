# Set the g++ compiler flags to compile to the C++11 standard and to show all
# warnings. 
CXXFLAGS = -std=c++11 -g

# Default behavior when typing "make" at the command line compiles both server
# and client executables.
.PHONY : all
all : startServer startClient

# Compile the client executable
startClient : client/ClientCS.cpp
	$(CXX) $(CXXFLAGS) $< -o startClient

# Link the compiled main and RPCServe code to create the server executable

startServer : server/main.cpp server/RPCServer.cpp server/Calculator.cpp server/RPCServer.cpp
	$(CXX) $(CXXFLAGS) $^ -o startServer

# startServer : server/Calculator.h server/Calculator.cpp  server/main.cpp server/RPCServer.h server/RPCServer.cpp
#	$(CXX) $(CXXFLAGS) server/Calculator.cpp server/main.cpp server/RPCServer.cpp -o startServer
