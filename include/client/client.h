#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "iobuffer.h"


void error(const char *msg);

typedef struct client_struct *Client;

Client client_ctor(const char * hostname, unsigned int port);
void client_dtor(void *obj);

struct client_struct
{
    int sockfd;
    int portno;
    int n;
    struct sockaddr_in *serv_addr;
    struct hostent *server;
    IOBuffer inbuff;
    IOBuffer outbuff;

    int (* opnsock)(Client self);
};

static int openSocket(Client self);

#endif // CLIENT_H
