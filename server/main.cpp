//Author  : Group#2
//Date    : 02/07/2022
//Version : 1.0
//Filename: main.cpp

/* This file contains the 'main' function. Program execution begins and ends here.
 * This is a simple example of a CPSC5042 Server that will listen to a CPSC5042 Client
 *
 * Milestone 1 will have the server handle one client at a time. The server will:
 * - Wait for connection from client
 * - Process Connect RPC
 * - Once connected, process all RPC requests until the client does a Disconnect RPC
 * - This initial server will handle 2 RPC's:
 *      - Connect
 *      - Disconnect
*/

#include <cstdio>
#include <iostream>
#include "RPCServer.h"


/**
 * The main() function starts the server applications, waits for client connection, then process RPCs from the client.
 *
 * This is an endless server, thus will not exit unless it hits an error, or terminated from the OS
 *
 * @param argc An int containing the number input arguments
 * @param argv A char array containing the command line arguments. Expecting IP Address, port number
 *              (e.g. 127.0.0.1 8080)
 * @return An int value with the exit status
 */
int main(int argc, char const* argv[])
{
    cout << "************************************************" << endl;
    cout << "*   Welcome to the Group 2 Server Application  *" << endl;
    cout << "************************************************" << endl;

    //check if user entered correct # of Command Line args for IP and Port
    if (argc < 3)
    {
        //If insufficient number of args, print error and exit program.
        cout << "\nMissing IP Address or Port number.\n";
        cout << "Exiting Server Application...\n";
        return -1;
    }

    //Extract serverIP and port numbers from command line arguments
    const char* serverIP = argv[1];
    int port = atoi(argv[2]);

    //Declare a bool flag to track the status of the system
    bool statusOk = true;

    //Declare a server object variable to handle the server operations
    RPCServer* serverObj = new RPCServer(serverIP, port);

    //Get server Status
    statusOk = serverObj->StartServer();

    //Print Server Status
    if (statusOk)
    {
        cout << "\nServer is now running\n" << endl;
    }
    else
    {
        cout << "\nServer failed to start\n" << endl;
    }

    //While loop listening to client and processing the incoming RPC calls
    while (statusOk)
    {
        printf("\nWaiting...\n");
        statusOk = serverObj->ListenForClient();
        //Start a new thread, that points to ProcessRPC. Inside ProcessRPC,
        // we will create a new Calc object
        statusOk = serverObj->ProcessRPC(); // Launch thread
    }

    //delete the serverObj and free up its memory
    delete serverObj;
    serverObj = nullptr;

    //Return success to OS
    return 0;
}