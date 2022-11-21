#include "exception.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int check_spaces(char *string, int length)
{
    char *temp = string;
    while ((temp = strpbrk(temp, " ")))
        return 1;
    return 0;
}

void standardize_input(char *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (*(buffer + i) == 10)
        {
            *(buffer + i) = 0;
            return;
        }
    }
    return;
}

int check_if_signed_in(char *buffer)
{
    char ok[5] = "OK.";
    printf("%s %s\n", buffer, ok);
    printf("%d %d\n", strlen(buffer), strlen(ok));
    printf("%d\n", strcmp(buffer, ok));
    if (strcmp(buffer, ok) == 0)
    {
        return 1;
    }
    return 0;
}