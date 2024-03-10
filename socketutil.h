#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

struct AcceptedSocket
{
    int __fd;
    struct sockaddr_in *address;
    int error;
    bool successfull;
};

int createTCPIpv4socket();
struct sockaddr_in *createIPV4Address(char *ip, int PORT);
struct AcceptedSocket *acceptedClient(int __sid);
void acceptIncomingRequest(int __fd);
void recieveMessageOnThread(int __fd);
void recieveMessageFromClient(int __fd);
void recieveMessageFromClient(int __fd);
void broadCastMessage(char *buffer, int __fd);

#endif