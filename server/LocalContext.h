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
    int getRpcCount()
    {
        return m_RpcCount;
    }

    /**
     * Set the number of RPCs for a specific client
     * @param rpcCount
     */
    void setRpcCount(int rpcCount)
    {
        m_RpcCount = rpcCount;
    }

    /**
     * Increment the RPC count for a specific client
     */
    void incrementRpcCount()
    {
        m_RpcCount ++;
    }

private:
    int m_RpcCount = 0; //Count of RPCs sent by a client
};


#endif //CPSC5042CLIENTSERVER_LOCALCONTEXT_H
