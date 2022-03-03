//Author  : Group#2
//Date    : 02/07/2022
//Version : 2.0
//Filename: ClientHandler.cpp

#include "ClientHandler.h"
#include <regex>

using namespace std;

/**
 * Constructor
 */
ClientHandler::ClientHandler(int socket)
{
    m_socket = socket;
    m_authenticated = false;

    //For milestone 1 we use hardcoded values. Later milestone may read and store credentials in file
    m_users = {
            {"Chance", "passw0rd"},
            {"Jusmin", "12340000"},
            {"Aacer",  "Pass123!"},
            {"Troy",   "HelloWorld!"},
            {"Mike",   "Mike"}
    };
}
/**
 * Destructor
 */
ClientHandler::~ClientHandler() {};

/**
 * ProcessRPC will examine buffer and will essentially control
 */
bool ClientHandler::ProcessRPC()
{
    char buffer[1024] = { 0 };
    std::vector<std::string> arrayTokens;
    int valread = 0;
    bool bConnected = false;
    bool bStatusOk = true;
    const int RPCTOKEN = 0;
    bool bContinue = true;

    //Loop while server is connected to client
    while ((bContinue))
    {
        // should be blocked when a new RPC has not called us yet
        printf("Waiting for client to send buffer\n");

        valread = read(this->m_socket, buffer, sizeof(buffer));

        //Acquire lock --> update count of RPCs received --> release lock
        //pthread_mutex_lock(&RPCServer::lock);
        RPCServer::GlobalContext.g_rpcCount ++;
        printf("RPC Count: %d", RPCServer::GlobalContext.g_rpcCount);
        pthread_mutex_unlock(&RPCServer::lock);

        printf("Received buffer from client: %s\n", buffer);

        if (valread <= 0)
        {
            //printf("errno is %d\n", errno);
            bContinue = false;
            break;
        }
        printf("\nRPC: %s\n", buffer);

        // reset token
        arrayTokens.clear();
        this->ParseTokens(buffer, arrayTokens);

        // string statements are not supported with a switch, so using if/else logic to dispatch
        string aString = arrayTokens[RPCTOKEN];

        //If we received a Connect RPC and the server is not yet connected, process the ConnectRPC
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

            //If we received a Disconnect RPC and the server is connected, process disconnect RPC
        else if ((bConnected == true) && (aString == DISCONNECT)) // disconnect RPC
        {
            bStatusOk = ProcessDisconnectRPC();
            printf("Terminating connection.\n");
            bConnected = false;
            bContinue = false; // we are going to leave this loop, as we are done
        }

        ////I combine all the checking in here. I think we can create a separate function to store these string too.
        else if(aString == CALC_EXPR || aString == CALC_BINTOHEX || aString == CALC_BINTODEC
                ||aString == CALC_HEXTOBIN || aString == CALC_DECTOBIN)
        {
        ////call only one function (Jusmin)
            ProcessCal(arrayTokens);
        }

        else if (aString == "summary")
        {
            ProcessStatSummary(arrayTokens);
        }

            //If RPC is not supported, print status on screen
        else
        {
            // Not in our list, perhaps, print out what was sent
            printf("RPCs not supported...\n");
        }

    }

    return true;
}

//*************************
//      Private method
//**************************

/**
 * ProcessConnectRPC will send connect flag to server with user authentication process
 */
bool ClientHandler::ProcessConnectRPC(std::vector<std::string>& arrayTokens)
{

    //Define the position of the username and the password in the token
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
        sleep(1);
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
    printf("sending buffer!\n");
    // Send Response back on our socket
    sendBuffer(szBuffer);
    printf("buffer sent!\n");
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
    strcpy(szBuffer, "disconnect");

    // Send Response back on our socket
    int nlen = strlen(szBuffer);
    szBuffer[nlen] = 0;
    send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);

    return true;
}

