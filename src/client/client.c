#include "../../include/client/client.h"

void error(const char *msg)
{
    fprintf(stderr, msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    { // insufficient arguments
       fprintf(stderr, "usage %s hostname port\n", argv[0]);
       exit(0);
    }
    Client c = client_ctor(argv[1], (unsigned int)atoi(argv[2]));

    if (openSocket(c->stream) < 0)
        error("ERROR opening socket");
    if (connectSock(c->stream) < 0)
        error("ERROR connecting");

    /* start thread, it reads from the socket */
    iostrm_tstart(c->stream, &c->thrd);

    printf("Please enter the message: ");
    if (stdinread(c->stream, 1234) < 0)
        error("ERROR reading from standard input");

    /* input thread should have sent message */

    // read from socket
    if (socketread(c->stream, 1234) < 0)
         error("ERROR reading from socket");
    print_inbuffer(c->stream);
    reset_inbuffer(c->stream);

    closeSocket(c->stream);
    pthread_join(c->thrd, NULL); // wait for thread to finish

    client_dtor(c);
    return 0;
}

Client client_ctor(const char * hostname, unsigned int port)
{
    Client c = (Client)malloc(sizeof(struct client_struct));
    c->stream = iostrm_ctor(hostname, port);
    c->dencrpt = crypto_ctor();
    return c;
}

void client_dtor(Client obj)
{
    iostrm_dtor(obj->stream);
    crypto_dtor(obj->dencrpt);
    free(obj);
}
