#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <signal.h>
#include <arpa/inet.h>
#include <errno.h>
#include "app/app.h"

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
    char client_address_str[LARGE];

    if (port < 1 || port > 65535)
    {
        printf("Error: Invalid port\n");
        return 0;
    }

    // socket create and verification
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
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
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return 0;
    }
    else
        printf("Socket successfully bind\n");

    // Now server is ready to listen and verification
    if ((listen(socket_fd, 5)) != 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
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
            fprintf(stderr, "[-]%s\n", strerror(errno));
            return 0;
        }
        else
        {
            inet_ntop(AF_INET, &client_address, client_address_str, INET_ADDRSTRLEN);
            printf("Server accept the client: %s\n", client_address_str);
        }

        if ((pid = fork()) == 0)
        {
            close(socket_fd);
            server_app(connect_fd);
            close(connect_fd);
            exit(0);
        }
        else if (pid > 0)
        {
            printf("Child %d created\n", pid);
        }
        else
        {
            printf("Failed to create child.\n");
        }
    }

    // Close the socket
    printf("Server is closing...\n");
    close(socket_fd);
    printf("Server's closed\n");

    return 0;
}