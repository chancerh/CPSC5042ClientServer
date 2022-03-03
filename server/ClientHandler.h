//Author  : Group#2
//Date    : 02/07/2022
//Version : 2.0
//Filename: ClientHandler.h

#ifndef CPSC5042CLIENTSERVER_CLIENTHANDLER_H
#define CPSC5042CLIENTSERVER_CLIENTHANDLER_H

#include "RPCServer.h"

class ClientHandler{
public:
    ClientHandler(int socket);
    ~ClientHandler();

    /**
    * Processes incoming RPCs by parsing the input arguments and calling the
    * appropriate functions
    *
    * @return Always TRUE
    */
    bool ProcessRPC();

private:
    int m_socket; //socket number
    bool m_authenticated; //flag to track if client provided correct credentials and is logged in
    unordered_map<string,string> m_users; //map storing all username and
    // password pairs

    //SUPPORTED RPCs
    //Conversion parted added and fixed by Jusmin 3/2/22
    const string CONNECT = "connect",
            DISCONNECT = "disconnect",
            CALC_EXPR = "calculateExpression",
            CALC_BINTODEC = "binToDec",
            CALC_DECTOBIN = "decToBin",
            CALC_HEXTOBIN = "hexToBin",
            CALC_BINTOHEX = "binToHex";

    //RPC Status
    const string SUCCESS = "0;",
            GENERAL_FAIL = "-1;";


    /**
     * The ProcessConnectRPC() function process a connect RPC by validating the input credentials. If input credentials
     * are correct, then m_authenticated is flipped to true
     * @param arrayTokens A vector containing the tokens of incoming RPC. arrayTokens[1] contains username,
     *                      arrayTokens[2] contains the password
     * @return A boolean indicating if the input credentials are correct
     */
    bool ProcessConnectRPC(vector<std::string>& arrayTokens);

    /**
     *
     *
     * @param arrayTokens
     */
    void ProcessCal(vector<std::string> &arrayTokens);

    /**
     * The processDisconnectRPC() disconnects the client connection from the server
     * @return Always returns true
     */
    bool ProcessDisconnectRPC();

    /**
    * The ProcessCalcExp() function processes a request to calculate an
    * algebraic expression, calculates the expression and returns the result
    * @param arrayTokens A vector containing the tokens of incoming RPC.
    *                      arrayTokens[1] contains the mathematical expression
    * @return A boolean indicating if the input credentials are correct
    */
    void ProcessCalcExp(vector<std::string>& arrayTokens);

    /**
     * The ParseTokens() function processes an input char array buffer containing the RPC parameters, and parses it
     * into a string vector. The parsing of the buffer relies on ';' as the delimiter
     * @param buffer Input char array buffer to be parsed, with ';' as delimeter between parameters
     * @param a A string vector containing the parsed RPC parameters
     */
    void ParseTokens(char* buffer, std::vector<std::string>& a);

    void HexConversionRPC(vector<std::string> &arrayTokens);

    string ProcessStatSummary(vector<std::string> &arrayTokens);

    //Helper function to send buffers back to client
    void sendBuffer(char *szBuffer) const;

};


#endif //CPSC5042CLIENTSERVER_CLIENTHANDLER_H
