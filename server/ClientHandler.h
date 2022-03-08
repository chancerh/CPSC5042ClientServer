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
#include "Authenticator.h"

using namespace std;

class ClientHandler{
public:
    /**
     * Constructor - authenticate user log-in
     * @param socket A socket number that created in RPCServer files
     * @param fileName A .csv file that contain the username and password
     */
    ClientHandler(int socket, const string &fileName);

    /**
     * Destructor - deallocates resources used when the instance is no longer needed
     */
    ~ClientHandler();

    /**
    * Processes incoming RPCs by parsing the input arguments and calling the
    * appropriate functions and create a threads
    * @param g_contextLock A mutex that lock global variable
    * @param g_screenLock A mutex that lock the printing statement in server side
    * @param g_globalContext A struck that track all the rpcCount, activeConnection,
    *                        maxConnection, and totalConnection
    * @return Always returns true
    */
    bool ProcessRPC(pthread_mutex_t *g_contextLock,
                    pthread_mutex_t *g_screenLock,
                    GlobalContext *g_globalContext);

private:
    int m_socket; //socket number
    bool m_authenticated; //flag to track if client provided correct credentials and is logged in
    Authenticator m_authenticator;

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
     * @param g_screenLock A mutex that lock the printing statement in server side
     * @return A boolean indicating if the input credentials are correct
     */
    bool ProcessConnectRPC(vector<std::string>& arrayTokens, pthread_mutex_t *g_screenLock);

    /**
     * The ProcessesCal() is a different RPC of the calculator functions. This function will create a calculator object
     * and call each calculator function according to incoming RPC
     * @param arrayTokens A vector containing the tokens of incoming RPC.
     * @param g_screenLock A mutex that lock the printing statement in server side
     * @return Always returns true
     */
    bool ProcessCal(vector<std::string> &arrayTokens, pthread_mutex_t *g_screenLock);

    /**
     * The processDisconnectRPC() disconnects the client connection from the server
     * @param g_screenLock A mutex that lock the printing statement in server side
     * @return Always returns true
     */
    bool ProcessDisconnectRPC(pthread_mutex_t *g_screenLock);

    /**
     * The ParseTokens() function processes an input char array buffer containing the RPC parameters, and parses it
     * into a string vector. The parsing of the buffer relies on ';' as the delimiter
     * @param buffer Input char array buffer to be parsed, with ';' as delimeter between parameters
     * @param a A string vector containing the parsed RPC parameters
     */
    void ParseTokens(char* buffer, std::vector<std::string>& a);

    /**
     * The sendBuffer() is a helper function to send buffers back to client
     * @param szBuffer A char buffer that contain result from ProcessCal() function
     * @param g_screenLock A mutex that lock the printing statement in server side
     */
    void sendBuffer(char *szBuffer, pthread_mutex_t *g_screenLock) const;

    /**
     * The printServerStats() will print out the status of server
     * @param g_globalContext A struck that track all the rpcCount, activeConnection,
     *                        maxConnection, and totalConnection
     * @param phase A string that pass in status when "Create" or "Destroy" thread
     */
    void printServerStats(const GlobalContext *g_globalContext,
                          const string &phase) const;
};


#endif //CPSC5042CLIENTSERVER_CLIENTHANDLER_H
