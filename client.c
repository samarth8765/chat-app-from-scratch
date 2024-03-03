#include "socketutil.h"

int main()
{
    int a = 4;
    int socket_FD = createTCPIpv4socket();
    if (socket_FD < 0)
    {
        perror("Socket creation failed");
        return -1;
    }
    printf("Socket FD is %d\n", socket_FD);

    struct sockaddr_in *address = createIPV4Address("127.0.0.1", 8080);

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

    char *message = "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n";
    send(socket_FD, message, strlen(message), 0);

    char recv_msg[1024];
    recv(socket_FD, recv_msg, 1024, 0);
    printf("Response\n %s", recv_msg);
    free(address);

    return 0;
}
