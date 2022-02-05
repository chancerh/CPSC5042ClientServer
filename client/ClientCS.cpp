// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <iterator>
#include <iostream>
#include <string.h>
#include <termio.h>

using namespace std;

// This is a C Program. No classes. You may turn this into an Object Oriented C++ program if you wish

string getCredentials()
{
    struct termios orig_termios;

    string username, password;
    cout << "\nPlease enter your username: ";
    cin >> username;



    cout << "Please enter your password: ";

    //Modify console settings to mask password
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios new_termios = orig_termios;
    new_termios.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    //Store user input
    cin >> password;

    //Restore the terminal settings to start echoing again
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);

    return username + ";" + password;
}

void ParseTokens(char* buffer, std::vector<std::string>& a)
{
    char* token;
    char* rest = (char*)buffer;

    while ((token = strtok_r(rest, ";", &rest)))
    {
        //printf("%s\n", token);
        a.push_back(token);
    }

    return;
}

/*
    ConnectToServer will connect to the Server based on command line
*/
bool ConnectToServer(const char *serverAddress, int port, int & sock)
{
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, serverAddress, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return false;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return false;
    }

    return true;
}

int main(int argc, char const* argv[])
{
    // Welcome the user
    cout << "*************************************************" << endl;
    cout << "*   Welcome to the Group 2 Client Application   *" << endl;
    cout << "*************************************************" << endl;



    int sock = 0;
    struct sockaddr_in serv_addr;
    string connectRPC = "connect;";
    const char* statusRPC = "status;";
    const char* logoffRPC = "disconnect;";
    char buffer[1024] = { 0 };
    const char *serverAddress = argv[1];
    const int port = atoi(argv[2]);
    char connected;
    const int SLEEP_TIME = 10;
 
    bool bConnect = ConnectToServer(serverAddress, port, sock);

    if (bConnect == true)
    {
        do
        {
            //reset buffers
            memset(buffer, 0 , 1024);
            connectRPC = "connect;";

            //Get credentials from user
            connectRPC = connectRPC + getCredentials() + ";";

            //Copy the created the RPC string to the buffer
            strcpy(buffer, &connectRPC[0]);

            //Add a null terminator
            int nlen = strlen(buffer);
            buffer[nlen] = 0;

            //Send the created RPC buffer to server
            send(sock, buffer, strlen(buffer) + 1, 0);

            //Read from server
            read(sock, buffer, 1024);
            //printf("Return response = %s with valread=%d\n", buffer, valread);
            connected = buffer[0];

            if (connected == '0')
            {
                cout << "\nLogin successful!" << endl;
            }
            else
            {
                cout << "\nInvalid login!" << endl;
            }
        } while (connected != '0');
    }
    else
    {
        printf("Exit without calling RPC");
    }

    //Sleep for 10 seconds
    printf("\nSleeping for %d seconds...\n\n", SLEEP_TIME);
    sleep(SLEEP_TIME);

    // Do a Disconnect Message
    if (bConnect == true)
    {

        cout << "Disconnecting from Server" << endl;

        //reset buffers
        memset(buffer, 0 , 1024);
        strcpy(buffer, logoffRPC);
        int nlen = strlen(buffer);
        buffer[nlen] = 0;   // Put the null terminator
        send(sock, buffer, strlen(buffer) + 1, 0);

        // printf("DisConnect message sent with %d bytes\n", valwrite);


        read(sock, buffer, 1024);
        //printf("Return response = %s with valread=%d\n", buffer, valread);

        if ((string)buffer == "disconnect")
        {
            cout << "Disconnected successfully" << endl;
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

    return 0;
}


