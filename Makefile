# Set the g++ compiler flags to compile to the C++11 standard and to show all
# warnings. 
CXXFLAGS = -std=c++11 -W -Wall

# Default behavior when typing "make" at the command line compiles both server
# and client executables.
.PHONY : all
all : startServer startClient

# Compile the client executable
startClient : client/ClientCS.cpp
	$(CXX) $(CXXFLAGS) $< -o startClient

# Link the compiled main and RPCServe code to create the server executable
startServer : server/main.o server/RPCServer.o
	$(CXX) $(CXXFLAGS) $^ -o startServer

# Compile the server files required for the startServe executable
server/%.o : server/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

