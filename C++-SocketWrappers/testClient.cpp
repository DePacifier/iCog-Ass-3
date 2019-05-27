#include <iostream>
#include"client.h"
using namespace std;


int main(int argc, char *argv[])
{
    client c("tcp");
    string host;

    cout << "Enter hostname or address : ";
    cin >> host;

    //connect to host
    c.connv4(host, 5000);

    //send some data
    c.send_data();

    //receive and echo reply
    cout << "----------------------------\n\n";
    cout << c.receive(1024);
    cout << "\n\n----------------------------\n\n";

    //done
    return 0;
}