#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

struct hostent *host_info;
struct in_addr *address;

int is_valid_ip_address(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}

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
        if (!is_valid_ip_address(argv[2]))
        {
            printf("%s is not valid\n", argv[2]);
            return 0;
        }

        struct sockaddr_storage addr;
        socklen_t addr_len = sizeof(addr);
        char buffer[NI_MAXHOST];
        int err = getnameinfo((struct sockaddr *)&addr, addr_len, argv[2], 1024, 0, 0, 0);
        if (err != 0)
        {
            printf("failed to convert address to string (code=%d)\n", err);
        }
        printf("Remote address: %s\n", argv[2]);
        break;
    default:
        printf("Paraemters Error\n");
        break;
    }
    return 0;
}