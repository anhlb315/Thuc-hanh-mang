#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include "../exception/exception.h"
#include "../account/account.h"
#include "../helper/helper.h"
#include <signal.h>
#define BUFFER_SIZE 1024
#define PORT 8080

void func(int connect_fd)
{
    char message[BUFFER_SIZE];
    char exit[BUFFER_SIZE] = "exit\0";

    // Chat
    while (1)
    {
        recv(connect_fd, message, sizeof(message), 0);
        standardize_input(message, sizeof(message));

        // Check for exit
        if (strcmp(message, exit) == 0)
            break;

        printf("> %s\n", message);
        bzero(message, sizeof(message));
    }
    return;
}

// Driver function
int main(int argc, char *argv[])
{
    // Catch wrong input
    if (argc != 2)
    {
        printf("Error: Please input port number\n");
        return 0;
    }

    char *port_number = argv[1];
    int port = atoi(port_number);
    int socket_fd, connect_fd, client_address_size;
    struct sockaddr_in server_address, client_address;
    int pid;
    int child_counter = 0;
    char client_address_str[BUFFER_SIZE];

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
    bzero(&server_address, sizeof(server_address));

    // assign IP, PORT
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    // Binding newly created socket to given IP and verification
    if ((bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address))) != 0)
    {
        printf("Error: Socket bind failed\n");
        return 0;
    }
    else
        printf("Socket successfully bind\n");

    // Now server is ready to listen and verification
    if ((listen(socket_fd, 5)) != 0)
    {
        printf("Error: Listen failed\n");
        return 0;
    }
    else
        printf("Server listening...\n");
    client_address_size = sizeof(client_address);

    // Handling SIGCHLD Signals
    signal(SIGCHLD, sig_chld);

    // Accept the data packet from client_address and verification
    while (1)
    {
        connect_fd = accept(socket_fd, (struct sockaddr *)&client_address, &client_address_size);
        if (connect_fd < 0)
        {
            printf("Error: Server accept failed\n");
            return 0;
        }
        else
        {
            inet_ntop(AF_INET, &client_address, client_address_str, INET_ADDRSTRLEN);
            printf("Server accept the client: %s\n", client_address_str);
        }
        
        switch (pid = fork())
        {
        case -1:
            close(connect_fd);
            break;
        case 0:
            close(socket_fd);
            func(connect_fd);
            close(connect_fd);
            exit(0);
            break;
        default:
            printf("pid: %d\n", pid);
            pause();
            break;
        }
    }

    // After chatting close the socket
    printf("Server is closing...\n");
    close(socket_fd);
    printf("Server's closed\n");

    return 0;
}
