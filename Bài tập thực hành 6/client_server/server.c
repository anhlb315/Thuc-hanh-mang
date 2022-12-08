#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include "../exception/exception.h"
#include "../account/account.h"
#include "../helper/helper.h"
#include <signal.h>
#include <arpa/inet.h>
#define BUFFER_SIZE 1024
#define PORT 8080

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

void func(int connect_fd)
{
    int client_fd = connect_fd;
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
}

// Driver function
int main(int argc, char *argv[])
{
    // Catch wrong input
    if (argc != 2)
    {
        printf("Error: Please input port number\n");
        return 0;
    }

    char *port_number = argv[1];
    int port = atoi(port_number);
    int socket_fd, connect_fd, client_address_size;
    struct sockaddr_in server_address, client_address;
    int pid;
    int child_counter = 0;
    char client_address_str[BUFFER_SIZE];

    if (port < 1 || port > 65535)
    {
        printf("Error: Invalid port\n");
        return 0;
    }

    // socket create and verification
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        printf("Error: Socket creation failed\n");
        return 0;
    }
    else
        printf("Socket successfully created\n");
    bzero(&server_address, sizeof(server_address));

    // assign IP, PORT
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    // Binding newly created socket to given IP and verification
    if ((bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address))) != 0)
    {
        printf("Error: Socket bind failed\n");
        return 0;
    }
    else
        printf("Socket successfully bind\n");

    // Now server is ready to listen and verification
    if ((listen(socket_fd, 5)) != 0)
    {
        printf("Error: Listen failed\n");
        return 0;
    }
    else
        printf("Server listening...\n");
    client_address_size = sizeof(client_address);

    // Handling SIGCHLD Signals
    signal(SIGCHLD, sig_chld);

    // Accept the data packet from client_address and verification
    while (1)
    {
        connect_fd = accept(socket_fd, (struct sockaddr *)&client_address, &client_address_size);
        if (connect_fd < 0)
        {
            printf("Error: Server accept failed\n");
            return 0;
        }
        else
        {
            inet_ntop(AF_INET, &client_address, client_address_str, INET_ADDRSTRLEN);
            printf("Server accept the client: %s\n", client_address_str);
        }

        if ((pid = fork()) == 0)
        {
            close(socket_fd);
            func(connect_fd);
            close(connect_fd);
            exit(0);
        }
        else if (pid > 0)
        {
            printf("Child %d created\n", pid);
        }
        else
        {
            printf("Failed to create child.\n");
        }
    }

    // Close the socket
    printf("Server is closing...\n");
    close(socket_fd);
    printf("Server's closed\n");

    return 0;
}