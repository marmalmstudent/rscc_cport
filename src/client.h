#ifndef CLIENT_H
#define CLIENT_H

#include <stdbool.h>
#include <stdint.h>

#include "iooperations/iostrm.h"
#include "iooperations/crypto.h"
#include "packetconstruct.h"

struct rsc_client {
  struct io_stream stream;
  struct rsa_crypto dencrpt;
  struct packet_construct pkt_out;
  pthread_t thrd;

  struct data_buffer inbuffer;
  struct data_buffer outbuffer;
};

/** Constructor */
bool client_ctor(struct rsc_client *client, char const *hostname, uint16_t port);
/** Destructor */
void client_dtor(struct rsc_client *client);

#endif // CLIENT_H
