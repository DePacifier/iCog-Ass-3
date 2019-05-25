#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int createSocket(int domain, int type, int protocol)
{
    int sock = 0;
    sock = socket(domain, type, protocol);
    if (sock == -1)
    {
        puts("Failed to create Socket \n");
    }
    else
    {
        puts("Socket created successfully\n");
        return sock;
    }
}

struct sockaddr_in assignPropandBindSocket(int sockDescriptor, struct sockaddr_in server, int domain, int address, int portNumber, int maxConn)
{
    server.sin_family = domain;
    server.sin_addr.s_addr = htonl(address);
    server.sin_port = htons(portNumber);

    if (bind(sockDescriptor, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        puts("Failed to bind the socket \n");
        exit(0);
    }
    puts("bind done");

    if (listen(sockDescriptor, maxConn) == -1)
    {
        puts("Failed to intitiate socket to listen \n");
        exit(0);
    }
    puts("listen done");

    return server;
}

void bindSocket(int sockDescriptor, struct sockaddr_in server, int maxConn)
{
}

void startServer(int sockDescriptor, char *messageToRespond)
{
    puts("Starting server");
    puts("Waiting for incoming connections...");
    int c = sizeof(struct sockaddr_in);
    struct sockaddr_in client;
    int new_socket;
    char *message, clientMessage[2000];
    ;

    while (1)
    {
        new_socket = accept(sockDescriptor, (struct sockaddr *)&client, (socklen_t *)&c);
        if (new_socket < 0)
        {
            perror("accept failed");
            //close(sockDescriptor);
            close(new_socket);
            exit(0);
        }
        puts("Connection accepted");
        char *client_ip = inet_ntoa(client.sin_addr);
        int client_port = ntohs(client.sin_port);
        /*if(read(new_socket, clientMessage, 2000) < 0){
            puts("Failed to recieve clients message");
            exit(0);
        }*/
        //puts("Message from clinet : \n");
        //puts(clientMessage);

        puts("Replying to request .... \n");
        if (write(new_socket, messageToRespond, strlen(messageToRespond)) < 0)
        {
            puts("Failed to write message to client");
            exit(0);
        }

        close(new_socket);
    }
}
/*
void startServerIPV6(int sockDescriptor, char *messageToRespond)
{
    puts("Starting server");
    puts("Waiting for incoming connections...");
    int c = sizeof(struct sockaddr_in6);
    struct sockaddr_in6 client;
    int new_socket;
    char *message, clientMessage[2000];
    ;

    while (1)
    {
        new_socket = accept(sockDescriptor, (struct sockaddr *)&client, (socklen_t *)&c);
        if (new_socket < 0)
        {
            perror("accept failed");
            close(sockDescriptor);
            close(new_socket);
            exit(0);
        }
        puts("Connection accepted");
        inet_ntop(AF_INET6, &(cli_addr.sin6_addr),client_addr_ipv6, 100);
        printf("Incoming connection from client having IPv6 address: %s\n",client_addr_ipv6);
       
        puts("Replying to request .... \n");
        
        // n = recv(newsockfd, buffer, 255, 0);
        // if (n < 0)
        //     error("ERROR reading from socket");

        // printf("Message from client: %s\n", buffer);

        int n = send(new_socket, messageToRespond, strlen(messageToRespond), 0);
        if (n < 0)
            error("ERROR writing to socket");
        
        //Sockets Layer Call: close()
        close(sockDescriptor);
        close(newsockfd);
    }
}
*/

void startSimpleIPV4(char * messageToReplay){
    int socket = 0;
    struct sockaddr_in server; 

    socket = createSocket(AF_INET, SOCK_STREAM, 0);
    server = assignPropandBindSocket(socket, server, AF_INET, INADDR_ANY, 6000, 10);
    startServer(socket, messageToReplay);
}


/*void startSimpleIPV6(char * messageToReplay){
    int socket = 0;
    struct sockaddr_in6 server;

    socket = createSocket(AF_INET6, SOCK_STREAM, 0);
    server = assignPropandBindSocket(socket, AF_INET6, IN6ADDR_ANY_INIT, 7000, 10);
    startServer(socket, "")

}*/