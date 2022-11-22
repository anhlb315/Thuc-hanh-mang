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
#include <ctype.h>
#include "exception/exception.h"
#define MAXLINE 1000

int main(int argc, char *argv[])
{
    // Check input
    if (argc != 3)
    {
        printf("Please input IP address and Port number\n");
        return 0;
    }

    char *ip_address = argv[1];   // Get IP from argv
    char *port_number = argv[2];  // Get Port number from argv
    int port = atoi(port_number); // int-type Port number

    struct sockaddr_in server_address;

    // Clear server_address
    bzero(&server_address, sizeof(server_address));
    server_address.sin_addr.s_addr = inet_addr(ip_address);
    server_address.sin_port = htons(port);
    server_address.sin_family = AF_INET;
    int n; // ???

    // Create datagram socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        printf("\nError: Connect failed.\n");
        exit(0);
    }

    do
    {
        char buffer[100];
        char username[100];
        char password[100];
        char only_number[100];
        char only_string[100];

    goal0:
        // Get username & password
        printf("Username: ");
        if (fgets(username, sizeof(username), stdin) == NULL)
            break;

        // Check for scape
        if (check_spaces(username, strlen(username)))
        {
            printf("Username contains white space. Please enter again.\n\n");
            goto goal0;
        }

        printf("Password: ");
        if (fgets(password, sizeof(password), stdin) == NULL)
            break;

        // Check for '\n' input
        if (strcmp(username, "\n") == 0 && strcmp(password, "\n") == 0)
        {
            printf("Exit Program.\n");
            char exit_program[100] = "exit_program\0";
            sendto(sockfd, exit_program, MAXLINE, 0, (struct sockaddr *)&server_address, sizeof(server_address));
            break;
        }

        // Check for scape
        if (check_spaces(password, strlen(password)))
        {
            printf("Password contains white space. Please enter again.\n\n");
            goto goal0;
        }

        // Request to send datagram ???
        // No need to specify server address in sendto
        // Connect stores the peers IP and port
        sendto(sockfd, username, MAXLINE, 0, (struct sockaddr *)&server_address, sizeof(server_address));
        sendto(sockfd, password, MAXLINE, 0, (struct sockaddr *)&server_address, sizeof(server_address));

        // Waiting for response
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
        switch (atoi(buffer))
        {
        case 0:
            char choice[100];

            printf("OK.\n");
            printf("------------------\n");
        goal1:
            printf("Do you want to change password or sign out?(y/n/bye): ");
            if (fgets(choice, sizeof(choice), stdin) == NULL)
                break;

            char bye[100] = "bye\n\0";
            if (choice[0] != 121 && choice[0] != 110 && !(strcmp(choice, bye) == 0))
            {
                printf("Wrong input. Only y or n.\n");
                goto goal1;
            }

            if (choice[0] == 121)
            {
                char new_password[100];
            goal2:
                printf("New password: ");
                if (fgets(new_password, sizeof(new_password), stdin) == NULL)
                    break;

                // Check new_password
                if (check_new_password(new_password))
                {
                    printf("Can only contains number or letter. Try again please.\n");
                    goto goal2;
                }

                char confirm_password[100];
            goal3:
                printf("Confirm password: ");
                if (fgets(confirm_password, sizeof(confirm_password), stdin) == NULL)
                    break;

                // Check confirm_password
                if (check_confirm_password(confirm_password, new_password))
                {
                    goto goal3;
                }

                sendto(sockfd, confirm_password, MAXLINE, 0, (struct sockaddr *)&server_address, sizeof(server_address));
                recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
                recvfrom(sockfd, only_number, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
                recvfrom(sockfd, only_string, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
                if (atoi(buffer) == 0)
                {
                    printf("Encoded password: %s %s\n", only_number, only_string);
                    printf("Change password successfully.\n");
                }
            }
            else if (choice[0] == 110)
            {
                char request[2] = "0";
                sendto(sockfd, request, MAXLINE, 0, (struct sockaddr *)&server_address, sizeof(server_address));
            }
            else
            {
                char sign_out_request[100] = "bye\0";
                sendto(sockfd, sign_out_request, MAXLINE, 0, (struct sockaddr *)&server_address, sizeof(server_address));
                recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
                if (strcmp(sign_out_request, buffer)==0)
                {
                    printf("Sign out successfully.\n");
                }
            }
            break;
        case 1:
            printf("Cannot find account.\n");
            break;
        case 2:
            printf("Account is not ready.\n");
            break;
        case 3:
            printf("Not OK.\n");
            break;
        case 4:
            printf("Not OK. Account is blocked.\n");
            break;
        default:
            break;
        }
        printf("------------------\n");
    } while (1);

    // Close the descriptor
    close(sockfd);

    return 0;
}