//Author  : Group#2
//Date    : 03/12/2022
//Version : 2.0
//Filename: ClientTestDriver.cpp

/**
 * This is a test driver to automate simultaneous client connections to the
 * server. This driver takes in 3 command line inputs:
 *      1- IP Address (e.g. 127.0.0.1)
 *      2- Port Number (e.g. 8080)
 *      3- Number of client threads to be created.
 *
 * Note: The maximum number of simultaneous connections is artificially
 * limited to 75 using a semaphore, due to resource limitations on CS1
 * machine. On local machines, with removing the semaphore restrictions,
 * number of simultaneous connections can be 200+
 */

#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <pthread.h>
#include <ctime>
#include "semaphore.h"


using namespace std;

//Struct for host address and port
struct hostAddr {
    string ipAddr;
    string port;
};

//Setting Buffer size to 1024 bytes
const int BUFFER_SIZE = 1024;


//A mutex to protect screen printing from races between client threads
pthread_mutex_t g_screenLock;

//A semaphore to control maximum number of simultaneous connections to the
// server
sem_t fullQueue;

//Control for the semaphore
const int MAX_CONNECTIONS = 75;

/******************************************************************************/
/**************************** Function Prototypes *****************************/
/******************************************************************************/

/**
 * Prompts the user to enter credentials via keyboard, then returns the
 * credentials in a format ready for RPC
 * @return A string containing user credentials
 */
string getCredentials();

/**
 * Parse RPC buffer into tokens using delimiter ";"
 * @param buffer A char array pointer containing the buffer to be parsed
 * @param a A vector of strings containing the parsed RPC parameters
 */
void ParseTokens(char* buffer, std::vector<std::string>& a);

/**
 * ConnectToServer will connect to the Server based on command line
 * @param serverAddress A pointer to a char array containing the IP address
 *                      of the server
 * @param port An int containing the port on the server for the connection
 * @param sock An int containing the socket number
 * @return A bool indicating if the connection was successful or not
 */
bool ConnectToServer(const char *serverAddress, int port, int & sock);

/**
 * Sends a buffer to the server
 * @param sock A socket number to send the buffer through
 * @param buffer The buffer to be sent
 */
void sendBuffer(int sock, char *buffer);

/**
 * Waits and reads a buffer from server
 * @param sock A socket to listen for the incoming buffer
 * @param buffer The buffer received
 */
void readBuffer(int sock, char *buffer);


/**
 * This function is the entry point of each client thread
 * @param hostAddr T host address struct
 * @return nullptr
 */
void* threadExecution(void *hostAddr);

/******************************************************************************/
/********************** End of Function Prototypes ****************************/
/******************************************************************************/

int main(int argc, char const* argv[])
{
   // Welcome the user
   cout << "*************************************************" << endl;
   cout << "*   Welcome to the Group 2 Client Test Driver   *" << endl;
   cout << "*************************************************" << endl;

   //check if user entered correct # of Command Line args for IP and Port
   if (argc < 4)
   {
      //If insufficient number of args, print error and exit program.
      cout << "\nERROR: Missing IP Address or Port number, or # of threads.\n";
      cout << "Exiting Client Application...\n";
      return -1;
   }

    //Init variables
    const int NUM_THREADS = stoi (argv[3]);
    struct hostAddr myHostAddr;
    myHostAddr.ipAddr = argv[1];
    myHostAddr.port = argv[2];
    int sem_Val; //To store current value of semaphore

    //If user entered a thread number less than 1, then exit program
    if (NUM_THREADS <= 0)
    {
        cout << "\nERROR: Number of threads must be greater than 1.\n";
        cout << "Exiting Client Application...\n";
        return -1;
    }

    //Init screen mutex
    if (pthread_mutex_init(&g_screenLock, nullptr) < 0)
    {
        printf("ERROR: Failed to init mutex. Exiting program.\n");
        return -1;
    }

    //Init the connections semaphore
    sem_init(&fullQueue, 0, MAX_CONNECTIONS);

    //Create Threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        //Wait for available sockets
        sem_wait(&fullQueue);

        //Create a new thread
        pthread_t* testThread = new pthread_t();
        pthread_create(testThread,
                       nullptr,
                       threadExecution,
                       (void *)&myHostAddr);

        //Detach thread from process (i.e. no join needed)
        pthread_detach(*testThread);

        //Artificial sleep time
        usleep(5000);
    }

    //Wait for all threads to finish executing by evaluating the semaphore value
    do
    {
        usleep(100000);
        sem_getvalue(&fullQueue, &sem_Val);
    }
    while (sem_Val < MAX_CONNECTIONS);

    return 0;
}

