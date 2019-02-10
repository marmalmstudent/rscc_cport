#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <rsc/io/strops.h>

#include "iooperations/buffer.h"

#include "packetconstruct.h"

#define BUFF_SIZE 5000
#define PKT_LEN_SIZE 2 /* packet length allocated 2 bytes */
#define PKT_ID_SIZE 1 /* packet length allocated 2 bytes */

struct pktcnstr
{
    int packet_start;
};

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

int createPacket(PacketConstruct self, IOBuffer buff, int packet_id)
{
  if ((self->packet_start = get_used(buff)) > 4 * BUFF_SIZE / 5) {
    return -1;
  }
  push_alloc(buff, PKT_LEN_SIZE); /* allocate memory for packet size defintion */
  put_1_byte(buff, (unsigned char) packet_id);
  return 1;
}

void formatPacket(PacketConstruct self, IOBuffer buff)
{
  int offset = get_used(buff);
  unsigned int pkg_len = offset - self->packet_start - PKT_LEN_SIZE;
  char tmp[] = {
    (uint8_t) ((pkg_len >> 8) & 0xff),
    (uint8_t) (pkg_len & 0xff)
  };
  pop_alloc(buff, tmp, 2);
  dealloc(buff);
  self->packet_start = offset;
}
