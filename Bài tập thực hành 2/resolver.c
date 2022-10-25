#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if(argc != 3) {
        printf("Parameters Error\n");
        return 0;
    }

    int argv_1 = atoi(argv[1]);
    switch (argv_1)
    {
    case 1:
        /* code */
        break;
    case 2:
        /* code */
        break;
    default:
        printf("Paraemters Error\n");
        break;
    }
    return 0;
}