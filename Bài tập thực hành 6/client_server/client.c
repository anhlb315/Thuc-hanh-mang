#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include "../exception/exception.h"
#include "../helper/helper.h"
#define BUFFER_SIZE 1024
#define PORT 8080

void func(int socket_fd)
{
	return;
}

int main(int argc, char *argv[])
{
	// Check input
	if (argc != 3)
	{
		printf("Please input IP address and Port number\n");
		return 0;
	}

	char *ip_address = argv[1];
	char *port_number = argv[2];
	int port = atoi(port_number);
	int socket_fd, connfd;
	struct sockaddr_in servaddr;

	// Check if address valid
	if (inet_addr(ip_address) == -1)
	{
		printf("Invalid IP address.\n");
		return 0;
	}

	// Check if port valid
	if (port < 1 || port > 65535)
	{
		printf("Invalid port.\n");
		return 0;
	}

	// socket create and verification
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip_address);
	servaddr.sin_port = htons(port);

	// connect the client socket to server socket
	if (connect(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
	{
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(socket_fd);

	// close the socket
	close(socket_fd);
}
