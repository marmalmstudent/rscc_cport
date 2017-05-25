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

    if (c->stream->opnsock(c->stream) < 0)
        error("ERROR opening socket");
    if (c->stream->cnctsock(c->stream) < 0)
        error("ERROR connecting");

    /* start thread, it reads from the socket */
    c->stream->iostrm_tstart(c->stream);

    printf("Please enter the message: ");
    if (c->stream->stdinread(c->stream, BUFF_SIZE) < 0)
        error("ERROR reading from standard input");

    /* input thread should have sent message */

    // read from socket
    if (c->stream->socketread(c->stream, BUFF_SIZE) < 0)
         error("ERROR reading from socket");
    printf("%s\n", c->stream->inbuffer->buffer);

    c->stream->clssock(c->stream);
    pthread_join(*c->stream->thrd, NULL); // wait for thread to finish

    client_dtor(c);
    return 0;
}

Client client_ctor(const char * hostname, unsigned int port)
{
    Client c = (Client)malloc(sizeof(struct client_struct));
    c->stream = iostrm_ctor(hostname, port);
    c->dencrpt = dataencryption_ctor();
    return c;
}

void client_dtor(Client obj)
{
    iostrm_dtor(obj->stream);
    dataencryption_dtor(obj->dencrpt);
    free(obj);
}

void encryptPacketWithKeys(mpz_t k, mpz_t m)
{
    mpz_import (z, sizeof(arr), 1, sizeof(char), 0, 0, arr);
}
