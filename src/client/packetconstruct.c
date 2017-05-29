#include "../../include/client/packetconstruct.h"

#include <stdlib.h>

#include "../../include/iooperations/buffer.h"

#define BUFF_SIZE 5000

struct pktcnstr
{
    IOBuffer buff;
};

PacketConstruct pktcnstr_ctor()
{
    PacketConstruct pc = (PacketConstruct)malloc(sizeof(struct pktcnstr));
    pc->buff = buffer_ctor(BUFF_SIZE);
    return pc;
}

void pktcnstr_dtor(PacketConstruct obj)
{
    buffer_dtor(obj->buff);
    obj->buff = NULL;
    free(obj);
}
