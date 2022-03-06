//Author  : Group#2
//Date    : 02/07/2022
//Version : 2.0
//Filename: RPCServer.h

#ifndef CPSC5042CLIENTSERVER_RPCSERVER_H
#define CPSC5042CLIENTSERVER_RPCSERVER_H

#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <string>
#include "Calculator.h"
#include "pthread.h"
#include "GlobalContext.h"

using namespace std;

class RPCServer
{
public:
    /**
     * Constructor - create a new RPCServer instance
     * @param serverIP The IP address of the server
     * @param port The port number the server should use
     */
    RPCServer(const char *serverIP, int port);

    /**
     * Destructor - deallocates resources used when the instance is no longer needed
     */
    ~RPCServer();

    /**
     * Sets up and configures and binds the socket
     *
     * @return True if the socket is able to be created, setup,
     *          and bound, and if the server is able to listen. False otherwise
     *
     */
    bool StartServer();

    /**
     * Accepts a new connection from a client by listening on its address,
     * and starts a separate thread for each client to process its incoming RPCs
     *
     * @return Always TRUE
     */
    bool ListenForClient();


private:
    //Member elements
    int m_server_fd; //server fault domain
    int m_socket; //socket number
    char *m_serverIP; //server IP
    int m_port; //server port connection
    struct sockaddr_in m_address;

    const string MUTEX_INIT_FAIL = "Failed to initialize mutex.";
};

#endif //CPSC5042CLIENTSERVER_RPCSERVER_H
