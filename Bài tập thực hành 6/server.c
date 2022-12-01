#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include "exception/exception.h"
#include "account/account.h"
#define BUFFER_SIZE 1024
#define PORT 8080

void func(int connfd)
{
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    int n;
    char sign_in_feedback[BUFFER_SIZE];
    Account *acc = NULL;
    acc = read_account(acc);
    int feedback;
    int password_incorrect_times = 3;
    char bye[100] = "bye\0";
    char is_password_changing[BUFFER_SIZE];
    char only_number[BUFFER_SIZE];
    char only_string[BUFFER_SIZE];
    char exit_program[100] = "exit_program\0";

    for (;;)
    {
        printf("Main function.\n");
        printf("---------------------\n");
    }
}

// Driver function
int main(int argc, char *argv[])
{
    // Catch wrong input
    if (argc != 2)
    {
        printf("Please input port number\n");
        return 0;
    }

    char *port_number = argv[1];
    int port = atoi(port_number);
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    if (port < 1 || port > 65535) {
		printf("Invalid port.\n");
		return 0;
	}

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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
    if (connfd < 0)
    {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    // Function for chatting between client and server
    func(connfd);

    // After chatting close the socket
    printf("Closing server...\n");
    close(sockfd);
    printf("Server's closed.\n");
}
