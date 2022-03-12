//Author  : Group#2
//Date    : 03/12/2022
//Version : 2.0
//Filename: GlobalContext.h

#ifndef CPSC5042CLIENTSERVER_GLOBALCONTEXT_H
#define CPSC5042CLIENTSERVER_GLOBALCONTEXT_H

/**
 * A struck that track all the RPCs and connection on the server
 */
struct GlobalContext {
    int g_rpcCount = 0;         //Count of RPCs call
    int g_activeConnection = 0; //Count the active connection on the server
    int g_maxConnection = 0;    //Count the maximum connection on the server
    int g_totalConnection = 0;  //Count the total of connection
};

#endif //CPSC5042CLIENTSERVER_GLOBALCONTEXT_H
