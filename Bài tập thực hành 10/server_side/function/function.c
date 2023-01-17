#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <signal.h>
#include <arpa/inet.h>
#include <errno.h>
#include "function.h"
#include "../exception/exception.h"

int login(int client_fd, char *login_name)
{
    FILE *file;
    Message message;

    standardize_input(login_name, sizeof(char) * MEDIUM);
    printf("login_name: %s\n", login_name);

    file = fopen(USERS_DIR, "w+");
    if (file == NULL)
    {
        printf("!!!Cannot open file: %s", USERS_DIR);

        message.header = ERROR;
        strcpy(message.text, "Cannot open file");

        if (send(client_fd, &message, sizeof(struct _message), 0) < 0)
        {
            fprintf(stderr, "[-]%s\n", strerror(errno));
            return 0;
        }

        return 0;
    }

    fprintf(file, "%s\n", login_name);

    fclose(file);

    message.header = OK;
    strcpy(message.text, "Login successfully");

    if (send(client_fd, &message, sizeof(struct _message), 0) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return 0;
    }

    return 1;
}

int text(int client_fd, char *login_name, char *text)
{
    FILE *user_file;
    FILE *text_file;
    Message message;
    char username[MEDIUM];
    char text_file_dir[LARGE];
    int found = 0;

    standardize_input(text, sizeof(char) * LARGE);
    printf("login_name: %s\n", login_name);
    printf("text: %s\n", text);

    user_file = fopen(USERS_DIR, "r");
    if (user_file == NULL)
    {
        printf("!!!Cannot open file: %s", USERS_DIR);

        message.header = ERROR;
        strcpy(message.text, "Cannot open file");

        if (send(client_fd, &message, sizeof(struct _message), 0) < 0)
        {
            fprintf(stderr, "[-]%s\n", strerror(errno));
            return 0;
        }

        return 0;
    }

    while (!feof(user_file))
    {
        fscanf(user_file, "%s\n", username);
        if (strcmp(username, login_name) == 0)
        {
            found = 1;
            sprintf(text_file_dir, "%s%s.txt", USER_DIR, username);
            text_file = fopen(text_file_dir, "w+");
            fprintf(text_file, "%s\n", text);
            fclose(text_file);
        }
    }

    if (!found)
    {
        message.header = ERROR;
        strcpy(message.text, "Not login yet");

        if (send(client_fd, &message, sizeof(struct _message), 0) < 0)
        {
            fprintf(stderr, "[-]%s\n", strerror(errno));
            return 0;
        }
    }
    else
    {
        message.header = OK;
        strcpy(message.text, "Save text successfully");

        if (send(client_fd, &message, sizeof(struct _message), 0) < 0)
        {
            fprintf(stderr, "[-]%s\n", strerror(errno));
            return 0;
        }
    }

    return 1;
}

int client_exit(int client_fd)
{
    Message message;

    message.header = OK;
    strcpy(message.text, "Exit successfully");

    if (send(client_fd, &message, sizeof(struct _message), 0) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return 0;
    }

    printf("Client (fd: %d) exited\n", client_fd);
    return 1;
}