void ClientHandler::ProcessCal(vector<std::string> &arrayTokens) {
    //Declaring a string to store the result
    string result;
    char szBuffer[80];

    Calculator myCalc;
    ////combine all our function -> check which one to call
    ////I have not use your convertor, but this use do the same thing
    //Calculate expression
    try
    {
        if(arrayTokens[0]==CALC_EXPR){
            result = myCalc.calculateExpression(arrayTokens[1]);
            result = result + ";" + SUCCESS;
        }
        else if(arrayTokens[0]==CALC_BINTODEC){
            result = myCalc.binToDec(arrayTokens[1]);
            result = result + ";" + SUCCESS;
        }
        else if(arrayTokens[0]==CALC_DECTOBIN){
            result = myCalc.decToBin(arrayTokens[1]);
            result = result + ";" + SUCCESS;
        }
        else if(arrayTokens[0]==CALC_HEXTOBIN){
            result = myCalc.decToBin(arrayTokens[1]);
            result = result + ";" + SUCCESS;
        }
        else if(arrayTokens[0]==CALC_BINTOHEX){
            result = myCalc.decToBin(arrayTokens[1]);
            result = result + ";" + SUCCESS;
        }
    }
        //if invalid argument, return failure status
    catch (invalid_argument& e)
    {
        result = "0;" + GENERAL_FAIL;
    }

    //Copy result to buffer and send buffer to client
    strcpy(szBuffer, result.c_str());
    sendBuffer(szBuffer);
    printf("send out result: %s\n",szBuffer);

}

void ClientHandler::ProcessCalcExp(vector<std::string> &arrayTokens)
{
    //Declaring a string to store the result
    string result;
    char szBuffer[80];

    Calculator myCalc;

    //Calculate expression
    try
    {
        result = myCalc.calculateExpression(arrayTokens[1]);
        result = result + ";" + SUCCESS;
    }
        //if invalid argument, return failure status
    catch (invalid_argument& e)
    {
        result = "0;" + GENERAL_FAIL;
    }

    //Copy result to buffer and send buffer to client
    strcpy(szBuffer, result.c_str());
    sendBuffer(szBuffer);

}

void ClientHandler::HexConversionRPC(vector<std::string> &arrayTokens)
{
    //Declaring a string to store the result
    string result;
    char szBuffer[80];

    Calculator myCalc;

    //Calculate expression
//    try
//    {
        // Hex to Dec
        if(arrayTokens[1] == "0")
        {
            result = myCalc.convertorMenu( "3", arrayTokens[2]);
        }
        // Dec to Hex
        else if (arrayTokens[1] == "1"){
            result = myCalc.convertorMenu( "4", arrayTokens[2]);
            result = myCalc.convertorMenu( "1", result );
        }
        else{
            throw invalid_argument("Invalid!");
        }
        result = result + ";" + SUCCESS;
//    }
//    //if invalid argument, return failure status
//    catch (invalid_argument& e)
//    {
//        result = "0;" + GENERAL_FAIL;
//    }

    //Copy result to buffer and send buffer to client
    strcpy(szBuffer, result.c_str());
    sendBuffer(szBuffer);

}

string ClientHandler::ProcessStatSummary(vector<std::string> &arrayTokens)
{
    vector<float> vec;
    Calculator myCalc;

    // For now, assume someone typed in mean followed by numbers separated by 
    // spaces make a whitespace regex
    auto const re = regex{R"(\s+)"};
    auto const vecString =
            vector<string>(sregex_token_iterator{begin(arrayTokens[1]),
						 end(arrayTokens[1]), re, -1},
		sregex_token_iterator{});

    // put results into single string
    string resultString;

    for (string str : vecString)
    {
	try {
	    vec.push_back(stof(str));
	}
	catch(const invalid_argument& is) {
	    resultString = str + " is not valid input\n;0";
	    char* charRes = &resultString[0];
	    send(this->m_socket, charRes, strlen(charRes) + 1, 0);
	    return resultString;
	}
    }
    // Get summary statistics
    vector<float> statSum = myCalc.summary(vec);


    resultString =
            "Min      " + to_string(statSum[0]) + "\n" +
            "1st Qu.  " + to_string(statSum[1]) + "\n" +
            "Median   " + to_string(statSum[2]) + "\n" +
            "Mean     " + to_string(statSum[3]) + "\n" +
            "3rd Qu.  " + to_string(statSum[4]) + "\n" +
            "Max      " + to_string(statSum[5]) + "\n;0";

    char* charRes = &resultString[0];
    send(this->m_socket, charRes, strlen(charRes) + 1, 0);

    return resultString;

}

/**
 * Going to populate a String vector with tokens extracted from the string the client sent.
 * The delimiter will be a ";"
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
}


