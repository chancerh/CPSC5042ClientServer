#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <iterator>
#include "RPCServer.h"

//#define PORT 8081

using namespace std;

RPCServer::RPCServer(const char *serverIP, int port)
{
    m_rpcCount = 0; 
    m_serverIP = (char *) serverIP;
    m_port = port;
    m_authenticated = false;
    m_users = {
            {"Chance", "passw0rd"},
            {"Jusmin", "12340000"},
            {"Aacer",  "Pass123!"},
            {"Troy",   "HelloWorld!"},
            {"Mike",   "Mike"}
    };
};

RPCServer::~RPCServer() {};

/*
* StartServer will create a server on a Port that was passed in, and create a socket
*/

bool RPCServer::StartServer()
{
    int opt = 1;
    const int BACKLOG = 10;

    // Creating socket file descriptor
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
        &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(m_port);

    // Forcefully attaching socket to the port 8080
    if (bind(m_server_fd, (struct sockaddr*)&m_address,
        sizeof(m_address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(m_server_fd, BACKLOG) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    return true;
}

/*
* Will accept a new connection by listening on it's address
*
*/

bool RPCServer::ListenForClient()
{

    int addrlen = sizeof(m_address);

    if ((m_socket = accept(m_server_fd, (struct sockaddr*)&m_address,
        (socklen_t*)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("-> Accepted Connection");
    this->ProcessRPC();
    return true;
}

/*
* Going to populate a String vector with tokens extracted from the string the client sent.
* The delimter will be a ; 
* An example buffer could be "connect;mike;mike;"
*/
void RPCServer::ParseTokens(char * buffer, std::vector<std::string> & a)
{
    char* token;
    char* rest = (char *) buffer;

    while ((token = strtok_r(rest, ";", &rest)))
    {
        a.push_back(token);
    }

    return;
}

/*
* ProcessRPC will examine buffer and will essentially control
*/
bool RPCServer::ProcessRPC()
{
    const char* rpcs[] = { "connect", "disconnect", "status"};
    char buffer[1024] = { 0 };
    std::vector<std::string> arrayTokens;
    int valread = 0;
    bool bConnected = false;
    bool bStatusOk = true;
    const int RPCTOKEN = 0;
    bool bContinue = true;

    while ((bContinue))
    {
        // Should be blocked when a new RPC has not called us yet
        valread = read(this->m_socket, buffer, sizeof(buffer));
        if (valread <= 0)
        {
            //printf("errno is %d\n", errno);
            bContinue = false;
            break;
        }
        printf("\nRPC: %s\n", buffer);

        arrayTokens.clear();
        this->ParseTokens(buffer, arrayTokens);

        // string statements are not supported with a switch, so using if/else logic to dispatch
        string aString = arrayTokens[RPCTOKEN];

        if ((bConnected == false) && (aString == "connect"))
        {
            bStatusOk = ProcessConnectRPC(arrayTokens);  // Connect RPC
            if (bStatusOk == true)
            {
                printf("User Login Successful!\n ");
                bConnected = true;
            }
            else{
                printf("User Login Unsuccessful!\n");
            }
        }
        else if ((bConnected == true) && (aString == "disconnect"))
        {
            bStatusOk = ProcessDisconnectRPC();
            printf("Terminating connection.\n");
            bConnected = false;
            bContinue = false; // We are going to leave this loop, as we are done
        }
        else 
        {
            // Not in our list, perhaps, print out what was sent
            printf("RPCs not supported...\n");
        }

    }

    return true;
}

/*
*/
bool RPCServer::ProcessDisconnectRPC()
{
    char szBuffer[16];
    strcpy(szBuffer, "disconnect");
    // Send Response back on our socket
    int nlen = strlen(szBuffer);
    szBuffer[nlen] = 0;
    send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
    return true;
}

bool RPCServer::ProcessConnectRPC(std::vector<std::string>& arrayTokens)
{

    const int USERNAMETOKEN = 1;
    const int PASSWORDTOKEN = 2;

    char szBuffer[80];

    // Strip out tokens 1 and 2 (username, password)
    string userNameString = arrayTokens[USERNAMETOKEN];
    string passwordString = arrayTokens[PASSWORDTOKEN];

    //Declare an iterator to search for credentials
    unordered_map<string, string>::const_iterator mapIterator = m_users.find(userNameString);

    //Reset authentication flag to false
    m_authenticated = false;

    //Search for the username in map
    //If user is not in map
    if (mapIterator == m_users.end())
    {
        strcpy(szBuffer, "-1;"); // Not Authenticated
    }
    else
    {
        //Check user credentials against stored credentials
        if(m_users[userNameString] == passwordString)
        {
            //If password matches, add success status to buffer
            strcpy(szBuffer, "0;");
            m_authenticated = true;
        }
        else
        {
            //if credentials do not match, add failure status to buffer
            strcpy(szBuffer, "-1;"); // Not Authenticated
            m_authenticated = false;
        }
    }

    // Send Response back on our socket
    int nlen = strlen(szBuffer);
    szBuffer[nlen] = 0;
    send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);

    return m_authenticated;
}