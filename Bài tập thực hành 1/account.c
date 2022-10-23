#include "account.h"
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

Account *add_account(Account *account, char *username, char *password, int status)
{
    if (account == NULL)
    {
        Account *temp = create_new_account(username, password, status);
        return temp;
    }
    if (check_user(account, username))
    {
        Account *cur = account;
        while (cur->next != NULL)
        {
            cur = cur->next;
        }
        Account *temp = create_new_account(username, password, status);
        cur->next = temp;
        return account;
    }
    return NULL;
}

int check_user(Account *account, char *username)
{
    Account *cur = account;
    while (cur != NULL)
    {
        if (strcmp(cur->username, username) == 0)
        {
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

int check_status(Account *account, char *username)
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

int check_signed_in(Account *account, char *username)
{
    Account *cur = account;
    while (cur != NULL)
    {
        if (strcmp(cur->username, username) == 0)
        {
            return cur->is_signed_in;
        }
        cur = cur->next;
    }
}

Account *read_account(Account *acc)
{
    char username[30];
    char password[30];
    int status;
    number_of_account = 0;
    FILE *inp = fopen("nguoidung.txt", "r");
    if (!inp)
    {
        printf("Error: Can't open this file! \n");
        return NULL;
    }

    do
    {
        if (fscanf(inp, "%s %s %d", username, password, &status) > 0)
        {
            acc = add_acount(acc, username, password, status);
            number_of_account++;
        }
        else
            break;
    } while (1);
    fclose(inp);
    return acc;
}

Account *register_account(Account *acc)
{
    printf("----Welcome to Register function.----\n");

    char username[30];
    char password[30];
    printf("Input your Username: ");
    scanf("%s", username);
    fflush(stdin);
    if (check_user(acc, username) != 0)
    {
        printf("Input your Password: ");
        scanf("%s", password);
        fflush(stdin);
        acc = add_account(acc, username, password, 2);
        number_of_account++;
        printf("Successful registration. \n");
        update_file(acc);
    }
    else
    {
        printf("This account existed! \n");
    }
    return acc;
}

void update_file(Account *acc)
{
    FILE *inp = fopen("nguoidung.txt", "w+");
    Account *cur = acc;
    while (cur != NULL)
    {
        fprintf(inp, "%s %s %d\n", cur->username, cur->password, cur->status);
        cur = cur->next;
    }
    fclose(inp);
}

void sign_in(Account *acc)
{
    printf("----Welcome to Sign in function.----\n");
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
        if (check_status(acc, username) == 0)
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

    printf("Input username: ");
    scanf("%s", username);
    if (check_user(acc, username) != 0)
    {
        printf("Account does not exist!\n");
        return;
    }

    if (check_signed_in(acc, username) == 0)
    {
        printf("Yet signed in.\n");
        return;
    }

    switch (check_status(acc, username))
    {
    case 0:
        printf("Account is blocked!\n");
        break;
    case 1:
        printf("Account is activated!\n");
        break;
    case 2:
        printf("Account is idle!\n");
        break;
    default:
        printf("Something wrong!!!");
        break;
    }
    return;
}

void sign_out(Account *acc)
{
    printf("----Welcome to Sign Out function.----\n");
    char username[30];

    printf("Input your Username: ");
    scanf("%s", username);
    if (check_user(acc, username) != 0)
    {
        printf("Account does not exist!\n");
        return;
    }

    if (check_signed_in(acc, username) == 0)
    {
        printf("Yet signed in.\n");
        return;
    }

    Account *cur = acc;
    while (cur != NULL)
    {
        if (strcmp(cur->username, username) == 0)
        {
            cur->is_signed_in = 0;
            printf("Sign out Successfully.\n");
            return;
        }
        cur = cur->next;
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

int check_activate_code(char *activate_code, char *correct_activate_code)
{
    return strcmp(activate_code, correct_activate_code) == 0;
}

void activate(Account *acc)
{
    printf("----Welcome to activate function.----\n");
    char username[30];
    char password[30];
    char activate_code[30];
    char correct_activate_code[30] = "20194616\n";

    printf("Input your Username: ");
    scanf("%s", username);
    if (check_user(acc, username) != 0)
    {
        printf("Account does not exist!\n");
    }
    else
    {
        printf("Input your Password: ");
        scanf("%s", password);
        if (check_password(acc, password) != 0)
        {
            printf("Password is incorrect!\n");
        }
        else
        {
            int time = 4;
            do
            {
                printf("Enter your activate code: ");
                scanf("%s", activate_code);
                if (check_activate_code(activate_code, correct_activate_code) != 0)
                {
                    printf("Activate Code is not correct, enter again.\n");
                    time--;
                }
                else
                {
                    break;
                }
            } while (time != 0);
            Account *cur = acc;
            while (cur != NULL)
            {
                if (strcmp(cur->username, username) == 0)
                {
                    cur->status = 1;
                    printf("Change to activate successfully.");
                }
                cur = cur->next;
            }
            update_file(acc);
        }
    }
    return;
}

void change_password(Account *acc)
{
    printf("----Welcome to change password function.----\n");
    char username[30];
    char old_password[30];
    char new_password[30];

    printf("Input your Username: ");
    scanf("%s", username);
    if (check_user(acc, username) != 0)
    {
        printf("Account does not exist!\n");
        return;
    }

    printf("Input your Old Password: ");
    scanf("%s", old_password);
    if (check_password(acc, old_password) != 0)
    {
        printf("Password is incorrect!\n");
        return;
    }

    printf("Input your New Password: ");
    scanf("%s", new_password);

    Account *cur = acc;
    while (cur != NULL)
    {
        if (strcmp(cur->username, username) == 0)
        {
            strcpy(cur->password, new_password);
            printf("Change password successfully.");
        }
        cur = cur->next;
    }
    update_file(acc);
    return;
}