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
        printf("Bind failed\n");
    }

    int listenResult = listen(socket_fd, 10);

    struct sockaddr_in *clientAddress = malloc(sizeof(struct sockaddr_in));
    socklen_t sizeClientAddress = sizeof(*clientAddress);
    int acceptResult = accept(socket_fd, (struct sockaddr *)clientAddress, &sizeClientAddress);

    char *buffer = malloc(10 * sizeof(char));
    size_t recvlen = 9;
    buffer[9] = '\0';
    while (true)
    {
        ssize_t recvResult = recv(acceptResult, buffer, recvlen, 0);
        if (recvResult > 0)
        {
            buffer[recvResult] = '\0';
            printf("Hola");
            printf("Response from buffer %s\n", buffer);
        }
        else
        {
            break;
        }
    }

    free(address);
    free(clientAddress);
    close(socket_fd);
    close(acceptResult);
}
