#include "socketutil.h"

#define true 1

int main()
{
    int socket_FD = createTCPIpv4socket();
    if (socket_FD < 0)
    {
        perror("Socket creation failed");
        return -1;
    }
    printf("Socket FD is %d\n", socket_FD);

    struct sockaddr_in *address = createIPV4Address("127.0.0.1", 2001);
    int res = connect(socket_FD, (struct sockaddr *)address, sizeof(*address));

    if (res == 0)
    {
        printf("Connection is successful\n");
    }
    else
    {
        perror("Connection failed");
        free(address);
        return -1;
    }

    recieveMessageFromServer(socket_FD);

    sendMessageToServer(socket_FD);

    free(address);
    close(socket_FD);

    return 0;
}
