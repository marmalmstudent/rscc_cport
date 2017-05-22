#include "../../include/client/iobuffer.h"

IOBuffer iobuffer_ctor()
{
    IOBuffer b = (IOBuffer)malloc(sizeof(struct iobuffer_struct));
    memset(b->buffer, 0, BUFF_SIZE);
    return b;
}

void iobuffer_dtor(IOBuffer obj)
{
    free(obj);
}