void* threadExecution(void* inHostAddr)
{
    //initialize data
    int sock = 0;
    struct sockaddr_in serv_addr;

    //Constants for creating buffers
    const string CONNECT = "connect",
            DISCONNECT = "disconnect",
            CALC_EXPR = "calculateExpression",
            CALC_STAT = "calculateStats",
            CALC_CONV = "conversion",
            SUCCESS = "0",
            FAIL = "-1";
    char buffer[BUFFER_SIZE] = { 0 };
    const char *serverAddress = ((hostAddr*)inHostAddr)->ipAddr.c_str();
    const int port = atoi((*(hostAddr*)inHostAddr).port.c_str());
    bool bConnect = false;

    //Strings for buffer creation
    string connectRPC,
            disconnectRPC,
            calcExpRPC,
            calcStatsRPC,
            convRPC;

    //Vector to store parsed version of incoming buffer
    vector<string> arrayTokens;

    //init a seeded random to be used for random wait times
    srand(time(nullptr));

    //Attempt to connect to server
    bConnect = ConnectToServer(serverAddress, port, sock);

   //if failed to connect the to server, print error message and exit thread
   if(!bConnect)
   {
       pthread_mutex_lock(&g_screenLock);
       printf("ERROR: Failed to connect to server. Exiting thread "
                       "%lu\n.", pthread_self());
       pthread_mutex_unlock(&g_screenLock);

       //release semaphore
       sem_post(&fullQueue);
       return nullptr;
   }

    //Reset buffers and Authenticate user
    bzero(buffer, BUFFER_SIZE);
    connectRPC = CONNECT + ";";

    //Get credentials from user
    connectRPC = connectRPC + getCredentials() + ";";

    //Copy the created the RPC string to the buffer
    strcpy(buffer, &connectRPC[0]);
    sendBuffer(sock, buffer);

    //Read from server
    readBuffer(sock, buffer);
    ParseTokens(buffer, arrayTokens);

    //check if successfully connected and authenticated
    if (arrayTokens[0] != SUCCESS)
    {
        //release semaphore
        sem_post(&fullQueue);
        return nullptr;
    }

    //Sleep
    usleep(100000 + rand() % 100000);

    //For loop to test 3 RPCs 6 times each on each thread/client
    for (int i = 0; i < 6; i++)
    {
        //Use Calculate Expression

        //Clear buffers
        bzero(buffer, BUFFER_SIZE);

        //Create message to be sent
        calcExpRPC = CALC_EXPR + ";";
        calcExpRPC = calcExpRPC + to_string(rand() % 1000 - 500) + "/" +
                     to_string(rand() % 1000 - 500) + " - " +
                     to_string(rand() % 1000 - 500) + " +" +
                     to_string(rand() % 1000 - 500) + " ^ " +
                     to_string(rand() % 4) + " +" +
                     to_string(rand() % 1000 - 500) + "*" +
                     to_string(rand() % 1000 - 500) + ";";

        //send buffer
        strcpy(buffer, &calcExpRPC[0]);
        sendBuffer(sock, buffer);

        //read response
        readBuffer(sock, buffer);

        //Sleep
        usleep(10000 + rand() % 5000);


        //Use Stats RPC

        //Clear buffers
        bzero(buffer, BUFFER_SIZE);
        arrayTokens.clear();

        //Create message to be sent
        calcStatsRPC = CALC_STAT + ";";
        calcStatsRPC = calcStatsRPC + to_string(rand() % 1000) + " " +
                       to_string(rand() % 1000) + " " +
                       to_string(rand() % 1000) + " " +
                       to_string(rand() % 1000) + " " +
                       to_string(rand() % 1000) + ";";

        //send buffer
        strcpy(buffer, &calcStatsRPC[0]);
        sendBuffer(sock, buffer);

        //read response
        readBuffer(sock, buffer);

        //Sleep
        usleep(10000 + rand() % 7000);

        //Use Conversion RPC

        //Clear buffers
        bzero(buffer, BUFFER_SIZE);

        //Create message to be sent
        convRPC = CALC_CONV + ";";
        convRPC = convRPC + "5;" + to_string(rand() % 1000) + ";";

        //send buffer
        strcpy(buffer, &convRPC[0]);
        sendBuffer(sock, buffer);

        //read response
        readBuffer(sock, buffer);

        //Sleep
        usleep(10000 + rand() % 2000);
    }


    //Use Disconnect RPC

    //Clear buffers
    bzero(buffer, BUFFER_SIZE);

    //Create message to be sent to server
    disconnectRPC = DISCONNECT + ";";

    //send buffer
    strcpy(buffer, &disconnectRPC[0]);
    sendBuffer(sock, buffer);

    //read response
    readBuffer(sock, buffer);
    close(sock);


    sem_post(&fullQueue);
    return nullptr;
}

