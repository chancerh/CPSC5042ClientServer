//Author  : Group#2
//Date    : 03/12/2022
//Version : 2.0
//Filename: LocalContext.h

#ifndef CPSC5042CLIENTSERVER_LOCALCONTEXT_H
#define CPSC5042CLIENTSERVER_LOCALCONTEXT_H


/**
 * This class maintains local context for each client connecting to server
 */
class LocalContext
{
public:

    /**
     * Get the number of RPCs
     * @return RPC count for the specific client
     */
    int getRpcCount();

    /**
     * Increment the RPC count for a specific client
     */
    void incrementRpcCount();

private:
    int m_RpcCount = 0; //Count of RPCs sent by a client
};


#endif //CPSC5042CLIENTSERVER_LOCALCONTEXT_H
