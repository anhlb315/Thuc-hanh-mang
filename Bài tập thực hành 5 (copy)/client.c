#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	// Catch wrong input
	if (argc != 3)
	{
		printf("Please input IP address and port number\n");
		return 0;
	}
	
	// Variables
	char *ip_address = argv[1];
	char *port_number = argv[2];
	int port = atoi(port_number);
	int socket_fd = 0;
	struct sockaddr_in serv_addr;
	char buffer[1024];

	// Try catch false when connecting
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	// Client
	while (1)
	{
		fgets(buffer, 1024, stdin);
		send(socket_fd, buffer, strlen(buffer), 0);
	}

	return 0;
}