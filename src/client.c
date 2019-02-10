#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>

#include <rsc/io/strops.h>

#include "client.h"

#define BUFF_SIZE 5000

static double drand() {
  return rand() / (RAND_MAX + 1.0);
}

bool client_ctor(struct rsc_client *client, char const *hostname, uint16_t port)
{
  client->stream = iostrm_ctor(hostname, port);
  client->dencrpt = crypto_ctor();
  client->pkt_out = pktcnstr_ctor();
  client->inbuffer = buffer_ctor(BUFF_SIZE);
  client->outbuffer = buffer_ctor(BUFF_SIZE);
  return true;
}

void client_dtor(struct rsc_client *client)
{
  iostrm_dtor(client->stream);
  crypto_dtor(client->dencrpt);
  pktcnstr_dtor(client->pkt_out);
  buffer_dtor(client->inbuffer);
  buffer_dtor(client->outbuffer);
}

static int
login(struct rsc_client *client)
{
  char username[21];
  memset(username, 0, 21);
  sprintf(username, "%s", "user");
  char password[21];
  memset(password, 0, 21);
  sprintf(password, "%s", "password");

  char user[21];
  memset(user, 0, 21);
  addCharacters(username, user, 20);
  char pass[21];
  memset(pass, 0, 21);
  addCharacters(password, pass, 20);

  createPacket(client->pkt_out, client->outbuffer, 32);
  /* packet data */
  long l = stringLength12ToLong(user);
  put_1_byte(client->outbuffer, (uint8_t) (l >> 16 & 0x1f));
  put_string(client->outbuffer, "CLIENT.MUDCLIENT");
  /* add packet length to data */
  formatPacket(client->pkt_out, client->outbuffer);
  put_bfr_out(client->stream, client->outbuffer);
  reset(client->outbuffer);

  socketread(client->stream, 8);
  get_bfr_in(client->stream, client->inbuffer);
  uint64_t sessionID = get_8_bytes(client->inbuffer, endian_big);
  reset(client->inbuffer);
  if (sessionID == 0) {
    printf("Login server offline.\nPlease try again in a few mins\n");
    return 0;
  }
  printf("Session ID: %lu\n", sessionID);

  put_4_bytes(client->dencrpt->buff, (int32_t) (drand() * 99999999), endian_big);
  put_4_bytes(client->dencrpt->buff, (int32_t) (drand() * 99999999), endian_big);
  put_4_bytes(client->dencrpt->buff, (int32_t) (sessionID >> 32), endian_big);
  put_4_bytes(client->dencrpt->buff, (int32_t) sessionID, endian_big);
  put_4_bytes(client->dencrpt->buff, 0, endian_big); // UID
  put_string(client->dencrpt->buff, user);
  put_string(client->dencrpt->buff, pass);
  encryptPacketWithKeys(client->dencrpt);
  
  createPacket(client->pkt_out, client->outbuffer, 0);
  put_1_byte(client->outbuffer, 0);
  put_2_bytes(client->outbuffer, 25, endian_big); // client version
  put_buffer(client->dencrpt->buff, client->outbuffer, get_used(client->dencrpt->buff));
  formatPacket(client->pkt_out, client->outbuffer);
  put_bfr_out(client->stream, client->outbuffer);
  reset(client->outbuffer);
  
  socketread(client->stream, 1);
  get_bfr_in(client->stream, client->inbuffer);
  uint8_t loginResponse = get_1_byte(client->inbuffer);
  reset(client->inbuffer);
  printf(" - Login Response: %d\n", loginResponse);

  sleep(5);

  printf("Loggin out...\n");
  createPacket(client->pkt_out, client->outbuffer, 39);
  formatPacket(client->pkt_out, client->outbuffer);
  return 0;
}

int main(int argc, char *argv[])
{
  if (argc < 3) { // insufficient arguments
    fprintf(stderr, "usage %s [hostname] [port]\n", argv[0]);
    return EXIT_FAILURE;
  }
  struct rsc_client c;
  client_ctor(&c, argv[1], (uint16_t)atoi(argv[2]));

  if (openSocket(c.stream) < 0) {
    fprintf(stderr, "ERROR opening socket\n");
    return EXIT_FAILURE;
  }
  if (connectSock(c.stream) < 0) {
    fprintf(stderr, "ERROR connecting\n");
    return EXIT_FAILURE;
  }

  /* start thread, it writes to the socket */
  iostrm_tstart(c.stream, &c.thrd);

  login(&c);

  closeSocket(c.stream);
  pthread_join(c.thrd, NULL); // wait for thread to finish

  client_dtor(&c);
  return 0;
}
