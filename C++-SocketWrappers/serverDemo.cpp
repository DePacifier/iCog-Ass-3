#include "serverWrapper.h"

int main(int argc, char const *argv[])
{
    serverWrapper server = serverWrapper();

    server.startSimpleIPV4("I am a bad ass\n");

    //server.startSimpleIPV4UDP("Another badass");

    return 0;
}
