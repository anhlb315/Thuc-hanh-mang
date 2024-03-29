#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include "app/app.h"

int main(int argc, char *argv[])
{
	// Check input
	if (argc != 3)
	{
		printf("Error: Please input IP address and Port number\n");
		return 0;
	}

	char *ip_address = argv[1];
	char *port_number = argv[2];
	int port = atoi(port_number);
	int socket_fd;
	struct sockaddr_in server_address;

	// Check if address valid
	if (inet_addr(ip_address) == -1)
	{
		printf("Error: Invalid IP address\n");
		return 0;
	}

	// Check if port valid
	if (port < 1 || port > 65535)
	{
		printf("Error: Invalid port\n");
		return 0;
	}

	// socket create and verification
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		printf("Error: Socket creation failed\n");
		return 0;
	}
	else
		printf("Socket successfully created\n");

	// assign IP, PORT
	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(ip_address);
	server_address.sin_port = htons(port);

	// connect the client socket to server socket
	if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) != 0)
	{
		printf("Error: Connection with the server failed\n");
		return 0;
	}
	else
		printf("Connected to the server\n");

	// function for chat
	if (client_app(socket_fd) == 0)
	{	
		printf("!!!Error: client_app\n");
		return 0;
	}

	// close the socket
	close(socket_fd);
	printf("Exit program\n");

	return 0;
}
