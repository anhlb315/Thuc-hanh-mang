#include "resolver.h"
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <ctype.h>

int check(char input[30])
{
    return isalpha(input[1]);
}

Account *get_signed_in_user(Account *account)
{
    Account *cur = account;
    while (cur != NULL)
    {
        if (cur->is_signed_in == 1)
        {
            return cur;
        }
        cur = cur->next;
    }
}

void print_ip_address_from_domain_name(char *domain_name)
{
    struct sockaddr_in sa;
    socklen_t len;
    char hbuf[NI_MAXHOST];

    memset(&sa, 0, sizeof(struct sockaddr_in));

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(domain_name);
    len = sizeof(struct sockaddr_in);

    if (getnameinfo((struct sockaddr *)&sa, len, hbuf, sizeof(hbuf), NULL, 0, NI_NAMEREQD))
    {
        printf("Not found information\n");
    }
    else
    {
        printf("User's Homepage (Domain Name): %s\n", hbuf);
    }
    return;
}

void print_domain_name_from_ip_address(char *ip_address)
{
    struct hostent *host_info;
    struct in_addr *address;

    host_info = gethostbyname(ip_address);
    address = (struct in_addr *)(host_info->h_addr);
    printf("User's Homepage (IP Address): %s\n", inet_ntoa(*address));
    return;
}

void print_domain_name(Account *account)
{
    Account *current_user = get_signed_in_user(account);
    if (check(current_user->homepage))
    {
        printf("User's Homepage (Domain Name): %s\n", current_user->homepage);
    }
    else
    {
        print_ip_address_from_domain_name(current_user->homepage);
    }
    return;
}

void print_ip_address(Account *account)
{
    Account *current_user = get_signed_in_user(account);
    if (check(current_user->homepage))
    {
        print_domain_name_from_ip_address(current_user->homepage);
    }
    else
    {
        printf("User's Homepage (IP Address): %s\n", current_user->homepage);
    }
    return;
}