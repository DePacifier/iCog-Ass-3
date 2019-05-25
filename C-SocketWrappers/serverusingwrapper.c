#include <stdio.h>
#include <stdlib.h>
#include "ServerSocket.h"

int main(int argc, char const *argv[])
{
    //Way to use 1
    startSimpleIPV4("hello there\n");

    //Way to use 2
    /*int listenfd = 0;
    struct sockaddr_in server; 

    listenfd = createSocket(AF_INET, SOCK_STREAM, 0);

    server = assignPropandBindSocket(listenfd, server, AF_INET, INADDR_ANY, 3000, 10);

    startServer(listenfd, "hello boy");*/

    return 0;
}