//Author  : Group#2
//Date    : 02/07/2022
//Version : 2.0
//Filename: ClientHandler.h

#ifndef CPSC5042CLIENTSERVER_CLIENTHANDLER_H
#define CPSC5042CLIENTSERVER_CLIENTHANDLER_H

//#include "RPCServer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "pthread.h"
#include "GlobalContext.h"

using namespace std;

class ClientHandler{
public:
    ClientHandler(int socket);
    ~ClientHandler();

    /**
    * Processes incoming RPCs by parsing the input arguments and calling the
    * appropriate functions
    *
    * @return Always TRUE
    */
    bool ProcessRPC(pthread_mutex_t *g_lock, GlobalContext *g_globalContext);

private:
    int m_socket; //socket number
    bool m_authenticated; //flag to track if client provided correct credentials and is logged in
    unordered_map<string,string> m_users; //map storing all username and
    // password pairs

    //SUPPORTED RPCs
    //Conversion parted added and fixed by Jusmin 3/2/22
    const string CONNECT = "connect",
                  DISCONNECT = "disconnect",
                  CALC_EXPR = "calculateExpression",
                  CALC_STAT = "calculateStats",
                  CALC_CONV = "conversion";


    //RPC Status
    const string SUCCESS = "0;",
            GENERAL_FAIL = "-1;";

    const string INVALID_ARG = "Invalid argument entered.";


    /**
     * The ProcessConnectRPC() function process a connect RPC by validating the input credentials. If input credentials
     * are correct, then m_authenticated is flipped to true
     * @param arrayTokens A vector containing the tokens of incoming RPC. arrayTokens[1] contains username,
     *                      arrayTokens[2] contains the password
     * @return A boolean indicating if the input credentials are correct
     */
    bool ProcessConnectRPC(vector<std::string>& arrayTokens);

    /**
     * Processes the different RPC of the calculator functions
     * @param arrayTokens A vector containing the tokens of incoming RPC.
     */
    bool ProcessCal(vector<std::string> &arrayTokens);

    /**
     * The processDisconnectRPC() disconnects the client connection from the server
     * @return Always returns true
     */
    bool ProcessDisconnectRPC();

    /**
     * The ParseTokens() function processes an input char array buffer containing the RPC parameters, and parses it
     * into a string vector. The parsing of the buffer relies on ';' as the delimiter
     * @param buffer Input char array buffer to be parsed, with ';' as delimeter between parameters
     * @param a A string vector containing the parsed RPC parameters
     */
    void ParseTokens(char* buffer, std::vector<std::string>& a);

    //Helper function to send buffers back to client
    void sendBuffer(char *szBuffer) const;

};


#endif //CPSC5042CLIENTSERVER_CLIENTHANDLER_H
