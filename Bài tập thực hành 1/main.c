#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

int main() {
    int menu;
    Account *acc = NULL;
    acc = read_account(acc);
    while (1)
    {
        printf("\nUSER MANAGEMENT PROGRAM\n");
        printf("-----------------------------------\n");
        printf("1. Register\n");
        printf("2. Activate\n");
        printf("3. Sign In\n");
        printf("4. Search\n");
        printf("5. Change Password\n");
        printf("6. Sign Out\n");
        printf("Your choice (1-6, other to quit):\n");
        scanf("%d", &menu); fflush(stdin);
        switch (menu) {
        case 1:
            acc = register_account(acc);
            break;
        case 2:
            activate(acc);
            break;
        case 3:
            sign_in(acc);
            break;
        case 4:
            search(acc);
            break;
        case 5:
            break;
        case 6:
            sign_out(acc);
            break;
        default:
            free_list(acc);
            exit(0);
        }
    }
}