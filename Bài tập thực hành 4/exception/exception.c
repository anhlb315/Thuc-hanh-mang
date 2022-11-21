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