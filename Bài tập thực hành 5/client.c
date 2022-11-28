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
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    // Check input
    if (argc != 3)
    {
        printf("Please input IP address and Port number\n");
        return 0;
    }

    // Some variables
    char *ip_address = argv[1];   // Get IP from argv
    char *port_number = argv[2];  // Get Port number from argv
    int port = atoi(port_number); // int-type Port number
    int sockfd = 0;               // Socket
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    char only_number[BUFFER_SIZE];
    char only_string[BUFFER_SIZE];
    char choice[BUFFER_SIZE];
    char new_password[BUFFER_SIZE];
    char confirm_password[BUFFER_SIZE];

    // Using TCP
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation error.\n");
        return 0;
    }

    // Clear server_address
    bzero(&server_address, sizeof(server_address));
    server_address.sin_port = htons(port);
    server_address.sin_family = AF_INET;

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip_address, &server_address.sin_addr) <= 0)
    {
        printf("Invalid address / Address not supported.\n");
        return 0;
    }
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(struct sockaddr_in)) < 0)
    {
        printf("Connection Failed.\n");
        return 0;
    }

    do
    {
    goal0:
        // Get username & password
        printf("Username: ");
        if (fgets(username, BUFFER_SIZE, stdin) == NULL)
            break;

        // Check for scape
        if (check_spaces(username, strlen(username)))
        {
            printf("Username contains white space. Please enter again.\n\n");
            goto goal0;
        }

        printf("Password: ");
        if (fgets(password, BUFFER_SIZE, stdin) == NULL)
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
            printf("Password contains white space. Please enter again.\n\n");
            goto goal0;
        }

        // Request to send datagram ???
        // No need to specify server address in sendto
        // Connect stores the peers IP and port
        write(sockfd, username, strlen(username));
        write(sockfd, password, strlen(password));

        // Waiting for response
        read(sockfd, buffer, BUFFER_SIZE);
        switch (atoi(buffer))
        {
        case 0:
            printf("OK.\n");
            printf("------------------\n");
        goal1:
            printf("Do you want to change password or sign out?(y/n/bye): ");
            if (fgets(choice, BUFFER_SIZE, stdin) == NULL)
                break;

            char bye[100] = "bye\n\0";
            if (choice[0] != 121 && choice[0] != 110 && !(strcmp(choice, bye) == 0))
            {
                printf("Wrong input. Only y or n.\n");
                goto goal1;
            }

            if (choice[0] == 121)
            {
            goal2:
                printf("New password: ");
                if (fgets(new_password, BUFFER_SIZE, stdin) == NULL)
                    break;

                // Check new_password
                if (check_new_password(new_password))
                {
                    printf("Can only contains number or letter. Try again please.\n");
                    goto goal2;
                }
            goal3:
                printf("Confirm password: ");
                if (fgets(confirm_password, BUFFER_SIZE, stdin) == NULL)
                    break;

                // Check confirm_password
                if (check_confirm_password(confirm_password, new_password))
                {
                    goto goal3;
                }

                write(sockfd, confirm_password, strlen(confirm_password));
                read(sockfd, buffer, BUFFER_SIZE);
                read(sockfd, only_number, BUFFER_SIZE);
                read(sockfd, only_string, BUFFER_SIZE);
                if (atoi(buffer) == 0)
                {
                    printf("Encoded password: %s %s\n", only_number, only_string);
                    printf("Change password successfully.\n");
                }
            }
            else if (choice[0] == 110)
            {
                char request[2] = "0";
                write(sockfd, request, strlen(request));
            }
            else
            {
                char sign_out_request[100] = "bye\0";
                write(sockfd, sign_out_request, strlen(sign_out_request));
                read(sockfd, buffer, BUFFER_SIZE);
                if (strcmp(sign_out_request, buffer) == 0)
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