#include <stdio.h>
#include <stdlib.h>
#include "account.h"

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
        printf("7. Homepage with domain name\n");
        printf("6. Homepage with IP address\n");
        printf("Your choice (1-8, other to quit):\n");
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
            change_password(acc);
            break;
        case 6:
            sign_out(acc);
            break;
        case 7:
            print_domain_name(acc);
            break;
        case 8:
            print_ip_address(acc);
            break;
        default:
            free_list(acc);
            printf("Exit program...\n");
            exit(0);
        }
    }
}