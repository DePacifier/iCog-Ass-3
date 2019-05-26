#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
using namespace std;

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

    ~serverWrapper(){
        close(sockDescriptor);
    }

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
        map<string,int> domainMap = {{"IPV4",AF_INET}, {"IPV6", AF_INET6}, {"LOCAL",AF_UNIX}};
        map<string,int> typeMap = {{"TCP",SOCK_STREAM}, {"UDP", SOCK_DGRAM}};

        if(domainMap.find(entry_domain) != domainMap.end()){
            domain = domainMap[entry_domain];
        }else {
            puts("Please provide either IPV4, IPV6 or LOCAL");
            exit(0);
        }

        if(typeMap.find(entry_type) != typeMap.end()){
            type = typeMap[entry_type];
        }else{
            puts("Please provide either TCP or UDP");
            exit(0);
        }

        // if (entry_domain == "IPV4")
        //     domain = AF_INET;
        // else
        //     domain = AF_INET6;

        // if (entry_type == "TCP")
        //     type = SOCK_STREAM;
        // else
        //     type = SOCK_DGRAM;

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

    void startServer(char * (*func)(char *))
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
            message = func(clientMessage);
            if (write(new_socket, message, strlen(message)) < 0)
            {
                puts("Failed to write message to client");
                exit(0);
            }

            close(new_socket);
        }
    }

    void startServerUDP(char * (*func)(char *))
    {
        puts("Starting server");
        puts("Waiting for incoming connections...");
        int c = sizeof(struct sockaddr_in);
        char clientMessage[1024];
        char * message;
        int len, n;

        while (1)
        {
            if ((n = recvfrom(sockDescriptor, clientMessage, strlen(clientMessage), MSG_WAITALL, (struct sockaddr *)&client, (socklen_t *)&client)) < 0)
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
            message = func(clientMessage);
            if (sendto(sockDescriptor, message, strlen(message), MSG_CONFIRM,(struct sockaddr *)&client, sizeof(client)) < 0)
            {
                // send(sockDescriptor, messageToRespond, 1024, MSG_CONFIRM, (struct sockaddr *)&client, sizeof(client))
                puts("Failed to write message to client");
                //exit(0);
            }
        }
    }

    void startSimpleIPV4(char * (*func) (char *))
    {
        createSocket("IPV4", "TCP", 0);
        assignPropandBindSocket();
        startServer(func);
    }

    void startSimpleIPV4UDP(char * (*func) (char *))
    {
        createSocket("IPV4", "UDP", 0);
        bindSocket();
        startServerUDP(func);
    }
};