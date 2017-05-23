#include "../../include/client/iostrm.h"

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

IOStream iostrm_ctor(const char * hostname, unsigned int port)
{
    IOStream s = (IOStream)malloc(sizeof(struct iobuffer_struct));

    /* in and out streams */
    s->inbuffer = iobuffer_ctor();
    s->outbuffer = iobuffer_ctor();

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

    /* set struct functions */
    s->opnsock = &openSocket;
    s->clssock = &closeSocket;
    s->cnctsock = &connectSock;
    s->stdinread = &stdinread;
    s->socketwrite = &socketwrite;
    s->socketread = &socketread;
    return s;
}

void iostrm_dtor(IOStream obj)
{
    iobuffer_dtor(obj->inbuffer);
    iobuffer_dtor(obj->outbuffer);
    free(obj->serv_addr);
    free(obj);
}

/* reads len bytes of data from the standard input */
static int stdinread(IOStream self, int len)
{
    int read_len = len < BUFF_SIZE ? len : BUFF_SIZE;
    char * tmp = fgets(self->outbuffer->buffer, read_len, stdin);
    if (tmp != NULL)
    {
        self->outbuffer->write_to_buffer(self->outbuffer, tmp, strlen(tmp));
        return 1;
    }
    return -1;
}

/* write offset bytes of data from buffer to socket */
static int socketwrite(IOStream self)
{
    int flag = 0;
    int byteswritten = write(self->sockfd, self->outbuffer->buffer, self->outbuffer->offset);
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
    int bytesread = read(self->sockfd, self->inbuffer->buffer, len_read);
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
    return self->sockfd < 0 ? -1 : 1;
}

/* closes the socket */
static int closeSocket(IOStream self)
{
    return close(self->sockfd) < 0 ? -1 : 1;
}

/* attemps to connect the socket to the port and address */
static int connectSock(IOStream self)
{
    return connect(self->sockfd, (struct sockaddr *)self->serv_addr,
                   sizeof(struct sockaddr_in)) < 0 ? -1 : 1;
}
