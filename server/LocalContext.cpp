//Author  : Group#2
//Date    : 03/12/2022
//Version : 2.0
//Filename: LocalContext.cpp

#include "LocalContext.h"

using namespace std;

//Returns number of RPCs for a client connection
int LocalContext::getRpcCount()
{
    return m_RpcCount;
}

//Increments number of RPCs for a client connection by 1
void LocalContext::incrementRpcCount()
{
    m_RpcCount ++;
}