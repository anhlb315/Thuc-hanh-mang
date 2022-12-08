#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helper.h"
#include "../exception/exception.h"
#include "../account/account.h"
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <pthread.h>
#define BUFFER_SIZE 1024

int menu()
{
    char user_choice[BUFFER_SIZE];
    int choice;

    printf("----------MENU----------\n");
    printf("1. Tutorial\n");
    printf("2. Play\n");
    printf("3. Log out\n");
goal:
    printf("Your choice: ");
    if (fgets(user_choice, sizeof(user_choice), stdin) == NULL)
    {
        printf("Input error.\n");
        return 0;
    }

    // Check input
    if (strlen(user_choice) <= 2)
    {
        choice = atoi(user_choice);
        if (choice <= 0 || choice >= 4)
        {
            printf("Invalid choice. Try again.\n");
            goto goal;
        }
    }
    else
    {
        printf("Invalid choice. Try again.\n");
        goto goal;
    }

    return choice;
}

char welcome()
{
    char user_choice[BUFFER_SIZE];

    printf("---------Welcome-----------\n");
    printf("Do you have an account?(y/n/bye): ");
goal:
    if (fgets(user_choice, sizeof(user_choice), stdin) == NULL)
    {
        printf("Input error.\n");
        return 0;
    }

    // Check input
    if (check_yes_no_bye(user_choice))
    {
        printf("Invalid choice. Again please: ");
        goto goal;
    }

    return user_choice[0];
}

int exit_program()
{
    char user_choice[BUFFER_SIZE];

    printf("-------------Exit----------\n");
    printf("Do you really really want to exit?(y/n): ");
goal:
    if (fgets(user_choice, sizeof(user_choice), stdin) == NULL)
    {
        printf("Input error.\n");
        return 0;
    }

    // Check input
    if (check_yes_no(user_choice))
    {
        printf("Invalid choice. Again please: ");
        goto goal;
    }

    if (user_choice[0] == 'y')
        return 1;
    else
        return 0;
}

int sign_up(int socket_fd)
{

    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    char confirm_password[BUFFER_SIZE];
    char control_signal[BUFFER_SIZE] = "sign in\0"; // for now

    printf("---------Sign up-----------\n");
goal:
    printf("Username: ");
    if (fgets(username, sizeof(username), stdin) == NULL)
    {
        printf("Input error.\n");
        return 0;
    }
    if (check_spaces(username, sizeof(username)))
    {
        printf("Contain white scape(s). Try again.\n");
        goto goal;
    }
goal1:
    printf("Password: ");
    if (fgets(password, sizeof(password), stdin) == NULL)
    {
        printf("Input error.\n");
        return 0;
    }
    if (check_spaces(password, sizeof(password)))
    {
        printf("Contain white scape(s). Try again.\n");
        goto goal1;
    }
goal2:
    printf("New password: ");
    if (fgets(confirm_password, sizeof(confirm_password), stdin) == NULL)
    {
        printf("Input error.\n");
        return 0;
    }
    if (check_spaces(confirm_password, sizeof(confirm_password)))
    {
        printf("Contain white scape(s). Try again.\n");
        goto goal2;
    }
    if (check_confirm_password(confirm_password, password))
    {
        goto goal2;
    }

    send(socket_fd, control_signal, sizeof(control_signal), 0);
    send(socket_fd, username, sizeof(username), 0);
    send(socket_fd, confirm_password, sizeof(confirm_password), 0);
    return 1;
}

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

void *client_handler(void *arg)
{
    int client_fd;
    char message[BUFFER_SIZE];
    char exit[BUFFER_SIZE] = "exit\0";
    pthread_detach(pthread_self());
    client_fd = (int)arg;
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    int n;
    char sign_in_feedback[BUFFER_SIZE];
    Account *acc = NULL;
    acc = read_account(acc);
    int feedback;
    int password_incorrect_times = 3;
    char bye[100] = "bye\0";
    char is_password_changing[BUFFER_SIZE];
    char only_number[BUFFER_SIZE];
    char only_string[BUFFER_SIZE];
    char exit_program[100] = "exit_program\0";

    // Chat
    while (1)
    {       
        // Clean buffers
        bzero(username, sizeof(username));
        bzero(password, sizeof(password));

        // Receive username & password from client
        read(client_fd, username, sizeof(username));
        read(client_fd, password, sizeof(password));

        // Standardize strings
        standardize_input(username, sizeof(username));
        standardize_input(password, sizeof(password));

        // Check for exit program
        if (strcmp(exit_program, username) == 0)
            break;

        // Print username & password
        printf("Username: %s\n", username);
        printf("Password: %s\n", password);

        // Sign in
        feedback = sign_in(acc, username, password);
        if (feedback == 3) // If wrong password
        {
            password_incorrect_times--;
            if (password_incorrect_times == 0)
            {
                change_current_account_status(acc, username, 2);

                feedback++; // 4 mean account is blocked
            }
        }

        sprintf(sign_in_feedback, "%d", feedback);
        write(client_fd, sign_in_feedback, sizeof(sign_in_feedback));

        if (feedback == 0) // If signed in
        {
            read(client_fd, is_password_changing, sizeof(is_password_changing));
            standardize_input(is_password_changing, sizeof(is_password_changing));

            if (strcmp(bye, is_password_changing) == 0)
            {
                if (sign_out(acc, username))
                {
                    write(client_fd, bye, sizeof(bye));
                }
            }
            else if (strlen(is_password_changing) > 1)
            {
                if (change_password(acc, username, is_password_changing))
                {
                    write(client_fd, sign_in_feedback, sizeof(sign_in_feedback));
                }
                if (split(is_password_changing, only_number, only_string))
                {
                    write(client_fd, only_number, sizeof(only_number));
                    write(client_fd, only_string, sizeof(only_string));
                }
            }
        }

        printf("---------------------\n");
    }

    close(client_fd);
}