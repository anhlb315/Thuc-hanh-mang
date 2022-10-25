#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

struct hostent *host_info;
struct in_addr *address;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Parameters Error\n");
        return 0;
    }

    int argv_1 = atoi(argv[1]);
    switch (argv_1)
    {
    case 1:
        printf("Will do a DNS query on: %s\n", argv[2]);
        host_info = gethostbyname(argv[2]);
        address = (struct in_addr *)(host_info->h_addr);
        printf("%s has address %s\n", argv[2], inet_ntoa(*address));
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