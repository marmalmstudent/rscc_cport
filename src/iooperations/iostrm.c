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
struct bfr
{
    char *data;
    int offset;
};
struct iostream_struct
{
    int streamopen;  // condition for shutting down the thread
    int tret;

    int sockfd;
    int portno;
    int n;
    struct sockaddr_in *serv_addr;
    struct hostent *server;

    struct bfr bfr_in; // where data from the socket is put
    struct bfr bfr_out; // where data to the socket is put
};

/** Thread control */
static void *iostrm_trun(void *ptr);

/** writes src data to b's buffer */
static void bfr_add_data(struct bfr *b, const IOBuffer src);
/** resets the buffer offset to zero and overwrites data with zeros */
static void bfr_reset(struct bfr *b);

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
    s->bfr_in.data = (char *)calloc(BUFF_SIZE, sizeof(char));
    s->bfr_in.offset = 0;
    s->bfr_out.data = (char *)calloc(BUFF_SIZE, sizeof(char));
    s->bfr_out.offset = 0;

    return s;
}

void iostrm_dtor(IOStream obj)
{
    free(obj->serv_addr);
    obj->serv_addr = NULL;

    free(obj->bfr_in.data);
    obj->bfr_in.data = NULL;
    free(obj->bfr_out.data);
    obj->bfr_out.data = NULL;
    free(obj);
    obj = NULL;
}

static void bfr_add_data(struct bfr *b, const IOBuffer src)
{
    copy_data(src, b->data);
    b->offset += get_used_size(src);
}

static void bfr_reset(struct bfr *b)
{
    memset(b->data, 0, b->offset);
    b->offset = 0;
}

/* write offset bytes of data from buffer to socket */
int socketwrite(IOStream self)
{
    int flag = 0;
    int buff_len = self->bfr_out.offset;
    int byteswritten = write(
            self->sockfd, self->bfr_out.data, buff_len);
    bfr_reset(&self->bfr_out);
    if (byteswritten != buff_len)
        flag = 1; // could not write entire buffer to socket
    else if (byteswritten < 0)
        flag = -1; // could not write any data to socket
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
        flag |= 2; // buffer size not big enough
    }
    int bytesread = read(self->sockfd,
                         self->bfr_in.data + self->bfr_in.offset,
                         len_read);
    if (bytesread >= 0)
    {
        if (bytesread != len_read)
            flag |= 1; // could not read all data
        self->bfr_in.offset += bytesread;
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
        if (self->bfr_out.offset == 0)
            pthread_cond_wait(&cond, &mutx);
        pthread_mutex_unlock(&mutx); // end of synchronize block in java
        if (self->streamopen)
            if (socketwrite(self) < 0)
                fprintf(stderr, "ERROR writing to socket");
    }
    return NULL;
}


void put_bfr_out(IOStream self, const IOBuffer src)
{
    pthread_mutex_lock(&mutx); // lock to this thread
    bfr_add_data(&self->bfr_out, src);
    pthread_mutex_unlock(&mutx); // unlock
    pthread_cond_signal(&cond); // notify thread
}

void get_bfr_in(IOStream self, IOBuffer dst)
{
    put_data(dst, self->bfr_in.data, self->bfr_in.offset);
    bfr_reset(&self->bfr_in);
}
