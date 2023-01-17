#include "app.h"
#include "../function/function.h"
#include <stdio.h>

int client_app(int socket_fd)
{
    //Menu
    switch (menu())
    {
    case 1: //- Login
        if(login(socket_fd) == 0)
        {
            printf("!!!Error: login");
            return 0;
        }
        break;
    case 2: //- Text
        if(text() == 0)
        {
            printf("!!!Error: text");
            return 0;
        }
        break;
    case 0: // Exit
        exit_program();
        break;
    default:
        printf("!!!Error: menu\n");
        break;
    }
    return 0;
}