#include "socketutil.h"
struct AcceptedSocket *clientSockets[10];
ssize_t clientSocketsCount = 0;

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

struct AcceptedSocket *acceptedClient(int __sid)
{
    struct sockaddr_in *clientAddress = malloc(sizeof(struct sockaddr_in));
    socklen_t sizeClientAddress = sizeof(*clientAddress);
    int acceptResult = accept(__sid, (struct sockaddr *)clientAddress, &sizeClientAddress);

    struct AcceptedSocket *acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->__fd = acceptResult;
    acceptedSocket->address = clientAddress;
    acceptedSocket->successfull = acceptResult > 0;
    if (!acceptedSocket->successfull)
    {
        acceptedSocket->error = acceptResult;
    }

    return acceptedSocket;
}

void acceptIncomingRequest(int socket_fd)
{
    while (true)
    {
        struct AcceptedSocket *acceptResult = acceptedClient(socket_fd);
        if (!acceptResult->successfull)
        {
            perror("Error occured during accepting the client");
            return;
        }
        else
        {
            char *newMemberJoinNotification = "New client connected joined\n";
            printf("%s", newMemberJoinNotification);
            broadCastMessage(newMemberJoinNotification, acceptResult->__fd);
        }
        clientSockets[clientSocketsCount++] = acceptResult;
        recieveMessageOnThread(acceptResult->__fd);
    }
}

void recieveMessageOnThread(int __fd)
{
    pthread_t id;
    pthread_create(&id, NULL, recieveMessageFromClient, __fd);
}

void recieveMessageFromClient(int __fd)
{
    char *buffer = malloc(1024 * sizeof(char));
    size_t recvlen = 1023;
    while (true)
    {
        ssize_t recvResult = recv(__fd, buffer, recvlen, 0);
        if (recvResult > 0)
        {
            buffer[recvResult] = '\0';
            printf("Response from buffer %s", buffer);
            broadCastMessage(buffer, __fd);
        }
        else
        {
            break;
        }
    }
    close(__fd);
}

void broadCastMessage(char *buffer, int __fd)
{
    for (int i = 0; i < clientSocketsCount; i++)
    {
        if (clientSockets[i]->__fd != __fd)
        {
            send(clientSockets[i]->__fd, buffer, strlen(buffer), 0);
        }
    }
}
