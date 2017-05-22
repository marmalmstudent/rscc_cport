#include "../../include/client/client.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    Client c = client_ctor(argv[1], (unsigned int)atoi(argv[2]));

    if (c->opnsock(c) < 0)
        error("ERROR opening socket");
    if (c->server == NULL)
        error("ERROR, no such host\n");

    memset((char *) c->serv_addr, 0, sizeof(struct sockaddr_in)); // init to zeros
    c->serv_addr->sin_family = AF_INET;
    bcopy((char *)c->server->h_addr,
         (char *)&c->serv_addr->sin_addr.s_addr,
         c->server->h_length);
    c->serv_addr->sin_port = htons(c->portno);
    if (connect(c->sockfd, (struct sockaddr *)c->serv_addr,
                sizeof(struct sockaddr_in)) < 0)
        error("ERROR connecting");

    printf("Please enter the message: ");
    if (fgets(c->outbuff->buffer, BUFF_SIZE, stdin) == NULL)
        error("ERROR reading from standard input");

    // write input to socket
    c->n = write(c->sockfd, c->outbuff->buffer, BUFF_SIZE);
    if (c->n < 0)
         error("ERROR writing to socket");

    // read from socket
    c->n = read(c->sockfd, c->inbuff->buffer, BUFF_SIZE);
    if (c->n < 0)
         error("ERROR reading from socket");
    printf("%s\n", c->inbuff->buffer);
    close(c->sockfd);
    client_dtor(c);
    return 0;
}

Client client_ctor(const char * hostname, unsigned int port)
{
    Client c = (Client)malloc(sizeof(struct client_struct));
    c->serv_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    c->portno = port;
    c->server = gethostbyname(hostname);
    c->opnsock = &openSocket;
    c->inbuff = iobuffer_ctor();
    c->outbuff = iobuffer_ctor();
    return c;
}

void client_dtor(void *obj)
{
    iobuffer_dtor(((Client)obj)->inbuff);
    iobuffer_dtor(((Client)obj)->outbuff);
    free(((Client)obj)->serv_addr);
    free((Client)obj);
}

static int openSocket(Client self)
{
    self->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return self->sockfd < 0 ? 1 : 0;
}
