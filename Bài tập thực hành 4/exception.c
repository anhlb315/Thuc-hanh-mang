#include "exception.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int check_spaces(char *string, int length)
{
    char *temp = string;
    while ((temp = strpbrk(temp, " "))) return 1;
    return 0;
}