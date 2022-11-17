// Client-side
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h>
#include <strings.h> 
#define MAXLINE 1000 

int main(int argc, char* argv[])
{
    // Check input
    if (argc != 3)
    {
        printf("Please input IP address and Port number\n");
        return 0;
    }

    char* ip_address = argv[1]; // Get IP from argv
    char* port_number = argv[2]; // Get Port number from argv
    int port = atoi(port_number); // int-type Port number

    int sockfd, n; // ???
    struct sockaddr_in server_address;

    // Clear server_address
    bzero(&server_address, sizeof(server_address));
    server_address.sin_addr.s_addr = inet_addr(ip_address);
    server_address.sin_port = htops(port);
    server_address.sin_family = AF_INET;

    // Create datagram socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Connect to server
    if(connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        printf("\nError: Connect failed.\n");
        exit(0);
    }

    do {
        int g = 0; // ???
        char buffer[100];
        char message[100];

        // Get message
        printf("Input data: ");
        g = scanf("%[^\n]", message); // ???
        if (g == 0) break;
        getchar();
        // Request to send datagram ???
        // No need to specify server address in sendto
        // Connect stores the peers IP and port
        sento(sockfd, message, MAXLINE, 0, (struct sockaddr*)&server_address, sizeof(server_address));

        // Waiting for response
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct scokaddr*)NULL, NULL);
        puts(buffer);
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
        puts(buffer);
        puts("------------------");
    } while(1);

    // Close the descriptor
    close(sockfd);

    return 0;
}