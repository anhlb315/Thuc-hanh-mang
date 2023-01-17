#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <errno.h>
#include "function.h"

int menu()
{
    char user_choice[SMALL];
    int choice;

    printf("-----MENU-----\n");
    printf("1. Login\n");
    printf("2. Text\n");
    printf("0. Exit\n");
    printf("Do: ");
menu_choice:
    if (fgets(user_choice, sizeof(user_choice), stdin) == NULL)
    {
        printf("!!!Error: fgets\n");
        return 0;
    }

    choice = atoi(user_choice);
    if (choice <= -1 || choice >= 3)
    {
        printf("!!!Invalid option\nAgain please: ");
        goto menu_choice;
    }

    return choice;
}

int login(int socket_fd)
{
    char login_name[MEDIUM];
    Message message;

    printf("-----LOGIN-----\n");
    printf("Your login name: ");
    if (fgets(login_name, sizeof(login_name), stdin) == NULL)
    {
        printf("!!!Error: fgets\n");
        return 0;
    }

    message.header = LOGIN;
    strcpy(message.login_name, login_name);

    if(send(socket_fd, &message, sizeof(struct _message), 0) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return 0;
    }

    printf("Loading...\n");

    if(recv(socket_fd, &message, sizeof(struct _message), 0) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return 0;
    }

    switch (message.header)
    {
    case OK:
        printf("%s\n", message.text);
        break;
    case ERROR:
        printf("%s\n", message.text);
        break;
    default:
        printf("!!!Something wrong with server\n");
        break;
    }

    return 0;
}

int text(int socket_fd)
{
    char text[LARGE];
    Message message;

    printf("-----TEXT-----\n");
    printf("Your text: ");
    if (fgets(text, sizeof(text), stdin) == NULL)
    {
        printf("!!!Error: fgets\n");
        return 0;
    }

    message.header = TEXT;
    strcpy(message.text, text);

    if(send(socket_fd, &message, sizeof(struct _message), 0) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return 0;
    }

    printf("Loading...\n");

    if(recv(socket_fd, &message, sizeof(struct _message), 0) < 0)
    {
        fprintf(stderr, "[-]%s\n", strerror(errno));
        return 0;
    }

    switch (message.header)
    {
    case OK:
        printf("%s\n", message.text);
        break;
    case ERROR:
        printf("%s\n", message.text);
        break;
    default:
        printf("!!!Something wrong with server\n");
        break;
    }

    return 0;
}

int exit_program(int socket_fd)
{
    return 0;
}