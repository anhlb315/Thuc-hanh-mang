// Server-side
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "exception.h"
#define MAXLINE 1000

int split(char *buffer, char *only_number, char *only_string)
{
    // Only number in buffer converts to string only_number
    strcpy(only_string, buffer);
    int k = 0;
    strcpy(only_number, buffer);
    int j = 0;

    // if number, copy to only_number
    // if character, copy to only_string
    int m = 0;
    for (int i = 0; i < 100; i++)
    {
        char ch = only_number[i];
        if (ch == '\0')
            break;
        if (ch >= '0' && ch <= '9')
        {
            only_number[j] = ch;
            j++;
        }
        else if ((ch >= 'a' && ch <= 'z') || (ch == ' '))
        {
            only_string[k] = ch;
            k++;
        }
        else
        {
            return 0;
        }
    }
    only_number[j] = '\0';
    only_string[k] = '\0';
    return 1;
}

int main(int argc, char *argv[])
{
    // Catch wrong input
    if (argc == 1)
    {
        printf("Please input port number\n");
        return 0;
    }

    char *port_number = argv[1];
    int port = atoi(port_number); // Get port from argv
    char username_buffer[100];    // Data username from client
    char password_buffer[100];    // Data password from client
    int listenfd, len;
    struct sockaddr_in server_address, client_address;
    bzero(&server_address, sizeof(server_address)); // ???

    // Create a UDP Socket
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
    server_address.sin_family = AF_INET;

    // Bind server address to socket descriptor
    bind(listenfd, (struct sockaddr *)&server_address, sizeof(server_address));

    do
    { // Receive the datagram
        len = sizeof(client_address);
        int n = recvfrom(listenfd, username_buffer, sizeof(username_buffer), 0, (struct sockaddr *)&client_address, &len); // Receive username from server
        username_buffer[n] = '\0';
        n = recvfrom(listenfd, password_buffer, sizeof(password_buffer), 0, (struct sockaddr *)&client_address, &len); // Receive password from server
        password_buffer[n] = '\0';

        printf("Username: ");
        puts(username_buffer);
        printf("Password: ");
        puts(password_buffer);

        // Sent the response to client
        sendto(listenfd, username_buffer, MAXLINE, 0, (struct sockaddr *)&client_address, sizeof(client_address));
        sendto(listenfd, password_buffer, MAXLINE, 0, (struct sockaddr *)&client_address, sizeof(client_address));
    } while (1);
}