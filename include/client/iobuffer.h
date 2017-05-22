#ifndef IOBUFFER_H
#define IOBUFFER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFF_SIZE 0x10000
typedef struct iobuffer_struct *IOBuffer;

IOBuffer iobuffer_ctor();
void iobuffer_dtor(void *obj);

struct iobuffer_struct
{
    char buffer[BUFF_SIZE];
    int offset;
    const int buffsiz;
};

#endif // IOBUFFER_H
