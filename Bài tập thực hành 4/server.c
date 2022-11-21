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
#include "exception/exception.h"
#define MAXLINE 1000 

int split(char* buffer, char* only_number, char* only_string)
{
    // Only number in buffer converts to string only_number
    strcpy(only_string,buffer);
	int k=0;
	strcpy(only_number,buffer);
	int j=0;

	// if number, copy to only_number
	// if character, copy to only_string
	int m = 0;
	for(int i=0; i<100 ;i++){
		char ch = only_number[i];
		if (ch == '\0') break;
		if(ch >= '0' && ch <= '9'){
    	    only_number[j] = ch;
    	   	j++;
   		}
   		else if ((ch >= 'a' && ch <= 'z')||(ch == ' ')){
   			only_string[k] = ch;
   			k++;
   		}
   		else{
   			return 0;
   		}
   	}
   	only_number[j] = '\0'; 
	only_string[k] = '\0'; 
	return 1;
}

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
        printf("Buffer: ");
        puts(buffer);
        printf("Only Number: ");
        puts(only_number);
        printf("Only String: ");
        puts(only_string);
    }
    else
    {
        puts("Error");
        strcpy(only_number, "Error");
        strcpy(only_string, "");
    }

    // Sent the response to client
    sendto(listenfd, only_number, MAXLINE, 0, (struct sockaddr*)&client_address, sizeof(client_address));
    sendto(listenfd, only_string, MAXLINE, 0, (struct sockaddr*)&client_address, sizeof(client_address));
}