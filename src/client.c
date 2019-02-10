#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <rsc/io/strops.h>

#include "iooperations/iostrm.h"
#include "iooperations/crypto.h"
#include "packetconstruct.h"

#include "client.h"

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
  fprintf(stderr, "%s\n", msg);
  exit(0);
}

/** prints the contents of the input buffer */
static void print_server_response(Client c);

static int
login()
{
  char username[20]; memset(username, 0, 20);
  char password[20]; memset(password, 0, 20);
  char user[20], pass[20];
  printf("username: ");
  scanf("%s", username);
  addCharacters(username, user, 20);
  printf("password: ");
  scanf("%s", password);
  addCharacters(password, pass, 20);
  return 0;
}

int main(int argc, char *argv[])
{
  if (argc < 3) { // insufficient arguments
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }
  Client c = client_ctor(argv[1], (unsigned int)atoi(argv[2]));

  if (openSocket(c->stream) < 0)
    error("ERROR opening socket");
  if (connectSock(c->stream) < 0)
    error("ERROR connecting");

  /* start thread, it writes to the socket */
  iostrm_tstart(c->stream, &c->thrd);

  /* for testing. trying to see if the enctryption and
     packet part works */
  makeSessionPacket(c->pkt_out, c->outbuffer, "admin");

  printf("Please enter the message: ");
  if (stdinread(c, 1234) < 0)
    error("ERROR reading from standard input");
  /* write output buffer to sream output buffer.
     the output thread should send the message. */
  put_bfr_out(c->stream, c->outbuffer);

  // read from socket
  if (socketread(c->stream, 1234) < 0)
    error("ERROR reading from socket");
  get_bfr_in(c->stream, c->inbuffer);
  print_server_response(c);

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

static void print_server_response(Client c)
{
  char print_dat[get_used(c->inbuffer)+1];
  get_data(c->inbuffer, print_dat);
  print_dat[get_used(c->inbuffer)] = 0;
  printf("%s\n", print_dat);
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
