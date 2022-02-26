# Set the g++ compiler flags to compile to the C++11 standard and to show all
# warnings. 
CXXFLAGS = -std=c++11

# Default behavior when typing "make" at the command line compiles both server
# and client executables.
.PHONY : all
all : startServer startClient

# Compile the client executable
startClient : client/ClientCS.cpp
	$(CXX) $(CXXFLAGS) $< -o startClient

# Link the compiled main and RPCServe code to create the server executable
startServer : server/main.cpp server/RPCServer.cpp server/Calculator.cpp
	$(CXX) $(CXXFLAGS) $^ -o startServer
