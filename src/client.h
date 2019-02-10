#ifndef CLIENT_H
#define CLIENT_H

#include <stdbool.h>
#include <stdint.h>

#include "iooperations/iostrm.h"
#include "iooperations/crypto.h"
#include "packetconstruct.h"

struct rsc_client {
  IOStream stream;
  struct rsa_crypto dencrpt;
  PacketConstruct pkt_out;
  pthread_t thrd;

  IOBuffer inbuffer;
  IOBuffer outbuffer;
};

/** Constructor */
bool client_ctor(struct rsc_client *client, char const *hostname, uint16_t port);
/** Destructor */
void client_dtor(struct rsc_client *client);

#endif // CLIENT_H
