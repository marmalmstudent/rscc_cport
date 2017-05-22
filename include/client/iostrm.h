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

#define BUFF_SIZE 0x10000
typedef struct iostream_struct *IOStream;

IOStream iostrm_ctor(const char * hostname, unsigned int port);
void iostrm_dtor(IOStream obj);

static struct iobuffer_struct
{
    char buffer[BUFF_SIZE];
    int offset;
};

struct iostream_struct
{
    int sockfd;
    int portno;
    int n;
    struct sockaddr_in *serv_addr;
    struct hostent *server;

    struct iobuffer_struct *inbuffer;
    struct iobuffer_struct *outbuffer;

    int (* opnsock)(IOStream self);
    int (* clssock)(IOStream self);
    int (* cnctsock)(IOStream self);
    int (* stdinread)(IOStream self, int len);
    int (* socketwrite)(IOStream self);
    int (* socketread)(IOStream self, int len);
};

static int openSocket(IOStream self);
static int closeSocket(IOStream self);
static int connectSock(IOStream self);
static int stdinread(IOStream self, int len);
static int socketwrite(IOStream self);
static int socketread(IOStream self, int len);

static void error(const char *msg);

#endif // IOSTRM_H
