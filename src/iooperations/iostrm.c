#include "../../include/iooperations/iostrm.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "../../include/iooperations/buffer.h"

#define BUFF_SIZE 5000

struct iostream_struct
{
    int streamopen;  // condition for shutting down the thread
    int tret;

    int sockfd;
    int portno;
    int n;
    struct sockaddr_in *serv_addr;
    struct hostent *server;

    IOBuffer inbuffer;
    IOBuffer outbuffer;
};

/** writes data to the iobuffer. locks the mutex during the write */
static int write_to_buffer(IOBuffer self, char * data, int len);
/** Thread control */
static void *iostrm_trun(void *ptr);

static pthread_mutex_t mutx;
static pthread_cond_t cond;

IOStream iostrm_ctor(const char * hostname, unsigned int port)
{
    IOStream s = (IOStream)malloc(sizeof(struct iostream_struct));

    /* socket addres */
    s->serv_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    memset((char *) s->serv_addr, 0, sizeof(struct sockaddr_in));
    s->serv_addr->sin_family = AF_INET;

    /* Port nad hostname */
    s->portno = port;
    s->server = gethostbyname(hostname);
    if (s->server == NULL)
    {
        fprintf(stderr, "ERROR no such host %s\n", hostname);
        exit(0);
    }
    bcopy((char *)s->server->h_addr, (char *)&s->serv_addr->sin_addr.s_addr,
         s->server->h_length);
    s->serv_addr->sin_port = htons(s->portno);

    /* in and out streams */
    s->inbuffer = buffer_ctor(BUFF_SIZE);
    s->outbuffer = buffer_ctor(BUFF_SIZE);

    return s;
}

void iostrm_dtor(IOStream obj)
{
    buffer_dtor(obj->inbuffer);
    obj->inbuffer = NULL;
    buffer_dtor(obj->outbuffer);
    obj->outbuffer = NULL;
    free(obj->serv_addr);
    obj->serv_addr = NULL;
    free(obj);
}

/* reads len bytes of data from the standard input */
int stdinread(IOStream self, int len)
{
    int read_len = len < BUFF_SIZE ? len : BUFF_SIZE;
    char buff[read_len];
    char * tmp = fgets(buff, read_len, stdin);
    if (tmp != NULL)
    {
        add_data(self->outbuffer, buff, strlen(buff));
        write_to_buffer(self->outbuffer, tmp, strlen(tmp));
        return 1;
    }
    return -1;
}

/* write offset bytes of data from buffer to socket */
int socketwrite(IOStream self)
{
    int flag = 0;
    int buff_len = get_used_size(self->outbuffer);
    int byteswritten = write(self->sockfd, get_data(self->outbuffer), buff_len);
    if (byteswritten != buff_len)
        flag = 1; // could not write entire buffer to socket
    else if (byteswritten < 0)
        flag = -1; // could not write any data to socket
    reset(self->outbuffer);
    return flag;
}

/* read len bytes of data from socket to buffer */
int socketread(IOStream self, int len)
{
    int flag = 0, len_read;
    if (len <= BUFF_SIZE)
        len_read = len;
    else
    {
        len_read = BUFF_SIZE;
        flag = 2; // buffer size not big enough
    }
    int bytesread = read(self->sockfd, get_data(self->inbuffer), len_read);
    if (bytesread >= 0)
    {
        if (bytesread != len_read)
            flag |= 1; // could not read all data
    }
    else
        flag = -1; // an error occured
    return flag;
}

/* opens the current (TCP) socket with current port and address */
int openSocket(IOStream self)
{
    self->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bcopy((char *)self->server->h_addr, (char *)&self->serv_addr->sin_addr.s_addr,
          self->server->h_length);
    self->serv_addr->sin_port = htons(self->portno);
    return self->sockfd;
}

/* closes the socket */
int closeSocket(IOStream self)
{
    self->streamopen = 0; // close stream
    pthread_cond_signal(&cond); // notify thread
    return close(self->sockfd);
}

/* attemps to connect the socket to the port and address */
int connectSock(IOStream self)
{
    return connect(self->sockfd, (struct sockaddr *)self->serv_addr,
                   sizeof(struct sockaddr_in));
}

/* start the thread */
void iostrm_tstart(IOStream self, pthread_t *thrd)
{
    self->streamopen = 1;
    self->tret = pthread_create(thrd, NULL,
                                iostrm_trun, (void *)self);
}

/* run the thread */
static void *iostrm_trun(void *ptr)
{
    IOStream self = (IOStream)ptr;
    while(self->streamopen)
    {
        pthread_mutex_lock(&mutx); // start of synchronize block in java
        if (get_used_size(self->outbuffer) == 0)
            pthread_cond_wait(&cond, &mutx);
        pthread_mutex_unlock(&mutx); // end of synchronize block in java
        if (self->streamopen)
            if (socketwrite(self) < 0)
                fprintf(stderr, "ERROR writing to socket");
    }
    return NULL;
}

/* writes len bytes from data to buffer */
static int write_to_buffer(IOBuffer bfr, char * data, int len)
{
    if (len > BUFF_SIZE - get_used_size(bfr))
        return -1; // buffer overflow
    pthread_mutex_lock(&mutx); // lock to this thread
    add_data(bfr, data, len);

    pthread_mutex_unlock(&mutx); // unlock
    pthread_cond_signal(&cond); // notify thread
    return 1;
}
/** resets the instream buffer */
void reset_inbuffer(IOStream self)
{
    reset(self->inbuffer);
}
/** resets the outstream buffer */
void reset_outbuffer(IOStream self)
{
    reset(self->outbuffer);
}

/* will be removed later */
void print_inbuffer(IOStream self)
{
    printf("%s\n", get_data(self->inbuffer));
}
