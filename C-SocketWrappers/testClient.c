#include <stdio.h>
#include "client.h"
int main(int argc, char *argv[])
{

	struct sockaddr_in server;
	int fd = 0;

	fd = createSocket(AF_INET, SOCK_STREAM, 0);
	connectSocket(fd, server, AF_INET, "192.168.1.104", 6003);
}