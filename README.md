# CPSC5042ClientServerGroupTwo

This repository contains the code for the Group 2 CPSC5042 Client/Server RPC project. The source code for the RPC server is in the server folder. The source code for the RPC Client is in the client folder. You can either use the Makefile to compile and create binaries from the command line, or use CMakeLists.txt and cmake. This README contains instructions for compiling with Make and running from the command line.

To compile all of the source code and create the executables for both the server and the client, type "make" (without the quotes) in the project root directory. This will create the startClient and startServer executables, both in the project root.

The server executable takes two command line arguments - the IP and port, separated by space. To run the server, type the following at the command prompt and hit return (replace IP_ADDRESS and PORT with your IP address and port):

    ./startServer IP_ADDRESS PORT

This will start the RPC server, which will run until you kill it with CTRL+C.

To run the client, start another session on the server running the client, change directory to the project root, and type the following:

    ./startClient IP_ADDRESS PORT

The client will connect to the server, prompt for a username and password. If the login is successful, the client will sleep for 10 seconds, then disconnect. If the login is unsuccessful, the user will be prompted for credentials until correct ones are entered, or the client is stopped with CTRL+C.
