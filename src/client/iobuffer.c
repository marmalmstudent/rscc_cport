#include "../../include/client/iobuffer.h"

/** writes len bytes from data to buffer */
static int write_to_buffer(IOBuffer self, char * data, int len);

IOBuffer iobuffer_ctor()
{
    IOBuffer b = (IOBuffer)malloc(sizeof(struct iobuffer_struct));
    memset(b->buffer, 0, BUFF_SIZE);
    b->write_to_buffer = write_to_buffer;
    return b;
}

void iobuffer_dtor(IOBuffer obj)
{
    free(obj);
}

/* writes len bytes from data to buffer */
static int write_to_buffer(IOBuffer self, char * data, int len)
{
    if (len > BUFF_SIZE - self->offset)
        return -1;
    while(len-- > 0)
        self->buffer[self->offset++] = *(data++);
    return 1;
}
