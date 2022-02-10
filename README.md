
# CPSC5042 Project Group #2 (Calculator)


## Makefile, Compilation, and Execution Instructions:

**This document includes instructions for compiling the project source code with either the accompanying makefile or for compiling manually from the command line. We have also included a CMakeLists.txt for use with cmake or your choice of IDE.**

## Compilation Instructions:

The source code for the RPC server is in the server directory. The source code for the RPC client is in the client directory.

### Compiling using Makefile (Recommended Method)

**To compile all the source code and create the executables for both the server and the client, type [make]{.underline} in the project root directory**. This will create the startClient and startServer binaries, both in the project root.

### Compiling using command line (Alternative Method)

Alternatively, you can compile the binaries manually. To do so, execute the following commands from the project root:

    g++ -std=c++11 server/main.cpp server/RPCServer.cpp -o startServer

    g++ -std=c++11 client/ClientCS.cpp -o startClient

Execution instructions:

On CS1, we recommended that the Client and Server be run in separate console to be able to monitor the output of both applications simultaneously.

The server executable takes two command line arguments - the IP address and port number, separated by a space. To run the server on CS1 console, "cd" into the root directory of the project, then type the following at the command prompt and hit return (replace IP_ADDRESS and PORT with your IP address and port):

    ./startServer IP_ADDRESS PORT

This will start the server, which will run until you kill it with 'Ctrl + c'.

To run the client, start another console session on CS1, "cd" to the project root, and type the following (replace IP_ADDRESS and PORT with your IP address and port):

    ./startClient IP_ADDRESS PORT

After executing the command above, the client will now ask the user to enter the username and password to authenticate the connection.

