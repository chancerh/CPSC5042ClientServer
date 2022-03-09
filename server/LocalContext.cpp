
#include "LocalContext.h"

using namespace std;


int LocalContext::getRpcCount()
{
    return m_RpcCount;
}

void LocalContext::setRpcCount(int rpcCount)
{
    m_RpcCount = rpcCount;
}

void LocalContext::incrementRpcCount()
{
    m_RpcCount ++;
}