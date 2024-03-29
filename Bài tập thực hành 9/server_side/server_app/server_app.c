#include "server_app.h"
#include "../exception/exception.h"
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <errno.h>

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

void sign_in(int client_fd, Account *acc)
{
    // Variables
    User user;
    char exit_program[100] = "exit_program\0";
    int feedback;
    char sign_in_feedback[BUFFER_SIZE];
    char only_number[BUFFER_SIZE], only_string[BUFFER_SIZE];

    // Recv User from Client
    if (recv(client_fd, &user, sizeof(struct _user), 0) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return;
    }

    // Print username & password
    standardize_input(user.username, sizeof(user.username));
    standardize_input(user.password, sizeof(user.password));
    printf("[+]Username: %s\n", user.username);
    printf("[+]Password: %s\n", user.password);

    // Sign in
    feedback = account_sign_in(acc, user.username, user.password);
    if (feedback == 3) // If wrong password
    {
        user.password_error = user.password_error - 1;
        if (user.password_error == 0)
        {
            change_current_account_status(acc, user.username, 2);
            feedback++; // 4 mean account is blocked
        }
    }

    // Send feedback to Client
    sprintf(sign_in_feedback, "%d", feedback);
    if (send(client_fd, sign_in_feedback, sizeof(sign_in_feedback), 0) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return;
    }

    return;
}

void change_password(int client_fd, Account *acc)
{
    // Variables
    char bye[100] = "bye\0";
    char only_number[BUFFER_SIZE], only_string[BUFFER_SIZE];
    int feedback;
    char change_password_feedback[BUFFER_SIZE];
    User user;

    // Recv User from client
    if (recv(client_fd, &user, sizeof(struct _user), MSG_WAITALL) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return;
    }
    else
    {
        printf("[+]User's username: %s\n", user.username);
        printf("[+]User's password: %s\n", user.password);
        feedback = account_change_password(acc, user.username, user.password);
        if (feedback)
        {
            printf("[+]Change password successfully\n");
            current_user_get_info(acc, user.username);
        }
        else
        {
            printf("[-]Fail to change password\n");
        }
        sprintf(change_password_feedback, "%d", feedback);
        if (send(client_fd, change_password_feedback, sizeof(change_password_feedback), 0) < 0)
        {
            fprintf(stderr, "[-]%s\n", strerror(errno));
            return;
        }
    }

    return;
}

int sign_out(int client_fd, Account *acc)
{
    // Variables
    int feedback;
    char sign_out_feedback[BUFFER_SIZE];
    User user;

    // Recv User from client
    if (recv(client_fd, &user, sizeof(struct _user), MSG_WAITALL) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return 0;
    }
    else
    {
        printf("[+]User's username: %s\n", user.username);
        printf("[+]User's password: %s\n", user.password);
        feedback = account_sign_out(acc, user.username);
        if (feedback)
        {
            printf("[+]%s sign out successfully\n", user.username);
            current_user_get_info(acc, user.username);
        }
        else
        {
            printf("[-]%s fail to sign out\n", user.username);
        }
        sprintf(sign_out_feedback, "%d", feedback);
        if (send(client_fd, sign_out_feedback, sizeof(sign_out_feedback), 0) < 0)
        {
            fprintf(stderr, "[-]%s\n", strerror(errno));
            return 0;
        }
    }

    return 1;
}