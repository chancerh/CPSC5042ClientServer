//
// Created by Aacer on 3/4/2022.
//

#ifndef CPSC5042CLIENTSERVER_GLOBALCONTEXT_H
#define CPSC5042CLIENTSERVER_GLOBALCONTEXT_H

struct GlobalContext {
    int g_rpcCount = 0;
    int g_activeConnection = 0;
    int g_maxConnection = 0;
    int g_totalConnection = 0;
};

#endif //CPSC5042CLIENTSERVER_GLOBALCONTEXT_H
