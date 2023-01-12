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

int sign_in(int socket_fd, User *current_user)
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
            {
                printf("[-]Error: fgets\n");
                return 0;
            }

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
            {
                printf("[-]Error: fgets\n");
                return 0;
            }

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
            standardize_input(username, sizeof(username));
            standardize_input(password, sizeof(password));
            strcpy(user.username, username);
            strcpy(user.password, password);
            user.password_error = 3;

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
                    strcpy(current_user->username, username);
                    strcpy(current_user->password, password);
                    printf("[+]OK\n");
                    break;
                case 1:
                    printf("[-]Cannot find account\n");
                    return 0;
                case 2:
                    printf("[-]Account is not ready\n");
                    return 0;
                case 3:
                    printf("[-]Not OK\n");
                    return 0;
                case 4:
                    printf("[-]Not OK\n");
                    printf("[-]Account is blocked\n");
                    return 0;
                default:
                    printf("[-]Something wrong with server\n");
                    return 0;
                }
            }
        }
        else
        {
            printf("[-]Something wrong with server\n");
        }
    }

    return 1;
}

int change_password(int socket_fd, User *current_user)
{
    // Variables
    char change_password_signal[BUFFER_SIZE] = "1\0";
    char server_feedback[BUFFER_SIZE];
    char new_password[BUFFER_SIZE], confirm_password[BUFFER_SIZE];
    char only_number[BUFFER_SIZE], only_string[BUFFER_SIZE];

    // Send signal to server
    if (send(socket_fd, change_password_signal, sizeof(change_password_signal), 0) < 0)
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
    new_password:
        printf("[+]New password: ");
        if (fgets(new_password, sizeof(new_password), stdin) == NULL)
        {
            printf("[-]Error: fgets\n");
            return 0;
        }

        // Check new_password
        if (check_new_password(new_password))
        {
            printf("[-]Can only contains number or letter. Try again please.\n");
            goto new_password;
        }

    confirm_password:
        printf("[+]Confirm password: ");
        if (fgets(confirm_password, sizeof(confirm_password), stdin) == NULL)
        {
            printf("[-]Error: fgets\n");
            return 0;
        }

        // Check confirm_password
        if (check_confirm_password(confirm_password, new_password))
        {
            goto confirm_password;
        }

        // Change current user password
        standardize_input(confirm_password, sizeof(confirm_password));
        strcpy(current_user->password, confirm_password);

        // Send User with new password to Server
        if (send(socket_fd, current_user, sizeof(struct _user), 0) < 0)
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
            case 1:
                printf("[+]Change password successfully.\n");
                break;
            case 0:
                printf("[-]Fail to change password\n");
                return 0;
            default:
                printf("[-]Something wrong with server\n");
                return 0;
            }
        }
    }

    return 1;
}

int sign_out(int socket_fd, User current_user)
{
    // Variables
    char sign_out_signal[BUFFER_SIZE] = "2\0";
    char server_feedback[BUFFER_SIZE];

    // Send signal to server
    if (send(socket_fd, sign_out_signal, sizeof(sign_out_signal), 0) < 0)
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
            // Send user to Server
            if (send(socket_fd, &current_user, sizeof(struct _user), 0) < 0)
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
                case 1:
                    printf("[+]Sign out successfully\n");
                    break;
                case 0:
                    printf("[-]Sign out failed\n");
                    return 0;
                default:
                    printf("[-]Something wrong with server\n");
                    return 0;
                }
            }
        }
        else
        {
            printf("[-]Something wrong with server\n");
        }
    }

    return 1;
}

int exit_program(int socket_fd, User current_user)
{
    // Variables
    char exit_signal[BUFFER_SIZE] = "3\0";
    char server_feedback[BUFFER_SIZE];

    // Send signal to server
    if (send(socket_fd, exit_signal, sizeof(exit_signal), 0) < 0)
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
            // Send user to Server
            if (send(socket_fd, &current_user, sizeof(struct _user), 0) < 0)
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
                case 1:
                    printf("[+]Sign out successfully\n");
                    break;
                case 0:
                    printf("[-]Sign out failed\n");
                    return 0;
                default:
                    printf("[-]Something wrong with server\n");
                    return 0;
                }
            }
        }
        else
        {
            printf("[-]Something wrong with server\n");
        }
    }

    return 1;
}

void app(int socket_fd)
{
    // Variables
    char choice[BUFFER_SIZE];
    char bye[BUFFER_SIZE] = "bye\n\0";
    User current_user;

    while (1)
    {
        if (sign_in(socket_fd, &current_user))
        {
            printf("[+]Do you want to change password?(y/n/bye): ");
        choice:
            if (fgets(choice, BUFFER_SIZE, stdin) == NULL)
            {
                printf("[-]Error: fgets\n");
                return;
            }

            if (choice[0] != 'y' && choice[0] != 'n' && !(strcmp(choice, bye) == 0))
            {
                printf("[-]Wrong input. Only y or n.\n");
                goto choice;
            }

            if (choice[0] == 'y')
            {
                if (!change_password(socket_fd, &current_user))
                {
                    printf("[-]Error: change_password\n");
                    return;
                }
            }
            else if (choice[0] == 'n')
            {
                if (!sign_out(socket_fd, current_user))
                {
                    printf("[-]Error: sign_out\n");
                    return;
                }
            }
            else
            {
                if (exit_program(socket_fd, current_user))
                {
                    printf("[+]Exit program\n");
                    return;
                }
                else
                {
                    printf("[-]Fail to exit program\n");
                    return;
                }
            }
        }
    }
}