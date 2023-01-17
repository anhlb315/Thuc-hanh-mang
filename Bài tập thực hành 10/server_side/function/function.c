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
    standardize_input(login_name, sizeof(char) * MEDIUM);
    printf("login_name: %s\n", login_name);
    return 0;
}

int text(int client_fd, char* login_name, char* text)
{
    standardize_input(text, sizeof(char)* LARGE);
    printf("text: %s\n", text);
    return 0;
}

int client_exit(int client_fd)
{
    close(client_fd);
    printf("Client (fd: %d) exited\n", client_fd);
    return 0;
}