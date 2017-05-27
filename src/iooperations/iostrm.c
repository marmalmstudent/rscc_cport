#include "../../include/iooperations/iostrm.h"

/* private functions */
/** opens the current (TCP) socket with current port and address */
static int openSocket(IOStream self);
/** closes the socket */
static int closeSocket(IOStream self);
/** attemps to connect the socket to the port and address */
static int connectSock(IOStream self);
/** reads len bytes of data from the standard input */
static int stdinread(IOStream self, int len);
/** write offset bytes of data from buffer to socket */
static int socketwrite(IOStream self);
/** read len bytes of data from socket to buffer */
static int socketread(IOStream self, int len);
/** writes data to the iobuffer. locks the mutex during the write */
static int write_to_buffer(IOBuffer self, char * data, int len);

/** Thread control */
static void *iostrm_trun(void *ptr);
/** Thread start */
static void iostrm_tstart(IOStream self);

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

    /* set struct functions */
    s->opnsock = &openSocket;
    s->clssock = &closeSocket;
    s->cnctsock = &connectSock;
    s->stdinread = &stdinread;
    s->socketwrite = &socketwrite;
    s->socketread = &socketread;
    /* thread functions */
    s->iostrm_tstart = &iostrm_tstart;
    s->iostrm_trun = &iostrm_trun;
    return s;
}

void iostrm_dtor(IOStream obj)
{
    buffer_dtor(obj->inbuffer);
    buffer_dtor(obj->outbuffer);
    free(obj->serv_addr);
    free(obj);
}

/* reads len bytes of data from the standard input */
static int stdinread(IOStream self, int len)
{
    int read_len = len < BUFF_SIZE ? len : BUFF_SIZE;
    char * tmp = fgets(self->outbuffer->bfr, read_len, stdin);
    if (tmp != NULL)
    {
        write_to_buffer(self->outbuffer, tmp, strlen(tmp));
        return 1;
    }
    return -1;
}

/* write offset bytes of data from buffer to socket */
static int socketwrite(IOStream self)
{
    int flag = 0;
    int byteswritten = write(self->sockfd, self->outbuffer->bfr, self->outbuffer->offset);
    if (byteswritten != self->outbuffer->offset)
        flag = 1; // could not write entire buffer to socket
    else if (byteswritten < 0)
        flag = -1; // could not write any data to socket
    self->outbuffer->offset = 0;
    return flag;
}

/* read len bytes of data from socket to buffer */
static int socketread(IOStream self, int len)
{
    int flag = 0, len_read;
    if (len <= BUFF_SIZE)
        len_read = len;
    else
    {
        len_read = BUFF_SIZE;
        flag = 2; // buffer size not big enough
    }
    int bytesread = read(self->sockfd, self->inbuffer->bfr, len_read);
    if (bytesread >= 0)
    {
        if (bytesread != len_read)
            flag |= 1; // could not read all data
        self->inbuffer->offset = 0;
    }
    else
        flag = -1; // an error occured
    return flag;
}

/* opens the current (TCP) socket with current port and address */
static int openSocket(IOStream self)
{
    self->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bcopy((char *)self->server->h_addr, (char *)&self->serv_addr->sin_addr.s_addr,
          self->server->h_length);
    self->serv_addr->sin_port = htons(self->portno);
    return self->sockfd;
}

/* closes the socket */
static int closeSocket(IOStream self)
{
    self->streamopen = 0; // close stream
    pthread_cond_signal(&cond); // notify thread
    return close(self->sockfd);
}

/* attemps to connect the socket to the port and address */
static int connectSock(IOStream self)
{
    return connect(self->sockfd, (struct sockaddr *)self->serv_addr,
                   sizeof(struct sockaddr_in));
}

/* start the thread */
static void iostrm_tstart(IOStream self)
{
    self->tret = pthread_create(&self->thrd, NULL,
                                self->iostrm_trun, self);
    self->streamopen = 1;
}

/* run the thread */
static void *iostrm_trun(void *ptr)
{
    IOStream self = (IOStream)ptr;
    while(self->streamopen)
    {
        pthread_mutex_lock(&mutx); // start of synchronize block in java
        if (self->outbuffer->offset == 0)
            pthread_cond_wait(&cond, &mutx);
        pthread_mutex_unlock(&mutx); // end of synchronize block in java
        if (self->streamopen)
            if (self->socketwrite(self) < 0)
                fprintf(stderr, "ERROR writing to socket");
    }
    return NULL;
}

/* writes len bytes from data to buffer */
static int write_to_buffer(IOBuffer self, char * data, int len)
{
    if (len > BUFF_SIZE - self->offset)
        return -1; // buffer overflow
    pthread_mutex_lock(&mutx); // lock to this thread
    memcpy(self->bfr + self->offset, data, len);
    self->offset += len;

    pthread_mutex_unlock(&mutx); // unlock
    pthread_cond_signal(&cond); // notify thread
    return 1;
}
