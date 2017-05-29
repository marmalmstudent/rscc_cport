#include "../../include/client/packetconstruct.h"

#include <stdlib.h>
#include <string.h>

#include "../../include/iooperations/buffer.h"
#include "../../include/dataoperations/strops.h"

#define BUFF_SIZE 5000
#define PKT_LEN_SIZE 2 /* packet length allocated 2 bytes */
#define PKT_ID_SIZE 1 /* packet length allocated 2 bytes */

struct pktcnstr
{
    IOBuffer buff;
    int packet_start;
};

/** initializes packet and writes the packet id to the buffer.
    allocates space at the beginning of the buffer where the
    packet size can be written to. */
static int createPacket(PacketConstruct self, int packet_id);
/** adds the packet size to the front of the packet. */
static void formatPacket(PacketConstruct self);

PacketConstruct pktcnstr_ctor()
{
    PacketConstruct pc = (PacketConstruct)malloc(sizeof(struct pktcnstr));
    memset(pc, 0, sizeof(struct pktcnstr));
    pc->buff = buffer_ctor(BUFF_SIZE);
    return pc;
}

void pktcnstr_dtor(PacketConstruct obj)
{
    buffer_dtor(obj->buff);
    obj->buff = NULL;
    free(obj);
}

static int createPacket(PacketConstruct self, int packet_id)
{
    if ((self->packet_start = get_used_size(self->buff)) > 4 * BUFF_SIZE / 5)
        return -1;
    push_step(self->buff, PKT_LEN_SIZE); /* allocate memory for packet size defintion */
    putUnsigned1Byte(self->buff, (unsigned char) packet_id);
    return 1;
}

static void formatPacket(PacketConstruct self)
{
    int offset = get_used_size(self->buff);
    unsigned int pkg_len = offset - self->packet_start - PKT_LEN_SIZE;
    char tmp[] = {
        (unsigned char) ((pkg_len >> 8) & 0xff),
        (unsigned char) (pkg_len & 0xff)
    };
    pop_step(self->buff, tmp, 2);
    reset_step(self->buff);
    self->packet_start = offset;
}

void makeSessionPacket(PacketConstruct self, const char *chrname)
{
    createPacket(self, 32);
    /* packet data */
    long l = stringLength12ToLong(chrname);
    putUnsigned1Byte(self->buff, (unsigned char) (l >> 16 & 31L));
    char *classname = "CLIENT.MUDCLIENT";
    add_data(self->buff, classname, strlen(classname));
    /* add packet length to data */
    formatPacket(self);
}

int getPacketDataLen(PacketConstruct self)
{
    return get_used_size(self->buff);
}

const char *getPacketData(PacketConstruct self)
{
    return get_data(self->buff);
}
