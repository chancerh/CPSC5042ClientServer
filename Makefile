# Set the g++ compiler flags to compile to the C++17 standard and to show all
# warnings. 
CXXFLAGS = -std=c++17 -g -lpthread -pthread

# Default behavior when typing "make" at the command line compiles both server
# and client executables.
.PHONY : all
all : startServer startClient

# Compile the client executable
startClient : client/ClientCS.cpp
	$(CXX) $(CXXFLAGS) $< -o startClient

# Link the compiled main and RPCServe code to create the server executable
# Start with class names
SERVER_CLASSES = RPCServer Calculator RPCServer ClientHandler Authenticator

# Create server/*.h
SERVER_H = $(addsuffix .h, $(addprefix server/, $(SERVER_CLASSES)))
# create server/*.cpp for classes but NOT main.cpp driver
SERVER_CPP = $(addsuffix .cpp, $(addprefix server/, $(SERVER_CLASSES)))
#
startServer : $(SERVER_H) $(SERVER_CPP) server/main.cpp
	$(CXX) $(CXXFLAGS) $(SERVER_CPP) server/main.cpp -o startServer


