#include "socketutil.h"

#define true 1

int main()
{
    int socket_fd = createTCPIpv4socket();
    struct sockaddr_in *address = createIPV4Address("127.0.0.1", 2001);

    int bindResult = bind(socket_fd, (struct sockaddr *)address, sizeof(*address));
    if (bindResult == 0)
    {
        printf("Socket is bound successfully\n");
    }
    else
    {
        perror("Bind failed\n");
        return -1;
    }

    int listenResult = listen(socket_fd, 10);
    if (listenResult == 0)
    {
        printf("Listenig on socket\n");
    }
    else if (listenResult == -1)
    {
        perror("Error in listening socket");
        return -1;
    }

    acceptIncomingRequest(socket_fd);

    free(address);
    close(socket_fd);
}
