#include "../../include/client/client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../../include/iooperations/iostrm.h"
#include "../../include/iooperations/crypto.h"
#include "../../include/client/packetconstruct.h"

#define BUFF_SIZE 5000

struct client_struct
{
    IOStream stream;
    Crypto dencrpt;
    PacketConstruct pkt_out;
    pthread_t thrd;

    IOBuffer inbuffer;
    IOBuffer outbuffer;
};

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

    /* for testing. trying to see if the enctryption and packet part works */
    makeSessionPacket(c->pkt_out, c->outbuffer, "admin");
    /* EOF */

    printf("Please enter the message: ");
    if (stdinread(c, 1234) < 0)
        error("ERROR reading from standard input");
    put_bfr_out(c->stream, c->outbuffer);


    /* input thread should have sent message */

    // read from socket
    if (socketread(c->stream, 1234) < 0)
         error("ERROR reading from socket");
    get_bfr_in(c->stream, c->inbuffer);
    char print_dat[get_used_size(c->inbuffer)+1];
    copy_data(c->inbuffer, print_dat);
    print_dat[get_used_size(c->inbuffer)] = 0;
    printf("%s\n", print_dat);

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
    c->pkt_out = pktcnstr_ctor();
    c->inbuffer = buffer_ctor(BUFF_SIZE);
    c->outbuffer = buffer_ctor(BUFF_SIZE);
    return c;
}

void client_dtor(Client obj)
{
    iostrm_dtor(obj->stream);
    crypto_dtor(obj->dencrpt);
    pktcnstr_dtor(obj->pkt_out);
    buffer_dtor(obj->inbuffer);
    buffer_dtor(obj->outbuffer);
    free(obj);
    obj = NULL;
}

/* reads len bytes of data from the standard input */
int stdinread(Client self, int len)
{
    int read_len = len < BUFF_SIZE ? len : BUFF_SIZE;
    char buff[read_len];
    char * tmp = fgets(buff, read_len, stdin);
    if (tmp != NULL)
    {
        put_data(self->outbuffer, tmp, strlen(tmp));
        return 1;
    }
    return -1;
}
