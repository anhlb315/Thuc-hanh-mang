#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024

int menu()
{
    char user_choice[BUFFER_SIZE];
    int choice;

    printf("-----MENU-----\n");
    printf("1. Login\n");
    printf("2. Text\n");
    printf("0. Exit\n");
    printf("Do: ");
menu_choice:
    if (fgets(user_choice, sizeof(BUFFER_SIZE), stdin) == NULL)
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

int login()
{
    return 0;
}

int text()
{
    return 0;
}

int exit_program()
{
    return 0;
}