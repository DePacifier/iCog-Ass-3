#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int createSocket(int domain, int type, int protocol)
{
    //Create socket
    int sock;
    sock = socket(domain, type, protocol);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    else
    {
        puts("Socket created\n");
        return sock;
    }
}

void connectSocket(int sockDescriptor, struct sockaddr_in server, int domain, char *address, int portNumber)
{
    char server_reply[10000], message[2000];
    if (inet_pton(AF_INET, address, &server.sin_addr) <= 0)
    {
        printf("\n inet_pton error occured\n");
        exit(0);
    }
    server.sin_family = domain;
    server.sin_port = htons(portNumber);

    //Connect to remote server
    if (connect(sockDescriptor, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        exit(0);
    }
    else
    {
        puts("Connected\n");
        while (1)
        {

            printf("Enter message : ");
            scanf("%s", message);
            //Send some data
            if (send(sockDescriptor, message, sizeof(message), 0) < 0)
            {
                puts("Send failed");
                exit(0);
            }

            //Receive a reply from the server
            if (recv(sockDescriptor, server_reply, sizeof(server_reply), 0) < 0)
            {
                puts("No message from server");
                exit(0);
            }

            puts("Server reply :");
            puts(server_reply);
        }
    }
}