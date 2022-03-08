//Author  : Group#2
//Date    : 02/07/2022
//Version : 1.0
//Filename: ClientTestDriver.cpp

#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <iterator>
#include <iostream>
#include <cstring>
#include <termio.h>
#include <pthread.h>


using namespace std;

struct hostAddr {
    string ipAddr;
    string port;
};

const int BUFFER_SIZE = 1024;

pthread_mutex_t g_screenLock;

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
 * @param sock
 * @param buffer
 */
void sendBuffer(int sock, char *buffer);

void readBuffer(int sock, char *buffer);

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
      cout << "\nMissing IP Address or Port number, or # of threads.\n";
      cout << "Exiting Client Application...\n";
      return -1;
   }

    //Init variables
    const int NUM_THREADS = stoi (argv[3]);
    pthread_t testThreads[NUM_THREADS];
    struct hostAddr myHostAddr;
    myHostAddr.ipAddr = argv[1];
    myHostAddr.port = argv[2];

    //Init screen mutex
    if (pthread_mutex_init(&g_screenLock, nullptr) < 0)
    {
        printf("ERROR: Failed to init mutex. Exiting program.\n");
        return -1;
    }

    //Create Threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&testThreads[i],
                       nullptr,
                       threadExecution,
                       (void *)&myHostAddr);
//        pthread_detach(testThreads[i]);

        usleep(5000);
    }

    //sleep(10);


    //Wait for threads to join after completion
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(testThreads[i], nullptr);
    }

    sleep(3);


    return 0;
}

void* threadExecution(void* inHostAddr)
{
    //initialize data
    int sock = 0;
    struct sockaddr_in serv_addr;
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
    string connectRPC,
            disconnectRPC,
            calcExpRPC,
            calcStatsRPC,
            convRPC;
    vector<string> arrayTokens;


   bConnect = ConnectToServer(serverAddress, port, sock);

   //if failed to connect the to server, print error message and exit thread
   if(!bConnect)
   {
       pthread_mutex_lock(&g_screenLock);
       printf("ERROR: Failed to connect to server. Exiting thread "
                       "%lu\n.", pthread_self());
       pthread_mutex_unlock(&g_screenLock);
       return nullptr;
   }

   //testing if client connect to server
    if (bConnect == true)
    {
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

        //check if successfully connected
        if (arrayTokens[0] != SUCCESS)
        {
            return nullptr;
        }

        //Sleep
        usleep(1000000);

        //Use Calculate Expression

        //Clear buffers
        bzero(buffer, BUFFER_SIZE);

        //Create message to be sent
        calcExpRPC = CALC_EXPR + ";";
        calcExpRPC = calcExpRPC + to_string(rand() % 1000) + "/" +
                                  to_string(rand() % 1000) + " - " +
                                  to_string(rand() % 1000) + " +" +
                                  to_string(rand() % 1000) + " ^ " + "2" + " +" +
                                  to_string(rand() % 1000) + "*" +
                                  to_string(rand() % 1000) + ";";

        //send buffer
        strcpy(buffer, &calcExpRPC[0]);
        sendBuffer(sock, buffer);

        //read response
        readBuffer(sock, buffer);

        //Sleep
        usleep(10000);

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
                        to_string(rand() % 1000) + ";" ;

        //send buffer
        strcpy(buffer, &calcStatsRPC[0]);
        sendBuffer(sock, buffer);

        //read response
        readBuffer(sock, buffer);

        //Sleep
        usleep(10000);

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
        usleep(10000);

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


    }
    else
    {
        pthread_mutex_lock(&g_screenLock);
        printf("Exit without calling RPC");
        pthread_mutex_lock(&g_screenLock);
    }
    return nullptr;
}

void sendBuffer(int sock, char *buffer)
{
   //Add a null terminator
   int nlen = strlen(buffer);
   buffer[nlen] = 0;

   //Send the created RPC buffer to server
    send(sock, buffer, strlen(buffer) + 1, 0);
    pthread_mutex_lock(&g_screenLock);
    printf("Socket %d --> %s\n", sock, buffer);
    pthread_mutex_unlock(&g_screenLock);

}

void readBuffer(int sock, char *buffer)
{
    bzero(buffer, BUFFER_SIZE);
    read(sock, buffer, BUFFER_SIZE);

    pthread_mutex_lock(&g_screenLock);
    printf("Socket %d <-- %s\n", sock, buffer);
    pthread_mutex_unlock(&g_screenLock);
}


string getCredentials()
{
   //return
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

