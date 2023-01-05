#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h> // read(), write(), close()
#include <pthread.h>
#include <errno.h>
#include "server_app/server_app.h"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    // Catch wrong input
    if (argc != 2)
    {
        printf("[-]Please input port number\n");
        return 0;
    }

    char *port_number = argv[1];
    int port = atoi(port_number);
    struct sockaddr_in server_address, client_address;
    int socket_fd, connect_fd, len = sizeof(client_address);
    bzero(&server_address, sizeof(server_address));
    bzero(&client_address, sizeof(client_address));
    fd_set current_sockets, read_sockets, write_sockets, exception_sockets;
    int socket_count = 0;
    char signal[BUFFER_SIZE];
    char feedback[BUFFER_SIZE];
    Account *acc = NULL;
    acc = read_account(acc);
    Account current_user;

    if (port < 1 || port > 65535)
    {
        printf("[-]Invalid port\n");
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
        printf("[+]Socket successfully created\n");

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
        printf("[+]Socket successfully bind\n");

    // Now server is ready to listen and verification
    if ((listen(socket_fd, 5)) != 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return 0;
    }
    else
        printf("[+]Server listening\n");

    // Initialize current set
    FD_ZERO(&current_sockets);
    FD_SET(socket_fd, &current_sockets);
    socket_count = socket_fd + 1;

    while (1)
    {
        // Because select is destructive
        read_sockets = current_sockets;
        write_sockets = current_sockets;

        printf("[+]Socket count: %d\n", socket_count);
        if (select(socket_count, &read_sockets, &write_sockets, NULL, NULL) < 0)
        {
            fprintf(stderr, "[-]%s\n", strerror(errno));
            return 0;
        }

        for (int i = 0; i <= socket_count; i++)
        {
            if (FD_ISSET(i, &read_sockets))
            {
                if (i == socket_fd)
                {
                    // There is a new connection that we can accept
                    connect_fd = accept(socket_fd, (struct sockaddr *)&client_address, &len);
                    if (connect_fd < 0)
                    {
                        fprintf(stderr, "[-]%s\n", strerror(errno));
                        return 0;
                    }
                    else
                    {
                        printf("[+]Server accept the client_address\n");
                        FD_SET(connect_fd, &current_sockets);
                        if (connect_fd >= socket_count)
                        {
                            socket_count = connect_fd + 1;
                        }
                    }
                }
                else
                {
                    // Recv client's signal
                    if (recv(i, signal, sizeof(signal), MSG_WAITALL) < 0)
                    {
                        fprintf(stderr, "[-]%s\n", strerror(errno));
                        // Send feedback to Client
                        sprintf(feedback, "%d", 0);
                        if (send(i, feedback, sizeof(feedback), 0) < 0)
                        {
                            fprintf(stderr, "[-]%s\n", strerror(errno));
                        }
                        break;
                    }
                    else
                    {
                        // Send feedback to Client
                        sprintf(feedback, "%d", 1);
                        if (send(i, feedback, sizeof(feedback), 0) < 0)
                        {
                            fprintf(stderr, "[-]%s\n", strerror(errno));
                            break;
                        }
                        else
                        {
                            switch (atoi(signal))
                            {
                            case 0:
                                printf("[+]Sign in\n");
                                sign_in(i, acc, &current_user);
                                break;
                            case 1:
                                printf("[+]Change password\n");
                                change_password(i, acc, &current_user);
                                break;
                            case 2:
                                printf("[+]Exit program\n");
                                // Clear client_fd out of current_sockets
                                FD_CLR(i, &current_sockets);
                                break;
                            default:
                                printf("[-]Something wrong with server\n");
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}