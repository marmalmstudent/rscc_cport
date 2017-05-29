#ifndef PACKETCONSTRUCT_H
#define PACKETCONSTRUCT_H

/** Handles creating packets. one should create packets and then get the
    packet data, which can be sent to the server using the functions in
    iostrm.h
*/

typedef struct pktcnstr *PacketConstruct;

PacketConstruct pktcnstr_ctor();
void pktcnstr_dtor(PacketConstruct obj);

#endif // PACKETCONSTRUCT_H
