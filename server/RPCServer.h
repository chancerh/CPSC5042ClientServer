//Author  : Group#2
//Date    : 02/07/2022
//Version : 2.0
//Filename: RPCServer.h

#pragma once
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
     * @return True if the socket is able to be created, setup,
     *          and bound, and if the server is able to listen. False otherwise
     *
     */
    bool StartServer();

    /**
     * Accepts a new connection from a client by listening on its address.
     * @return Always TRUE
     */
    bool ListenForClient();

    /**
     * Processes incoming RPCs by parsing the input arguments and calling the appropriate functions
     * @return Always TRUE
     */
    //bool ProcessRPC();


    //create a global variable
    static struct _GlobalContext {
        int g_rpcCount;
        int g_activeConnection;
        int g_totalConnection;
        int g_threadCounts;
    } GlobalContext;

// Create mutex lock
    static pthread_mutex_t lock;



private:
    //Member elements
    int m_rpcCount;
    int m_server_fd; //server fault domain
    int m_socket; //socket number
    char* m_serverIP; //server IP
    int m_port; //server port connection
    struct sockaddr_in m_address;
    //bool m_authenticated; //flag to track if client provided correct credentials and is logged in
    //unordered_map<string,string> m_users; //map storing all username and password pairs
    vector<pthread_t> m_threadContainer;


    //SUPPORTED RPCs
    const string CONNECT = "connect",
                  DISCONNECT = "disconnect",
                  CALC_EXPR = "calculateExpression";

    //RPC Status
    const string SUCCESS = "0;",
                  GENERAL_FAIL = "-1;";

//    /**
//     * The ProcessConnectRPC() function process a connect RPC by validating the input credentials. If input credentials
//     * are correct, then m_authenticated is flipped to true
//     * @param arrayTokens A vector containing the tokens of incoming RPC. arrayTokens[1] contains username,
//     *                      arrayTokens[2] contains the password
//     * @return A boolean indicating if the input credentials are correct
//     */
//    bool ProcessConnectRPC(vector<std::string>& arrayTokens);
//
//    /**
//     * The processDisconnectRPC() disconnects the client connection from the server
//     * @return Always returns true
//     */
//    bool ProcessDisconnectRPC();
//
//    /**
//    * The ProcessCalcExp() function processes a request to calculate an
//    * algebraic expression, calculates the expression and returns the result
//    * @param arrayTokens A vector containing the tokens of incoming RPC.
//    *                      arrayTokens[1] contains the mathematical expression
//    * @return A boolean indicating if the input credentials are correct
//    */
//    void ProcessCalcExp(vector<std::string>& arrayTokens);
//
//    /**
//     * The ParseTokens() function processes an input char array buffer containing the RPC parameters, and parses it
//     * into a string vector. The parsing of the buffer relies on ';' as the delimiter
//     * @param buffer Input char array buffer to be parsed, with ';' as delimeter between parameters
//     * @param a A string vector containing the parsed RPC parameters
//     */
//    void ParseTokens(char* buffer, std::vector<std::string>& a);


//    string ProcessStatSummary(vector<std::string> &arrayTokens);

//    //Helper function to send buffers back to client
//    void sendBuffer(char *szBuffer) const;

    //void* startThread(void* input);

};

