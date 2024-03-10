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

    char *message = NULL;
    size_t message_len = 0;
    printf("Type the message and send\n");

    while (true)
    {
        ssize_t charCount = getline(&message, &message_len, stdin);
        if (charCount > 0 && strcmp(message, "exit\n") == 0)
            break;
        else
        {
            ssize_t messageSent = send(socket_FD, message, charCount, 0);
        }
    }

    free(address);
    close(socket_FD);

    return 0;
}
