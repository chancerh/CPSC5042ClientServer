// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <vector>
#include <iterator>

// This is a C Program. No classes. You may turn this into an Object Oriented C++ program if you wish

void ParseTokens(char* buffer, std::vector<std::string>& a)
{
    char* token;
    char* rest = (char*)buffer;

    while ((token = strtok_r(rest, ";", &rest)))
    {
        printf("%s\n", token);
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
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char* connectRPC = "connect;MIKE;MIKE;";
    const char* statusRPC = "status;";
    const char* logoffRPC = "disconnect;";
    char buffer[1024] = { 0 };
    const char *serverAddress = argv[1];
    const int port = atoi(argv[2]);
 
    bool bConnect = ConnectToServer(serverAddress, port, sock);

    if (bConnect == true)
    {
        strcpy(buffer, connectRPC);
        int nlen = strlen(buffer);
        buffer[nlen] = 0;   // Put the null terminator
        int valwrite = send(sock, buffer, strlen(buffer)+1, 0);

        printf("Connect message sent with %d bytes\n", valwrite);

        int valread = read(sock, buffer, 1024);
        printf("Return response = %s with valread=%d\n", buffer, valread);


        
    }
    else
    {
        printf("Exit without calling RPC");
    }
 

    // Do a Disconnect Message

    if (bConnect == true)
    {
        strcpy(buffer, logoffRPC);
        int nlen = strlen(buffer);
        buffer[nlen] = 0;   // Put the null terminator
        int valwrite = send(sock, buffer, strlen(buffer) + 1, 0);

        printf("DisConnect message sent with %d bytes\n", valwrite);

        int valread = read(sock, buffer, 1024);
        printf("Return response = %s with valread=%d\n", buffer, valread);
    }
    else
    {
        printf("Exit without calling RPC");
    }

    // Terminate connection
    close(sock);

    return 0;
}
