#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include "exception/exception.h"
#define BUFFER_SIZE 1024
#define PORT 8080

void func(int sockfd)
{
	char username[BUFFER_SIZE];
	char password[BUFFER_SIZE];
	int n;

	for (;;)
	{
	goal0:
		// Clean buffers
		bzero(username, sizeof(username));
		bzero(password, sizeof(password));

		// Get user's input
		printf("Username: ");
		if (fgets(username, sizeof(username), stdin) == NULL)
			break;

		// Check for exception
		if (check_spaces(username, strlen(username)))
		{
			printf("Username contains white space. Please enter again.\n");
			goto goal0;
		}
	goal1:
		printf("Password: ");
		if (fgets(password, sizeof(username), stdin) == NULL)
			break;

		// Check for '\n' input
		if (strcmp(username, "\n") == 0 && strcmp(password, "\n") == 0)
		{
			printf("Exit Program.\n");
			char exit_program[100] = "exit_program\0";
			write(sockfd, exit_program, strlen(exit_program));
			break;
		}

		// Check for scape
		if (check_spaces(password, strlen(password)))
		{
			printf("Password contains white space. Please enter again.\n");
			goto goal1;
		}

		// Send username & password to server
		write(sockfd, username, sizeof(username));
		write(sockfd, password, sizeof(password));

		printf("---------------------\n");
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
	{
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}
