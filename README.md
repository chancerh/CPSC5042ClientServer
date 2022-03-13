# CPSC5042 Project Group #2 (Calculator)

## Makefile, Compilation, and Execution Instructions:

**This document includes instructions for compiling the project source code with either the accompanying makefile or for
compiling manually from the command line. We have also included a CMakeLists.txt for use with cmake or your choice of
IDE.**

## Obtaining the source code:

The source code for this program can be retrieved from github with the following command:

    git clone https://github.com/chancerh/CPSC5042ClientServer.git

You can now cd into the root directory of the project:

    cd CPSC5042ClientServer

## Compilation Instructions:

The source code for the RPC server is in the server directory. The source code for the RPC client and the client test
driver are in the client directory.

### Compilation with GNU Make (Recommended Method)

**To compile all the source code and create the executables for both the server and the client, type *make* followed by
a return in the project root directory**. This will create the startClient, and startServer, and clientTestDriver
binaries, all in the project root.

### Compiling using command line (Alternative Method)

Alternatively, you can compile the binaries manually. To do so, execute the following commands from the project root:

    g++ -std=c++17 -lpthread -pthread server/*.cpp -o startServerg++ -std=c++11 server/main.cpp server/RPCServer.cpp -o startServer
    g++ -std=c++17 -lpthread -pthread client/ClientCS.cpp -o startClientg++ -std=c++11 client/ClientCS.cpp -o startClient
    g++ -std=c++17 -lpthread -pthread client/ClientTestDriver.cpp -o clientTestDriver

Execution instructions:

We recommended that the Client and Server be run in separate console to be able to monitor the output of both
applications simultaneously.

The server executable takes two command line arguments - the IP address and port number, separated by a space. To run
the server on CS1 console, "cd" into the root directory of the project, then type the following at the command prompt
and hit return (replace IP_ADDRESS and PORT with your IP address and port):

    ./startServer IP_ADDRESS PORT

This will start the server, which will run until you kill it with 'Ctrl + c'. During execution, the server will print
output showing the RPC that the server received, the response it sent back, as well as a readout of connection
statistics.

To run the client, start another console session on CS1, "cd" to the project root, and type the following (replace
IP_ADDRESS and PORT with your IP address and port):

    ./startClient IP_ADDRESS PORT

After executing the command above, the client will now ask the user to enter the username and password to authenticate
the connection.

This project also includes the clientTestDriver program, which uses multithreading to make multiple client connections
to a running server. This tests its ability to properly handle simultaneous connections in a stable and deterministic
manner. This can be run with the following command:

    ./startClientDriver IP_ADDRESS PORT NUMBEER_OF_THREADS

Due to the volume output that this causes the server to produce, it may be useful to send it to a file rather than the
terminal. You can do so by redirecting the output to a file when starting the server:

    ./startServer IP_ADDRESS PORT > FILENAME.txt

Usernames and passwords are stored in a CSV file called “credentials.csv”. This file is used by the server application
and must be in the same directory as the startServer binary, or in the working directory specified in your IDE. Both
username and password are case-sensitive.

*Note: When Client application is running on CS1 using an actual terminal (not IDE), the passwords will be masked while
typing (i.e., nothing will be printed on the screen)* 