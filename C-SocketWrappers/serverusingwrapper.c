#include <stdio.h>
#include <stdlib.h>
#include "ServerSocket.h"

int main(int argc, char const *argv[])
{
    /*int listenfd = 0, connfd = 0;
    struct sockaddr_in server; 

    //char sendBuff[200000];
    //char message[] = "I have recieved\n";

    listenfd = createSocket(AF_INET, SOCK_STREAM, 0);
    //memset(&server, '0', sizeof(server));
    //memset(sendBuff, '0', sizeof(sendBuff));

    server = assignPropandBindSocket(listenfd, server, AF_INET, INADDR_ANY, 3000, 10);

    startServer(listenfd, "hello boy");*/

    startSimpleIPV4("hello there");

    return 0;
}