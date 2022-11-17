#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <ctype.h>


int check(int option, char input[30]){
    if (option == 1) return isdigit(input[0]);
    else return isalpha(input[1]);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Wrong parameter\n");
        return 0;
    }

    int argv_1 = atoi(argv[1]);
    switch (argv_1)
    {
    case 2:
        if (!check(2, argv[2])){
            printf("Wrong parameter\n");
            return 0;
        }

        host_info = gethostbyname(argv[2]);
        address = (struct in_addr *)(host_info->h_addr);
        printf("Official IP: %s\n", inet_ntoa(*address));
        break;
    case 1:
        if (!check(1, argv[2])){
            printf("Wrong parameter\n");
            return 0;
        }

        struct sockaddr_in sa;
        socklen_t len;
        char hbuf[NI_MAXHOST];

        memset(&sa, 0, sizeof(struct sockaddr_in));

        sa.sin_family = AF_INET;
        sa.sin_addr.s_addr = inet_addr(argv[2]);
        len = sizeof(struct sockaddr_in);

        if (getnameinfo((struct sockaddr *)&sa, len, hbuf, sizeof(hbuf), NULL, 0, NI_NAMEREQD))
        {
            printf("Not found information\n");
        }
        else
        {
            printf("Official name: %s\n", hbuf);
        }
        break;
    default:
        printf("Paraemters Error\n");
        break;
    }
    return 0;
}