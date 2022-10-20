#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int number_of_account;

Account *create_new_account(char *username, char *password, int status)
{
    Account *p = (Account *)malloc(sizeof(struct _Account));
    strcpy(p->username, username);
    strcpy(p->password, password);
    p->status = status;
    p->is_signed_in = 0;
    p->next = NULL;
    return p;
}

Account *add_account(Account *account, char *username, char *password, int status) {
    if (account == NULL) {
        Account *temp = create_new_account(username, password, status);
        return temp;
    }
    else {
        if (check_user(account, username)) {
            Account *cur = account;
            while (cur->next != NULL) {
                cur = cur->next;
            }
            Account *temp = create_new_account(username, password, status);
            cur->next = temp;
            return account;
        }
        else {
            return NULL;
        }
    }
}

int check_user(Account *account, char *username) {
    Account *cur = account;
    while (cur != NULL) {
        if (strcmp(cur->username, username) == 0) {
            return 0;
        }
        cur = cur->next;
    }
    return 1;
}

int check_password(Account *account, char *password)
{
    Account *cur = account;
    while (cur != NULL)
    {
        if (strcmp(cur->password, password) == 0)
        {
            return 0;
        }
        cur = cur->next;
    }
    return 1;
}

int CheckBlocked(Account *account, char *username)
{
    Account *cur = account;
    while (cur != NULL)
    {
        if (strcmp(cur->username, username) == 0)
        {
            return cur->status;
        }
        cur = cur->next;
    }
}

void check_signed_in(Account *account, char *username)
{
    Account *cur = account;
    while (cur != NULL)
    {
        if (strcmp(cur->username, username) == 0)
        {
            if (cur->is_signed_in == 0)
            {
                printf("This account is not signed in! \n");
            }
            else
            {
                printf("Sign out is sucessful! \n");
                cur->is_signed_in = 0;
            }
            return;
        }
        cur = cur->next;
    }
}

Account *read_account(Account *acc) {
    char username[30];
    char password[30];
    int status;
    number_of_account = 0;
    FILE *inp = fopen("nguoidung.txt", "r");
    if (!inp) {
        printf("Error: Can't open this file! \n");
        return NULL;
    }

    do {
        if (fscanf(inp, "%s %s %d", username, password, &status) > 0) {
            acc = add_account(acc, username, password, status);
            number_of_account++;
        }
        else break;
    } while (1);
    fclose(inp);
    return acc;
}

Account *register_account(Account *acc) {
    printf("----Welcome to Register function.----\n");

    char username[30];
    char password[30];
    printf("Input your Username: ");
    scanf("%s", username); fflush(stdin);
    if (check_user(acc, username) != 0) {
        printf("Input your Password: ");
        scanf("%s", password); fflush(stdin);
        acc = add_account(acc, username, password, 2);
        number_of_account++;
        printf("Successful registration. \n");
        update_file(acc);
    }
    else {
        printf("This account existed! \n");
    }
    return acc;
}

void update_file(Account *acc) {
    FILE *inp = fopen("nguoidung.txt", "w+");
    Account *cur = acc;
    while (cur != NULL) {
        fprintf(inp, "%s %s %d\n", cur->username, cur->password, cur->status);
        cur = cur->next;
    }
    fclose(inp);
}

void sign_in(Account *acc)
{
    printf("----Welcome to sign_in function.----\n");
    char username[30];
    char password[30];

    int n = 3;
    printf("Input your Username: ");
    scanf("%s", username);
    if (check_user(acc, username) != 0)
    {
        printf("Cannot find account \n");
        return;
    }
    else
    {
        if (CheckBlocked(acc, username) == 0)
        {
            printf("Account is blocked, you can't access this account !\n");
            return;
        }
        else
        {
            while (n > 0)
            {
                printf("Input your Password: ");
                scanf("%s", password);
                if (check_password(acc, password) != 0)
                {
                    printf("Password is incorrect!\n");
                    n--;
                }
                else
                {
                    printf("Hello %s, sign in is successful!\n", username);
                    Account *cur = acc;
                    while (cur != NULL)
                    {
                        if (strcmp(cur->username, username) == 0)
                        {
                            cur->is_signed_in = 1;
                        }
                        cur = cur->next;
                    }
                    return;
                }
            }
            printf("This account is blocked!\n");
            Account *cur = acc;
            while (cur != NULL)
            {
                if (strcmp(cur->username, username) == 0)
                {
                    cur->status = 0;
                }
                cur = cur->next;
            }
            update_file(acc);
            return;
        }
    }
}

void search(Account *acc)
{
    printf("----Welcome to search function.----\n");
    char username[30];

    printf("Input your Username: ");
    scanf("%s", username);
    if (check_user(acc, username) != 0)
    {
        printf("Account does not exist!\n");
    }
    else
    {
        if (CheckBlocked(acc, username) == 0)
        {
            printf("Account is blocked!\n");
        }
        else
        {
            printf("Account is active!\n");
        }
    }
    return;
}

void sign_out(Account *acc)
{
    printf("----Welcome to sign_out function.----\n");
    char username[30];

    printf("Input your Username: ");
    scanf("%s", username);
    if (check_user(acc, username) != 0)
    {
        printf("Account does not exist!\n");
    }
    else
    {
        check_signed_in(acc, username);
    }
}

void free_list(Account *head)
{
    Account *tmp;

    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}