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
    m_authenticator = Authenticator(filename);

}
/**
 * Destructor
 */
ClientHandler::~ClientHandler() = default;

/**
 * ProcessRPC will examine buffer and will essentially control
 */
bool ClientHandler::ProcessRPC(pthread_mutex_t *g_lock,
                               GlobalContext *g_globalContext)
{
    char buffer[1024] = { 0 };
    std::vector<std::string> arrayTokens;
    int valread = 0;
    bool bConnected = false;
    bool bStatusOk = true;
    const int RPCTOKEN = 0;
    bool bContinue = true;

    pthread_mutex_lock(g_lock);
    g_globalContext->g_activeConnection += 1;
    if (g_globalContext->g_activeConnection > g_globalContext->g_maxConnection)
       g_globalContext->g_maxConnection = g_globalContext->g_activeConnection;
    g_globalContext->g_totalConnection += 1;
    pthread_mutex_unlock(g_lock);

    //Loop while server is connected to client
    while ((bContinue))
    {
        // should be blocked when a new RPC has not called us yet
        printf("Waiting for client to send buffer\n");

        bzero(buffer, sizeof(buffer));
        valread = read(this->m_socket, buffer, sizeof(buffer));
        printf("Received buffer on Socket %d: %s\n", m_socket, buffer);

        pthread_mutex_lock(g_lock);
        g_globalContext->g_rpcCount += 1;
        pthread_mutex_unlock(g_lock);

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
            bStatusOk = ProcessConnectRPC(arrayTokens);  // connect RPC
            if (bStatusOk == true)
            {
                printf("User Login Successful!\n ");
                bConnected = true;
            }
            else{
                printf("User Login Unsuccessful!\n");
            }
        }

        //Process disconnect RPC if server connected
        else if ((bConnected == true) && (aString == DISCONNECT))
        {
            bStatusOk = ProcessDisconnectRPC();
            bConnected = false;
            bContinue = false; // Leaving this loop, as we are done
        }

        else if(bConnected && m_authenticated && (aString == CALC_EXPR ||
                                                  aString == CALC_CONV ||
                                                  aString == CALC_STAT))
        {
            ProcessCal(arrayTokens);
        }

        //If RPC is not supported, print status on screen
        else
        {
           // Not in our list, perhaps, print out what was sent
           sendBuffer((char*)(GENERAL_FAIL.c_str()));
           printf("RPC received on Socket %d is not supported...\n",
                  m_socket);
        }
    }

    //Updating global count of connections
    pthread_mutex_lock(g_lock);
    g_globalContext->g_activeConnection -= 1;
    pthread_mutex_unlock(g_lock);

    return true;
}

//*************************
//      Private method
//**************************

/**
 * ProcessConnectRPC will send connect flag to server with user authentication
 * process
 */
bool ClientHandler::ProcessConnectRPC(std::vector<std::string>& arrayTokens)
{
    if (arrayTokens.size() < 3)
    {
        throw new invalid_argument(INVALID_ARG);
    }

    //Define the position of the username and the password in the token
    const int USERNAMETOKEN = 1;
    const int PASSWORDTOKEN = 2;
    char szBuffer[80];

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
    sendBuffer(szBuffer);

    //return the result of the authentication
    return m_authenticated;
}

/**
 * ProcessDisconnectRPC will send the disconnect flag to server
 */
bool ClientHandler::ProcessDisconnectRPC()
{
    //Declare a buffer for the response
    char szBuffer[16];

    //Add response to the buffer
    strcpy(szBuffer, SUCCESS.c_str());

    // Send Response back on our socket
    sendBuffer(szBuffer);

    return true;
}

bool ClientHandler::ProcessCal(vector<std::string> &arrayTokens) {
    //Declaring a string to store the result
    string result;
    char szBuffer[1024] = {0};

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
    sendBuffer(szBuffer);

    return true;
}

/**
 * Going to populate a String vector with tokens extracted from the string the
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

void ClientHandler::sendBuffer(char *szBuffer) const
{
    //Add null termination and send buffer to client
    int nlen = strlen(szBuffer);
    szBuffer[nlen] = 0;
    send(m_socket, szBuffer, strlen(szBuffer) + 1, 0);
    printf("Sent buffer on Socket %d: %s\n", m_socket, szBuffer);
}


