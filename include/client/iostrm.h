#ifndef IOSTRM_H
#define IOSTRM_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "iobuffer.h"

typedef struct iostream_struct *IOStream;

void error(const char *msg);
IOStream iostrm_ctor(const char * hostname, unsigned int port);
void iostrm_dtor(IOStream obj);

struct iostream_struct
{
    int sockfd;
    int portno;
    int n;
    struct sockaddr_in *serv_addr;
    struct hostent *server;

    IOBuffer inbuffer;
    IOBuffer outbuffer;

    int (* opnsock)(IOStream self);
    int (* clssock)(IOStream self);
    int (* cnctsock)(IOStream self);
    int (* stdinread)(IOStream self, int len);
    int (* socketwrite)(IOStream self);
    int (* socketread)(IOStream self, int len);
};

#endif // IOSTRM_H
