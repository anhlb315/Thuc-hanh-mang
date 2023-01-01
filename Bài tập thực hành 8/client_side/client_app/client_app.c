#include "client_app.h"
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <errno.h>
#include "../exception/exception.h"

// void app(int socket_fd)
// {
//     char username[BUFFER_SIZE];
//     char password[BUFFER_SIZE];
//     char sign_in_feedback[BUFFER_SIZE];
//     char choice[BUFFER_SIZE];
//     char bye[BUFFER_SIZE] = "bye\n\0";
//     char new_password[BUFFER_SIZE];
//     char confirm_password[BUFFER_SIZE];
//     char buffer[BUFFER_SIZE];
//     char only_number[BUFFER_SIZE];
//     char only_string[BUFFER_SIZE];
//     char sign_out_request[100] = "bye\0";
//     User user;

//     for (;;)
//     {
//     goal0:
//         // Clean buffers
//         bzero(username, sizeof(username));
//         bzero(password, sizeof(password));

//         // Get user's input
//         printf("[+]Username: ");
//         if (fgets(username, sizeof(username), stdin) == NULL)
//             break;

//         // Check for exception
//         if (check_spaces(username, strlen(username)))
//         {
//             printf("[-]Username contains white space. Please enter again.\n");
//             goto goal0;
//         }
//     goal1:
//         printf("[+]Password: ");
//         if (fgets(password, sizeof(password), stdin) == NULL)
//             break;

//         // Check for '\n' input
//         if (strcmp(username, "\n") == 0 && strcmp(password, "\n") == 0)
//         {
//             char exit_program[100] = "exit_program\0";
//             send(socket_fd, exit_program, sizeof(exit_program), 0);
//             break;
//         }

//         // Check for scape
//         if (check_spaces(password, strlen(password)))
//         {
//             printf("[-]Password contains white space. Please enter again.\n");
//             goto goal1;
//         }

//         // Send username & password to server
//         strcpy(user.username, username);
//         strcpy(user.password, password);
//         send(socket_fd, &user, sizeof(struct _user), 0);

//         // Sign in response
//         recv(socket_fd, sign_in_feedback, sizeof(sign_in_feedback), 0);
//         switch (atoi(sign_in_feedback))
//         {
//         case 0:
//             printf("[+]OK\n");
//             printf("------------------\n");
//         goal2:
//             printf("[+]Do you want to change password?(y/n/bye): ");
//             if (fgets(choice, BUFFER_SIZE, stdin) == NULL)
//                 break;

//             if (choice[0] != 121 && choice[0] != 110 && !(strcmp(choice, bye) == 0))
//             {
//                 printf("[-]Wrong input. Only y or n.\n");
//                 goto goal2;
//             }

//             if (choice[0] == 121)
//             {
//             goal3:
//                 printf("[+]New password: ");
//                 if (fgets(new_password, BUFFER_SIZE, stdin) == NULL)
//                     break;

//                 // Check new_password
//                 if (check_new_password(new_password))
//                 {
//                     printf("[-]Can only contains number or letter. Try again please.\n");
//                     goto goal3;
//                 }
//             goal4:
//                 printf("[+]Confirm password: ");
//                 if (fgets(confirm_password, BUFFER_SIZE, stdin) == NULL)
//                     break;

//                 // Check confirm_password
//                 if (check_confirm_password(confirm_password, new_password))
//                 {
//                     goto goal4;
//                 }

//                 send(socket_fd, confirm_password, sizeof(confirm_password), 0);
//                 recv(socket_fd, buffer, sizeof(buffer), 0);
//                 recv(socket_fd, only_number, sizeof(buffer), 0);
//                 recv(socket_fd, only_string, sizeof(buffer), 0);
//                 if (atoi(buffer) == 0)
//                 {
//                     printf("[+]Encoded password: %s %s\n", only_number, only_string);
//                     printf("[+]Change password successfully.\n");
//                 }
//             }
//             else if (choice[0] == 110)
//             {
//                 char request[2] = "0";
//                 write(socket_fd, request, strlen(request));
//             }
//             else
//             {
//                 write(socket_fd, sign_out_request, strlen(sign_out_request));
//                 read(socket_fd, buffer, BUFFER_SIZE);
//                 if (strcmp(sign_out_request, buffer) == 0)
//                 {
//                     printf("[+]Sign out successfully.\n");
//                 }
//             }
//             break;
//         case 1:
//             printf("[-]Cannot find account.\n");
//             break;
//         case 2:
//             printf("[-]Account is not ready.\n");
//             break;
//         case 3:
//             printf("[-]Not OK.\n");
//             break;
//         case 4:
//             printf("[-]Not OK. Account is blocked.\n");
//             break;
//         default:
//             break;
//         }

//         printf("---------------------\n");
//     }
//     return;
// }

int sign_in(int socket_fd)
{
    // Variables
    char sign_in_signal[BUFFER_SIZE] = "0\0";
    char server_feedback[BUFFER_SIZE];
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    char exit_program[100] = "exit_program\0";
    User user;

    // Send signal to server
    if (send(socket_fd, sign_in_signal, sizeof(sign_in_signal), 0) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return 0;
    }

    // Recv server's feedback
    if (recv(socket_fd, server_feedback, sizeof(server_feedback), MSG_WAITALL) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return 0;
    }
    else
    {
        standardize_input(server_feedback, sizeof(server_feedback));
        if (atoi(server_feedback))
        {
            printf("[+]Sign in\n");
            // Ask for username
        username:
            printf("[+]Username: ");
            if (fgets(username, sizeof(username), stdin) == NULL)
                return 0;

            // Valid username
            if (check_spaces(username, strlen(username)))
            {
                printf("[-]Username contains white space. Please enter again.\n");
                goto username;
            }

            // Ask for password
        password:
            printf("[+]Password: ");
            if (fgets(password, sizeof(password), stdin) == NULL)
                return 0;

            // Check for exit program
            if (strcmp(username, "\n") == 0 && strcmp(password, "\n") == 0)
            {
                send(socket_fd, exit_program, sizeof(exit_program), 0);
                return 0;
            }

            // Valid password
            if (check_spaces(password, strlen(password)))
            {
                printf("[-]Password contains white space. Please enter again.\n");
                goto password;
            }

            // Create user
            strcpy(user.username, username);
            strcpy(user.password, password);

            // Send user to Server
            if (send(socket_fd, &user, sizeof(struct _user), 0) < 0)
            {
                fprintf(stderr, "[-]%s\n", strerror(errno));
                return 0;
            }

            // Recv server's feedback
            if (recv(socket_fd, server_feedback, sizeof(server_feedback), 0) < 0)
            {
                fprintf(stderr, "[-]%s\n", strerror(errno));
                return 0;
            }
            else
            {
                // Handling server's feedback
                standardize_input(server_feedback, sizeof(server_feedback));
                switch (atoi(server_feedback))
                {
                case 0:
                    printf("[+]OK\n");
                    break;
                case 1:
                    printf("[-]Cannot find account\n");
                    break;
                case 2:
                    printf("[-]Account is not ready\n");
                    break;
                case 3:
                    printf("[-]Not OK\n");
                    break;
                case 4:
                    printf("[-]Not OK\n");
                    printf("[-]Account is blocked\n");
                    break;
                default:
                    printf("[-]Something wrong with server\n");
                    break;
                }
            }
        }
        else
        {
            printf("[-]Something wrong with server\n");
        }
    }

    return 0;
}

void app(int socket_fd)
{
    while (1)
    {
        if (sign_in(socket_fd))
        {
            // change_password(socket_fd);
        }
    }
}