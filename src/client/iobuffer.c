#include "../../include/client/iobuffer.h"

IOBuffer iobuffer_ctor()
{
    IOBuffer buff = (IOBuffer)malloc(sizeof(struct iobuffer_struct));
    memset(buff->buffer, 0, BUFF_SIZE); // init to zeros
    return buff;
}

void iobuffer_dtor(void *obj)
{
    free((IOBuffer)obj);
}
