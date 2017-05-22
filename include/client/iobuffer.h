#ifndef IOBUFFER_H_INCLUDED
#define IOBUFFER_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 0x10000
typedef struct iobuffer_struct *IOBuffer;

IOBuffer iobuffer_ctor();
void iobuffer_dtor(IOBuffer obj);

struct iobuffer_struct
{
    char buffer[BUFF_SIZE];
    int offset;
};

#endif // IOBUFFER_H_INCLUDED
