#include <iostream>
#include <stdio.h> 
#include <unistd.h>
#include <string.h> 
#include <string>  
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netdb.h>     

using namespace std;


class client
{
private:
    int sock;
    int type = 0;
    std::string address;
    int port;
    struct sockaddr_in server;
    struct sockaddr_in6 server6;
   
public:
    client(string);
    ~client();
    bool connv4(string, int);
    bool connv6(string,int);
    bool send_data();
    string receive(int);
};

client::client(string trans)
{
    if (trans.compare("tcp")){
        type = 1;
    }
    if(trans.compare("udp")){
        type = 2;
    }
    sock = -1;
    port = 0;
    address = "";
}


bool client::connv4(string address, int port)
{
    //Error message if the type of connection is unspecified
    if(type <= 0 ){         
        perror("Unscpecified type of connection");
    }
    //create socket if it is not already created
    if (sock == -1)
    {
        //Create socket
        sock = socket(AF_INET, type, 0);
        if (sock == -1)
        {
            perror("Could not create socket");
        }

        cout << "Socket created\n";
    }
    else
    { 
    }

    //resolve the hostname to find the IP address
    if (inet_addr(address.c_str()) == -1)
    {
        struct hostent *he;
        struct in_addr **addr_list;

        if ((he = gethostbyname(address.c_str())) == NULL)
        {
            
            herror("gethostbyname");
            cout << "Failed to resolve hostname\n";

            return false;
        }

        
        addr_list = (struct in_addr **)he->h_addr_list;

        for (int i = 0; addr_list[i] != NULL; i++)
        {
           
            server.sin_addr = *addr_list[i];

            cout << address << " resolved to " << inet_ntoa(*addr_list[i]) << endl;

            break;
        }
    }

    //The IP address is given convert to binary
    else
    {
        server.sin_addr.s_addr = inet_addr(address.c_str());
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    //Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    cout << "Connected\n";
    return true;
}



bool client::connv6(string address, int port)
{
    //Error message if the type of connection is unspecified
    if(type <= 0 ){         
        perror("Unscpecified type of connection");
    }
    //create socket if it is not already created
    if (sock == -1)
    {
        //Create socket
        sock = socket(AF_INET6, type, 0);
        if (sock == -1)
        {
            perror("Could not create socket");
        }

        cout << "Socket created\n";
    }
    else
    { 
    }

    //
    if (inet_addr(address.c_str()) == -1)
    {
        perror("Incorrect address input");
    }

    //plain ip address
    else
    {
        //server6.sin6_addr.s6_addr = inet_addr(address.c_str());
        if(inet_pton(AF_INET6,address.c_str(),&server6.sin6_addr) <= 0){
            perror("ip address");
        }
    }

    server6.sin6_family = AF_INET6;
    server6.sin6_port = htons(port);

    //Connect to remote server
    if (connect(sock, (struct sockaddr *)&server6, sizeof(server6)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    cout << "Connected\n";
    return true;
}



bool client::send_data()
{
    //Send some data
    string message = "";
    cout << "Enter message: \n";
    cin >> message;
    if (send(sock, message.c_str(), strlen(message.c_str()), 0) < 0)
    {
        perror("Send failed : ");
        return false;
    }
    cout << "Data sent\n";

    return true;
}


string client::receive(int size = 512)
{
    char buffer[size];
    string reply;

    //Receive a reply from the server
    if (recv(sock, buffer, sizeof(buffer), 0) < 0)
    {
        puts("recv failed");
    }

    reply = buffer;
    return reply;
}

//Destructor to close socket and free the port
client::~client(){
    close(sock);
}