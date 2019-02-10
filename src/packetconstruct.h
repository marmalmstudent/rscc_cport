#ifndef PACKETCONSTRUCT_H
#define PACKETCONSTRUCT_H

#include <stdbool.h>

#include "iooperations/buffer.h"

/** Handles creating packets. one should create packets and then get the
    packet data, which can be sent to the server using the functions in
    iostrm.h
*/


struct packet_construct {
    int packet_start;
};

bool pktcnstr_ctor(struct packet_construct *construct);
void pktcnstr_dtor(struct packet_construct *construct);

/** initializes packet and writes the packet id to the buffer.
    allocates space at the beginning of the buffer where the
    packet size can be written to. */
int createPacket(struct packet_construct *construct, struct data_buffer *buff, int packet_id);

/** adds the packet size to the front of the packet. */
void formatPacket(struct packet_construct *construct, struct data_buffer *buff);

/** creates a packet that asks the server if it is available */
void makeSessionPacket(struct packet_construct *construct, struct data_buffer *buff, char const *chrname);

#endif // PACKETCONSTRUCT_H