void sendBuffer(int sock, char *buffer)
{
   //Add a null terminator
   int nlen = strlen(buffer);
   buffer[nlen] = 0;

   //Send the created RPC buffer to server and print on screen
    send(sock, buffer, strlen(buffer) + 1, 0);
    pthread_mutex_lock(&g_screenLock);
    printf("Socket %d --> %s\n", sock, buffer);
    pthread_mutex_unlock(&g_screenLock);

}

void readBuffer(int sock, char *buffer)
{
    //Read buffer from socket connection
    bzero(buffer, BUFFER_SIZE);
    read(sock, buffer, BUFFER_SIZE);

    //Print buffer on the screen
    pthread_mutex_lock(&g_screenLock);
    printf("Socket %d <-- %s\n", sock, buffer);
    pthread_mutex_unlock(&g_screenLock);
}


string getCredentials()
{
   //hardcoded credentials for testing
   return ("Mike;Mike");
}


void ParseTokens(char* buffer, std::vector<std::string>& a)
{
   //Declare variables to facilitate the parsing of input buffer
   char* token;
   char* rest = (char*)buffer;

   //Loop through the input buffer, and extract strings using the ';' delimiter
   while ((token = strtok_r(rest, ";", &rest)))
   {
      a.push_back(token);
   }

   return;
}


bool ConnectToServer(const char *serverAddress, int port, int & sock)
{
   //Create socket
   struct sockaddr_in serv_addr;
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
       pthread_mutex_lock(&g_screenLock);
       printf("\n Socket creation error \n");
       pthread_mutex_unlock(&g_screenLock);
       return false;
   }
      //Debuging
   else
   {
       pthread_mutex_lock(&g_screenLock);
       printf("Socket: %d connected\n", sock);
       pthread_mutex_unlock(&g_screenLock);
   }

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(port);

   // Convert IPv4 and IPv6 addresses from text to binary form
   if (inet_pton(AF_INET, serverAddress, &serv_addr.sin_addr) <= 0)
   {
       pthread_mutex_lock(&g_screenLock);
       printf("\nInvalid address/ Address not supported \n");
       pthread_mutex_unlock(&g_screenLock);
      return false;
   }

   //Connect to server
   if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
   {
       pthread_mutex_lock(&g_screenLock);
       printf("\nConnection Failed \n");
       pthread_mutex_unlock(&g_screenLock);
      return false;
   }

   return true;
}

