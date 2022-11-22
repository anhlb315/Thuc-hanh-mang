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
#include "account/account.h"
#define MAXLINE 1000

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

int main(int argc, char *argv[])
{
    // Catch wrong input
    if (argc == 1)
    {
        printf("Please input port number\n");
        return 0;
    }

    int password_incorrect_times = 3; // Counting enter wrong password time
    char *port_number = argv[1];
    int port = atoi(port_number); // Get port from argv
    int listenfd, new_socket;
    int opt = 1;
    struct sockaddr_in server_address, client_address;
    int len = sizeof(server_address);
    // bzero(&server_address, sizeof(server_address));

    // Create a UDP Socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == 0)
    {
        printf("Socket failed.\n");
        return 0;
    }

    // Forcefully attaching socket to the port
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        printf("setsockopt\n");
        return 0;
    }

    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);
    server_address.sin_family = AF_INET;

    // Forcefully attaching socket to the port
    if (bind(listenfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        printf("bind failed\n");
        return 0;
    }
    if (listen(listenfd, 3) < 0)
    {
        printf("listen\n");
        return 0;
    }
    if ((new_socket = accept(listenfd, (struct sockaddr *)&server_address, (socklen_t *)&len)) < 0)
    {
        printf("accept\n");
        return 0;
    }

    do
    {
        char username_buffer[100] = {0}; // Data username from client
        char password_buffer[100] = {0}; // Data password from client
        char only_number[100] = {0};
        char only_string[100] = {0};

        // Receive the datagram
        // int n = recvfrom(listenfd, username_buffer, sizeof(username_buffer), 0, (struct sockaddr *)&client_address, &len); // Receive username from client
        // standardize_input(username_buffer, n);
        recv(listenfd, username_buffer, strlen(username_buffer), 0);

        // Check for exit program
        char exit_program[100] = "exit_program\0";
        if (strcmp(exit_program, username_buffer) == 0)
            break;

        // n = recvfrom(listenfd, password_buffer, sizeof(password_buffer), 0, (struct sockaddr *)&client_address, &len); // Receive password from client
        recv(listenfd, password_buffer, strlen(password_buffer), 0);
        // standardize_input(password_buffer, n);
        // Note that these buffers have "\n" at the end

        printf("Username: %s\n", username_buffer);
        printf("Password: %s\n", password_buffer);
        printf("-----------------\n");

        //---------------Account---------------------------------------------
        char sign_in_feedback[100];
        Account *acc = NULL;
        acc = read_account(acc);

        // Sign in
        int feedback = sign_in(acc, username_buffer, password_buffer);
        if (feedback == 3) // If wrong password
        {
            password_incorrect_times--;
            if (password_incorrect_times == 0)
            {
                change_current_account_status(acc, username_buffer, 2);

                feedback++; // 4 mean account is blocked
            }
        }

        sprintf(sign_in_feedback, "%d", feedback);
        // sendto(listenfd, sign_in_feedback, MAXLINE, 0, (struct sockaddr *)&client_address, sizeof(client_address));
        send(listenfd, sign_in_feedback, strlen(sign_in_feedback), 0);

        if (feedback == 0) // If signed in
        {
            char is_password_changing[10];
            // n = recvfrom(listenfd, is_password_changing, sizeof(is_password_changing), 0, (struct sockaddr *)&client_address, &len); // Receive from client
            recv(listenfd, is_password_changing, strlen(is_password_changing), 0);
            // standardize_input(is_password_changing, n);

            char bye[100] = "bye\0";
            if (strcmp(bye, is_password_changing) == 0)
            {
                if (sign_out(acc, username_buffer))
                {
                    // sendto(listenfd, bye, MAXLINE, 0, (struct sockaddr *)&client_address, sizeof(client_address));
                    send(listenfd, bye, strlen(bye), 0);
                }
            }
            else if (strlen(is_password_changing) > 1)
            {
                if (change_password(acc, username_buffer, is_password_changing))
                {
                    // sendto(listenfd, sign_in_feedback, MAXLINE, 0, (struct sockaddr *)&client_address, sizeof(client_address));
                    send(listenfd, sign_in_feedback, strlen(sign_in_feedback), 0);
                }
                if (split(is_password_changing, only_number, only_string))
                {
                    // sendto(listenfd, only_number, MAXLINE, 0, (struct sockaddr *)&client_address, sizeof(client_address));
                    send(listenfd, only_number, strlen(only_number), 0);
                    // sendto(listenfd, only_string, MAXLINE, 0, (struct sockaddr *)&client_address, sizeof(client_address));
                    send(listenfd, only_string, strlen(only_string), 0);
                }
            }
        }

        //-------------------------------------------------------------------
    } while (1);
}