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

// void app(int client_fd)
// {
//     char message[BUFFER_SIZE];
//     char exit[BUFFER_SIZE] = "exit\0";
//     int n;
//     char sign_in_feedback[BUFFER_SIZE];
//     Account *acc = NULL;
//     acc = read_account(acc);
//     int feedback;
//     int password_incorrect_times = 3;
//     char bye[100] = "bye\0";
//     char is_password_changing[BUFFER_SIZE];
//     char only_number[BUFFER_SIZE];
//     char only_string[BUFFER_SIZE];
//     char exit_program[100] = "exit_program\0";
//     User user;

//     // Chat
//     while (1)
//     {
//         // Receive username & password from client
//         recv(client_fd, &user, sizeof(struct _user), MSG_WAITALL);

//         // Standardize strings
//         standardize_input(user.username, sizeof(user.username));
//         standardize_input(user.password, sizeof(user.password));

//         // Check for exit program
//         if (strcmp(exit_program, user.username) == 0)
//             break;

//         // Print username & password
//         printf("[+]Username: %s\n", user.username);
//         printf("[+]Password: %s\n", user.password);

//         // Sign in
//         feedback = sign_in(acc, user.username, user.password);
//         if (feedback == 3) // If wrong password
//         {
//             password_incorrect_times--;
//             if (password_incorrect_times == 0)
//             {
//                 change_current_account_status(acc, user.username, 2);

//                 feedback++; // 4 mean account is blocked
//             }
//         }

//         sprintf(sign_in_feedback, "%d", feedback);
//         send(client_fd, sign_in_feedback, sizeof(sign_in_feedback), 0);

//         if (feedback == 0) // If signed in
//         {
//             recv(client_fd, is_password_changing, sizeof(is_password_changing), 0);
//             standardize_input(is_password_changing, sizeof(is_password_changing));

//             if (strcmp(bye, is_password_changing) == 0)
//             {
//                 if (sign_out(acc, user.username))
//                 {
//                     send(client_fd, bye, sizeof(bye), 0);
//                 }
//             }
//             else if (strlen(is_password_changing) > 1)
//             {
//                 if (change_password(acc, user.username, is_password_changing))
//                 {
//                     send(client_fd, sign_in_feedback, sizeof(sign_in_feedback), 0);
//                 }
//                 if (split(is_password_changing, only_number, only_string))
//                 {
//                     send(client_fd, only_number, sizeof(only_number), 0);
//                     send(client_fd, only_string, sizeof(only_string), 0);
//                 }
//             }
//         }

//         printf("---------------------\n");
//     }

//     close(client_fd);
// }

void sign_in(int client_fd, Account* acc)
{
    // Variables
    User user;
    char exit_program[100] = "exit_program\0";
    int feedback;
    int password_incorrect_times = 3;
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
        password_incorrect_times--;
        if (password_incorrect_times == 0)
        {
            change_current_account_status(acc, user.username, 2);
            feedback++; // 4 mean account is blocked
        }
    }

    // Send feedback to Client
    sprintf(sign_in_feedback, "%d", feedback);
    if(send(client_fd, sign_in_feedback, sizeof(sign_in_feedback), 0) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return;
    }

    return;
}