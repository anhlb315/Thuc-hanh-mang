#include "app.h"
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
#include <sys/wait.h>
#include <sys/resource.h>

int server_app(int client_fd)
{
    Message message;
    char login_name[MEDIUM];

    while (1)
    {
        if (recv(client_fd, &message, sizeof(struct _message), MSG_WAITALL) < 0)
        {
            fprintf(stderr, "[-]%s\n", strerror(errno));
            return 0;
        }

        switch (message.header)
        {
        case LOGIN:
            printf("Client (fd: %d) trying to login\n", client_fd);
            if (login(client_fd, message.login_name) == 0)
            {
                printf("!!!Error: login\n");
            }
            strcpy(login_name, message.login_name);
            break;
        case TEXT:
            printf("Client (fd: %d) trying to send text\n", client_fd);
            if (text(client_fd, login_name, message.text) == 0)
            {
                printf("!!!Error: text\n");
            }
            break;
        case EXIT:
            printf("Client (fd: %d) trying to exit\n", client_fd);
            if (client_exit(client_fd) == 0)
            {
                printf("!!!Error: client_exit\n");
            }
            break;
        default:
            printf("!!!Something wrong with client\n");
            break;
        }
    }

    return 0;
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;
    pid = waitpid(-1, &stat, WNOHANG);
    printf("Child %d terminated\n", pid);
}