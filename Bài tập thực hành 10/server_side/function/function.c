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

int login(int client_fd, char* login_name)
{
    FILE* file;

    standardize_input(login_name, sizeof(char) * MEDIUM);
    printf("login_name: %s\n", login_name);

    file = fopen(USER_DIR, "w+");
    if(file == NULL)
    {
        printf("!!!Cannot open file: %s", USER_DIR);
        return 0;
    }

    fprintf(file, "%s\n", login_name);

    fclose(file);

    return 1;
}

int text(int client_fd, char* login_name, char* text)
{
    FILE* user_file;

    standardize_input(text, sizeof(char)* LARGE);
    printf("login_name: %s\n", login_name);
    printf("text: %s\n", text);

    user_file = fopen(USER_DIR, "r");
    if (user_file == NULL)
    {
        printf("!!!Cannot open file: %s", USER_DIR);
        return 0;
    }

    

    return 1;
}

int client_exit(int client_fd)
{
    close(client_fd);
    printf("Client (fd: %d) exited\n", client_fd);
    return 1;
}