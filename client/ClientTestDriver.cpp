//Author  : Group#2
//Date    : 02/07/2022
//Version : 1.0
//Filename: ClientCS.cpp

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <iterator>
#include <iostream>
#include <string.h>
#include <termio.h>
#include <pthread.h>

using namespace std;

struct hostAddr {
    string ipAddr;
    string port;
};

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
void sendBuffer(int sock, char *buffer);
/******************************************************************************/
/********************** End of Function Prototypes ****************************/
void* threadExecution(void *hostAddr);

/******************************************************************************/

int main(int argc, char const* argv[])
{
   // Welcome the user
   cout << "*************************************************" << endl;
   cout << "*   Welcome to the Group 2 Client Test Driver   *" << endl;
   cout << "*************************************************" << endl;

   //check if user entered correct # of Command Line args for IP and Port
   if (argc < 3)
   {
      //If insufficient number of args, print error and exit program.
      cout << "\nMissing IP Address or Port number.\n";
      cout << "Exiting Client Application...\n";
      return -1;
   }

   const int NUM_THREADS = 20;
   pthread_t testThreads[NUM_THREADS];
   struct hostAddr myHostAddr;
   myHostAddr.ipAddr = argv[1];
   myHostAddr.port = argv[2];

   for (int i = 0; i < NUM_THREADS; i++)
   {
      pthread_create(&testThreads[i], nullptr, threadExecution, (void *)
      &myHostAddr);
      usleep(100);
   }

   for (int i = 0; i < NUM_THREADS; i++)
   {
      pthread_join(testThreads[i], nullptr);
   }

   return 0;
}

void* threadExecution(void* inHostAddr)
{//initialize data
   int sock = 0;
   struct sockaddr_in serv_addr;
   string connectRPC = "connect;";
   string calcExpRPC = "calculateExpression;";
   //string calcExpRPC = "";
   const char* logoffRPC = "disconnect;";
   char buffer[1024] = { 0 };
   const char* arguments[3];
   const char *serverAddress = ((hostAddr*)inHostAddr)->ipAddr.c_str();
   const int port = atoi((*(hostAddr*)inHostAddr).port.c_str());
   char connected;
   const int SLEEP_TIME = 10;
   bool bConnect = false;


   bConnect = ConnectToServer(serverAddress, port, sock);

   //testing if client connect to server
   if (bConnect == true)
   {
      //keep asking for user input, successfully connected.
      do
      {
         //reset buffers
         memset(buffer, 0 , 1024);
         connectRPC = "connect;";

         //Get credentials from user
         connectRPC = connectRPC + getCredentials() + ";";

         //Copy the created the RPC string to the buffer
         strcpy(buffer, &connectRPC[0]);
         sendBuffer(sock, buffer);

         //Read from server
         read(sock, buffer, 1024);
         connected = buffer[0];

         //check if successfully connected
         if (connected == '0')
         {
            cout << "\nSocket: " << sock << ", Login successful!" << endl;
            usleep(10000);
         }
         else
         {
            cout << "\nSocket: "<< sock << ", Invalid login!" << endl;
         }
      } while (connected != '0');
   }
   else
   {
      printf("Exit without calling RPC");
   }

   //Sleep for 10 seconds
//    printf("\nSleeping for %d seconds...\n\n", SLEEP_TIME);
//    sleep(SLEEP_TIME)

   if (bConnect == true)
   {
      string expr;
      vector<string> result;

      // calcExpRPC = "calculateExpression;";

      expr = to_string(rand() % 1000) + "/" +
              to_string(rand() % 1000) + " - " + to_string(rand() % 1000) +
              to_string(rand() % 1000) + " ^ " + "2" + " +" +
              to_string(rand() % 1000) + "*" + to_string(rand() % 1000);

      cout << "\nSocket: " << sock << ", " << expr << endl;
      calcExpRPC = calcExpRPC + expr + ";";
      strcpy(buffer, &calcExpRPC[0]);

      //Add a null terminator
      int nlen = strlen(buffer);
      buffer[nlen] = 0;

      //Send the created RPC buffer to server
      send(sock, buffer, strlen(buffer) + 1, 0);

      //Read from server
      read(sock, buffer, 1024);
      ParseTokens(buffer, result);

      if(result[1] == "0")
      {
         printf("\nSocket: %d, %s\n",sock, result[0].c_str());
      }
      else
      {
         printf("%s\n", "Invalid expression.");
      }
   }

   // Do a Disconnect Message
   if (bConnect == true)
   {
      cout << "\nSocket: " << sock << ", Disconnecting from Server" << endl;

      //reset buffers
      memset(buffer, 0 , 1024);

      //create the buffer to be sent via RPC
      strcpy(buffer, logoffRPC);
      int nlen = strlen(buffer);
      buffer[nlen] = 0;   // Put the null terminator

      //send buffer
      send(sock, buffer, strlen(buffer) + 1, 0);

      //get RPC response from server
      read(sock, buffer, 1024);

      //check if buffer equal to disconnect
      if ((string)buffer == "disconnect")
      {
         cout << "Socket: " << sock << ", Disconnected successfully" << endl;
      }
      else
      {
         cout << "Failed to disconnect successfully" << endl;
      }
   }
   else
   {
      printf("Exit without calling RPC");
   }

   // Terminate connection
   close(sock);
}

void sendBuffer(int sock, char *buffer)
{
   //Add a null terminator
   int nlen = strlen(buffer);
   buffer[nlen] = 0;

   //Send the created RPC buffer to server
   send(sock, buffer, strlen(buffer) + 1, 0);
}


string getCredentials()
{
   //return
   return ("Aacer;Pass123!");
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
      printf("\n Socket creation error \n");
      return false;
   }
      //Debuging
   else
   {
      printf("Socket: %d connected\n", sock);
   }

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(port);

   // Convert IPv4 and IPv6 addresses from text to binary form
   if (inet_pton(AF_INET, serverAddress, &serv_addr.sin_addr) <= 0)
   {
      printf("\nInvalid address/ Address not supported \n");
      return false;
   }

   //Connect to server
   if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
   {
      printf("\nConnection Failed \n");
      return false;
   }

   return true;
}

