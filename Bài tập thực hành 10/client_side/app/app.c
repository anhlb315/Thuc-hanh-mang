#include "app.h"
#include "../function/function.h"
#include <stdio.h>

int client_app(int socket_fd)
{
    //Menu
    switch (menu())
    {
    case 1: //- Login
        login();
        break;
    case 2: //- Text
        text();
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