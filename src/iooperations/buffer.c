#include "../../include/iooperations/buffer.h"

#include <stdlib.h>
#include <string.h>

IOBuffer buffer_ctor(int bsize)
{
    IOBuffer b = (IOBuffer)malloc(sizeof(struct buffer_struct));
    memset(b, 0, sizeof(struct buffer_struct));
    b->bfr = (char *)calloc(bsize, sizeof(char));
    return b;
}

void buffer_dtor(IOBuffer obj)
{
    free(obj->bfr);
    free(obj);
}
