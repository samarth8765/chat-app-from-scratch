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
            printf("%s", buffer);
            broadCastMessage(buffer, __fd);
        }
        else if (recvResult == -1)
        {
            // manage the broadcast if client exits
            break;
        }
    }
    close(__fd);
}

void broadCastMessage(char *buffer, int __fd)
{
    printf("%d", clientSocketsCount);
    for (int i = 0; i < clientSocketsCount; i++)
    {
        if (clientSockets[i]->__fd != __fd)
        {
            send(clientSockets[i]->__fd, buffer, strlen(buffer), 0);
        }
    }
}

void printMessageFromServer(int __fd)
{
    char buffer[1024];

    while (true)
    {
        ssize_t amountReceived = recv(__fd, buffer, 1024, 0);

        if (amountReceived > 0)
        {
            buffer[amountReceived] = 0;
            printf("%s", buffer);
        }

        if (amountReceived == 0)
            break;
    }

    close(__fd);
}

void recieveMessageFromServer(int __fd)
{
    pthread_t id;
    pthread_create(&id, NULL, printMessageFromServer, __fd);
}

void sendMessageToServer(int __fd)
{
    /*Get username from user*/
    char *buffer;
    size_t buffer_len = 0;
    ssize_t usernameCount = 0;
    while (true)
    {
        printf("Please enter your name\n");
        usernameCount = getline(&buffer, &buffer_len, stdin);
        if (usernameCount == 1 && buffer[0] == '\n')
        {
            continue;
        }
        else
        {
            buffer[usernameCount - 1] = ':';
            buffer[usernameCount] = ' ';
            buffer[usernameCount + 1] = '\0';
            break;
        }
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
            buffer = realloc(buffer, usernameCount + 1 + charCount);
            strcat(buffer, message);
            ssize_t messageSent = send(__fd, buffer, strlen(buffer), 0);
            buffer[usernameCount + 1] = '\0';
        }
    }
}
