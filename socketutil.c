#include "socketutil.h"

int createTCPIpv4socket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

struct sockaddr_in *createIPV4Address(char *ip, int PORT)
{
    char *ip_address = ip;
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(PORT);
    inet_pton(AF_INET, ip_address, &address->sin_addr.s_addr);

    return address;
}