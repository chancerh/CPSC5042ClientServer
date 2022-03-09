//Author  : Group#2
//Date    : 02/07/2022
//Version : 2.0
//Filename: ClientHandler.cpp

#include "ClientHandler.h"
#include <regex>
#include "pthread.h"
#include <unistd.h>
#include <sys/socket.h>
#include "Calculator.h"

using namespace std;

/**
 * Constructor
 */
ClientHandler::ClientHandler(int socket, const string& filename) :
        m_authenticator(filename)
{
    m_socket = socket;
    m_authenticated = false;
    //call Authenticator to authenticate username and password
    m_authenticator = Authenticator(filename);
    localContext = new LocalContext();

}
/**
 * Destructor
 */
ClientHandler::~ClientHandler()
{
    delete localContext;
    localContext = nullptr;
}

/**
 * ProcessRPC will examine buffer and will essentially control
 */
bool ClientHandler::ProcessRPC(pthread_mutex_t *g_contextLock,
                               pthread_mutex_t *g_screenLock,
                               GlobalContext *g_globalContext)
{
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE] = { 0 };
    std::vector<std::string> arrayTokens;
    int valread = 0;
    bool bConnected = false;
    bool bStatusOk = true;
    const int RPCTOKEN = 0;
    bool bContinue = true;

    //Update global context variables (num of connections, ...)
    //get mutex
    pthread_mutex_lock(g_contextLock);
    pthread_mutex_lock(g_screenLock);

    //update number of active connections
    g_globalContext->g_activeConnection += 1;

    //update max concurrent connections
    if (g_globalContext->g_activeConnection > g_globalContext->g_maxConnection)
       g_globalContext->g_maxConnection = g_globalContext->g_activeConnection;

    //update number of overall connections
    g_globalContext->g_totalConnection += 1;

    printServerStats(g_globalContext, "Created");

    pthread_mutex_unlock(g_screenLock);
    pthread_mutex_unlock(g_contextLock);

    //Loop while server is connected to client
    while (bContinue)
    {
        // should be blocked when a new RPC has not called us yet
        //printf("Waiting for client to send buffer\n");
        //Update total number of RPCs for server and client


        bzero(buffer, BUFFER_SIZE);
        valread = read(this->m_socket, buffer, BUFFER_SIZE);

        pthread_mutex_lock(g_contextLock);
        pthread_mutex_lock(g_screenLock);
        g_globalContext->g_rpcCount += 1;
        localContext->incrementRpcCount();
        printf("Socket %d (#%d) <-- %s\n", m_socket, localContext->getRpcCount(), buffer);
        pthread_mutex_unlock(g_screenLock);
        pthread_mutex_unlock(g_contextLock);

        if (valread <= 0)
        {
            //printf("errno is %d\n", errno);
            bContinue = false;
            break;
        }

        // reset token
        arrayTokens.clear();
        this->ParseTokens(buffer, arrayTokens);

        // string statements are not supported with a switch, so using if/else
        // logic to dispatch
        string aString = arrayTokens[RPCTOKEN];

        //If we received a Connect RPC and the server is not yet connected,
        // process the ConnectRPC
        if ((bConnected == false) && (aString == CONNECT))
        {
            bStatusOk = ProcessConnectRPC(arrayTokens, g_screenLock);

            if (bStatusOk == true)
            {
                bConnected = true;
            }
        }
        //Process disconnect RPC if server connected
        else if (aString == DISCONNECT)
        {
            bStatusOk = ProcessDisconnectRPC(g_screenLock);
            bConnected = false;
            bContinue = false; // Leaving this loop, as we are done
            break;
        }
        //If connected, authenticate = true, and RPC equal to one of the calculator functions
        //then, call the ProcessCal() to calculate input
        else if(bConnected && m_authenticated && (aString == CALC_EXPR ||
                                                  aString == CALC_CONV ||
                                                  aString == CALC_STAT))
        {
            ProcessCal(arrayTokens, g_screenLock);
        }
        //If RPC is not supported, print status on screen
        else
        {
           // Not in our list, perhaps, print out what was sent
           sendBuffer((char*)(GENERAL_FAIL.c_str()), g_screenLock);
           pthread_mutex_lock(g_screenLock);
           printf("RPC received on Socket %d is not supported...\n",
                  m_socket);
            pthread_mutex_unlock(g_screenLock);
        }
    }

    //Updating global count of connections
    pthread_mutex_lock(g_contextLock);
    pthread_mutex_lock(g_screenLock);
    g_globalContext->g_activeConnection -= 1;
    printServerStats(g_globalContext, "Destroyed");
    pthread_mutex_unlock(g_screenLock);
    pthread_mutex_unlock(g_contextLock);

    return true;
}

//*************************
//      Private method
//**************************

/**
 * ProcessConnectRPC will send connect flag to server with user authentication
 * process
 */
