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
    int packet_start;
};

/** initializes packet and writes the packet id to the buffer.
    allocates space at the beginning of the buffer where the
    packet size can be written to. */
static int createPacket(PacketConstruct self, IOBuffer buff, int packet_id);
/** adds the packet size to the front of the packet. */
static void formatPacket(PacketConstruct self, IOBuffer buff);

PacketConstruct pktcnstr_ctor()
{
    PacketConstruct pc = (PacketConstruct)malloc(sizeof(struct pktcnstr));
    pc->packet_start = 0;
    return pc;
}

void pktcnstr_dtor(PacketConstruct obj)
{
    free(obj);
    obj = NULL;
}

static int createPacket(PacketConstruct self, IOBuffer buff, int packet_id)
{
    if ((self->packet_start = get_used(buff)) > 4 * BUFF_SIZE / 5)
        return -1;
    push_alloc(buff, PKT_LEN_SIZE); /* allocate memory for packet size defintion */
    put_1_byte(buff, (unsigned char) packet_id);
    return 1;
}

static void formatPacket(PacketConstruct self, IOBuffer buff)
{
    int offset = get_used(buff);
    unsigned int pkg_len = offset - self->packet_start - PKT_LEN_SIZE;
    char tmp[] = {
        (unsigned char) ((pkg_len >> 8) & 0xff),
        (unsigned char) (pkg_len & 0xff)
    };
    pop_alloc(buff, tmp, 2);
    dealloc(buff);
    self->packet_start = offset;
}

void makeSessionPacket(PacketConstruct self, IOBuffer buff, const char *chrname)
{
    createPacket(self, buff, 32);
    /* packet data */
    long l = stringLength12ToLong(chrname);
    put_1_byte(buff, (unsigned char) (l >> 16 & 31L));
    char *classname = "CLIENT.MUDCLIENT";
    put_data(buff, classname, strlen(classname));
    /* add packet length to data */
    formatPacket(self, buff);
}
