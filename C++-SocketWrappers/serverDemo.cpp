#include "serverWrapper.h"
#include <string>

char *reply(char *value)
{
    if (strcmp(value, "get1") == 0)
    {
        return "You asked one, and Got One";
    }
    else if (strcmp(value, "get2") == 0)
    {
        return "You asked Two, and Got Two";
    }
    else
    {
        return "Hello this is a bad ass message";
    }
}

int main(int argc, char const *argv[])
{
    serverWrapper server = serverWrapper();

    // Ways to use
    // 1 ---
    server.startSimpleIPV4(reply);

    // 2 ---
    //server.startSimpleIPV4UDP(reply);

    // 3 ---
    // server.createSocket("IPV4", "TCP", 0);
    // server.assignPropandBindSocket(INADDR_ANY, 2000, 5);
    // server.startServer(reply);

    //4 ---
    // server.createSocket("IPV4", "UDP", 0);
    // server.bindSocket(INADDR_ANY, 3000, 6);
    // server.startServerUDP(reply);

    //5 ---
    //similar ways for IPV6

    return 0;
}
