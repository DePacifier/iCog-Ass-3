#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

class serverWrapper
{
private:
    int domain;
    int type;
    int protocol;
    int sockDescriptor;
    struct sockaddr_in server, client;
    int address;
    int portNumber;
    int maxConn;

public:
    int getDomain()
    {
        return domain;
    }

    int getType()
    {
        return type;
    }

    int getProtocol()
    {
        return protocol;
    }

    int getAddress()
    {
        return address;
    }

    int getPortnumber()
    {
        return portNumber;
    }

    int getMaxConectionsAllowed()
    {
        return maxConn;
    }

    void createSocket(char *entry_domain = "IPV4", char *entry_type = "TCP", int entry_protocol = 0)
    {
        if (entry_domain == "IPV4")
            domain = AF_INET;
        else
            domain = AF_INET6;

        if (entry_type == "TCP")
            type = SOCK_STREAM;
        else
            type = SOCK_DGRAM;

        protocol = entry_protocol;

        sockDescriptor = socket(domain, type, protocol);
        if (sockDescriptor == -1)
            puts("Failed to create socket");
        else
            puts("Socket created successfully\n");
    }

    void bindSocket(int address = INADDR_ANY, int portNumber = 6000)
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
    }

    void assignPropandBindSocket(int address = INADDR_ANY, int portNumber = 5000, int maxConn = 10)
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
    }

    void startServer(char *messageToRespond)
    {
        puts("Starting server");
        puts("Waiting for incoming connections...");
        int c = sizeof(struct sockaddr_in);
        int new_socket;
        char *message, clientMessage[2000];

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
            printf("Connection From:\n>> Client IP : %s\n>> On Port : %d\n", client_ip, client_port);

            if (read(new_socket, clientMessage, 2000) < 0)
            {
                puts("Failed to recieve clients message");
                //exit(0);
            }
            else
            {
                puts("Message from client :");
                printf(">> %s\n", clientMessage);
            }

            puts("Replying to request .... \n");
            if (write(new_socket, messageToRespond, strlen(messageToRespond)) < 0)
            {
                puts("Failed to write message to client");
                exit(0);
            }

            close(new_socket);
        }
    }

    void startServerUDP(char *messageToRespond)
    {
        puts("Starting server");
        puts("Waiting for incoming connections...");
        int c = sizeof(struct sockaddr_in);
        char clientMessage[1024];
        int len, n;

        while (1)
        {
            if ((n = recvfrom(sockDescriptor, clientMessage, 1024, MSG_WAITALL, (struct sockaddr *)&client, (socklen_t *)&client)) < 0)
            {
                puts("Failed to recieve from client\n");
                exit(0);
            }

            clientMessage[n] = '\0';
            puts("Message from client :");
            char *client_ip = inet_ntoa(client.sin_addr);
            int client_port = ntohs(client.sin_port);
            printf("Connection From:\n>> Client IP : %s\n>> On Port : %d\n", client_ip, client_port);
            printf(">> %s\n", clientMessage);

            puts("Replying to request .... \n");
            if (send(sockDescriptor, messageToRespond, 1024, MSG_CONFIRM) < 0)
            {
                // send(sockDescriptor, messageToRespond, 1024, MSG_CONFIRM, (struct sockaddr *)&client, sizeof(client))
                puts("Failed to write message to client");
                //exit(0);
            }
        }
    }

    void startSimpleIPV4(char *messageToReplay)
    {
        createSocket("IPV4", "TCP", 0);
        assignPropandBindSocket();
        startServer(messageToReplay);
    }

    void startSimpleIPV4UDP(char *messageToReplay)
    {
        createSocket("IPV4", "UDP", 0);
        bindSocket();
        startServerUDP(messageToReplay);
    }
};