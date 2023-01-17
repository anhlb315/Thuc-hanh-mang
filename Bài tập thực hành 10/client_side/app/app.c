#include "app.h"
#include "../function/function.h"
#include <stdio.h>

int client_app(int socket_fd)
{
// Menu
menu:
    switch (menu())
    {
    case 1: //- Login
        if (login(socket_fd) == 0)
        {
            printf("!!!Error: login");
            return 0;
        }
        goto menu;
    case 2: //- Text
        if (text(socket_fd) == 0)
        {
            printf("!!!Error: text");
            return 0;
        }
        goto menu;
    case 0: // Exit
        if (exit_program(socket_fd) == 0)
        {
            printf("!!!Error: exit_program");
            return 0;
        }
        return 1;
    default:
        printf("!!!Error: menu\n");
        break;
    }
    return 0;
}