bool ClientHandler::ProcessConnectRPC(std::vector<std::string>& arrayTokens, pthread_mutex_t *g_screenLock)
{
    //Define the position of the username and the password in the token
    const int USERNAMETOKEN = 1;
    const int PASSWORDTOKEN = 2;
    char szBuffer[80];

    //if user entered empty strings for credentials, send failure
    if (arrayTokens.size() < 3)
    {
        strcpy(szBuffer, GENERAL_FAIL.c_str());
        sendBuffer(szBuffer, g_screenLock);
        return false;
    }

    // Strip out tokens 1 and 2 (username, password)
    string userNameString = arrayTokens[USERNAMETOKEN];
    string passwordString = arrayTokens[PASSWORDTOKEN];

    // Authenticate based on parsed credentials
    m_authenticated =
            m_authenticator.authenticate(userNameString, passwordString);

    if (m_authenticated)
        strcpy(szBuffer, SUCCESS.c_str());
    else
        strcpy(szBuffer, GENERAL_FAIL.c_str());

    // Send Response back on our socket
    sendBuffer(szBuffer, g_screenLock);

    //return the result of the authentication
    return m_authenticated;
}

/**
 * ProcessDisconnectRPC will send the disconnect flag to server
 */
bool ClientHandler::ProcessDisconnectRPC(pthread_mutex_t *g_screenLock)
{
    //Declare a buffer for the response
    const int BUFFER_SIZE = 1024;
    char szBuffer[BUFFER_SIZE] = {0};

    //Add response to the buffer
    strcpy(szBuffer, SUCCESS.c_str());

    // Send Response back on our socket
    sendBuffer(szBuffer, g_screenLock);
    close(m_socket);

    return true;
}

/**
 * ProcessCal will process the arrayTokens(incoming RPC) and extract it to each of the calculator function
 * ,then return the result after calculation back to the client
 */
bool ClientHandler::ProcessCal(vector<std::string> &arrayTokens, pthread_mutex_t *g_screenLock)
{
    //Declaring a string to store the result
    string result;
    const int BUFFER_SIZE = 1024;
    char szBuffer[BUFFER_SIZE] = {0};

    Calculator myCalc;

    //Calculate expression
    try
    {
       if (arrayTokens[0] == CALC_EXPR && arrayTokens.size() == 2)
       {
           result = myCalc.calculateExpression(arrayTokens[1]);
           result = result + ";" + SUCCESS;
       }

       else if(arrayTokens[0] == CALC_CONV && arrayTokens.size() == 3)
       {
           result = myCalc.convertor(arrayTokens[1], arrayTokens[2]);
           result = result + ";" + SUCCESS;
       }

       else if (arrayTokens[0] == CALC_STAT && arrayTokens.size() == 2)
       {
           result = myCalc.summary(arrayTokens[1]);
           result = result + ";" + SUCCESS;
       }
       else
        {
            result = "0;" + GENERAL_FAIL;
        }
    }
    catch (invalid_argument &e)
    {
        result = "0;" + GENERAL_FAIL;
    }
    catch (exception &e)
    {
        result = "0;" + GENERAL_FAIL;
    }

    //Copy result to buffer and send buffer to client
    strcpy(szBuffer, result.c_str());
    sendBuffer(szBuffer, g_screenLock);

    return true;
}

/**
 * ParseTokens is going to populate a String vector with tokens extracted from the string the
 * client sent. The delimiter will be a ";"
 * An example buffer could be "connect;mike;mike;"
 */
void ClientHandler::ParseTokens(char * buffer, std::vector<std::string> & a)
{
    //Declare variables to facilitate the parsing of input buffer
    char* token;
    char* rest = (char *) buffer;

    //Loop through the input buffer, and extract strings using the ';' delimiter
    while ((token = strtok_r(rest, ";", &rest)))
    {
        a.push_back(token);
    }

    return;
}

/**
 * sendBuffer is a helper function to send the result to client
 */
void ClientHandler::sendBuffer(char *szBuffer, pthread_mutex_t *g_screenLock)
const
{
    //Add null termination and send buffer to client
    int nlen = strlen(szBuffer);
    szBuffer[nlen] = 0;
    send(m_socket, szBuffer, strlen(szBuffer) + 1, 0);
    pthread_mutex_lock(g_screenLock);
    printf("Socket %d (#%d) --> %s\n", m_socket, localContext->getRpcCount(), szBuffer);
    pthread_mutex_unlock(g_screenLock);
}

/**
 * printServerStats will print out the status of server in each thread
 */
void ClientHandler::printServerStats(const GlobalContext *g_globalContext,
                                     const string &phase) const
{
    auto input = phase.c_str();
    printf("\n");
    printf("********************************************************\n");
    printf("%s Thread %lu on Socket %d.\n", input, pthread_self(), m_socket);
    printf("Max # of connections: %d\n", g_globalContext->g_maxConnection);
    printf("Active connections: %d\n", g_globalContext->g_activeConnection);
    printf("Total # of connections: %d\n", g_globalContext->g_totalConnection);
    printf("Total # of Server handled RPCs: %d\n", g_globalContext->g_rpcCount);
    printf("Total # of Client instance RPCs: %d\n", localContext->getRpcCount());
    printf("********************************************************\n\n");
}


