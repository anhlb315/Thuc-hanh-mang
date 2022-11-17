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
#define MAXLINE 1000 

int main(int argc, char* argv[])
{
    // Catch wrong input
    if(argc == 1)
    {
        printf("Please input port number\n");
        return 0;
    }
    
    char* port_number = argv[1]; 
    int port = atoi(port_number); // Get port from argv
    char buffer[100]; // Data get from client
    char only_string[100]; // Message send to client (string only)
    char only_number[100]; // Message send to client (number only)
    int listenfd, len;
    struct sockaddr_in server_address, client_address;
    bzero(&server_address, sizeof(server_address)); // ???

    // Create a UDP Socket
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
    server_address.sin_family = AF_INET;

    // Bind server address to socket descriptor
    bind(listenfd, (struct sockaddr*)&server_address, sizeof(server_address));

    // Receive the datagram
    len = sizeof(client_address);
    int n = recvfrom(listenfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_address, &len); // Receive message from server
    buffer[n] = '\0';

    // Split data to only string and only number
    if (split(buffer, only_number, only_string) == 1)
    {
        //puts("Success");
    }
    else
    {
        //puts("Error");
        strcpy(only_number, "Error");
        strcpy(only_string, "");
    }

    // Sent the response to client
    sento(listenfd, only_number, MAXLINE, 0, (struct sockaddr*)&client_address, sizeof(client_address));
    sento(listenfd, only_string, MAXLINE, 0, (struct sockaddr*)&client_address, sizeof(client_address));
}