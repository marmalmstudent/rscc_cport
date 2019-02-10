#ifndef PACKETCONSTRUCT_H
#define PACKETCONSTRUCT_H

#include "iooperations/buffer.h"

/** Handles creating packets. one should create packets and then get the
    packet data, which can be sent to the server using the functions in
    iostrm.h
*/

typedef struct pktcnstr *PacketConstruct;

PacketConstruct pktcnstr_ctor();
void pktcnstr_dtor(PacketConstruct obj);

/** initializes packet and writes the packet id to the buffer.
    allocates space at the beginning of the buffer where the
    packet size can be written to. */
int createPacket(PacketConstruct self, IOBuffer buff, int packet_id);

/** adds the packet size to the front of the packet. */
void formatPacket(PacketConstruct self, IOBuffer buff);

/** creates a packet that asks the server if it is available */
void makeSessionPacket(PacketConstruct self, IOBuffer buff,
                        const char *chrname);

#endif // PACKETCONSTRUCT_H
