#ifndef IOBUFFER_H_INCLUDED
#define IOBUFFER_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 0x10000
typedef struct iobuffer_struct *IOBuffer;

/** Constructor */
IOBuffer iobuffer_ctor();
/** Destructor */
void iobuffer_dtor(IOBuffer obj);

/** The IOBuffer declaration */
struct iobuffer_struct
{
    char buffer[BUFF_SIZE];
    int offset;

    int (* write_to_buffer)(IOBuffer self, char * data, int len);
};

#endif // IOBUFFER_H_INCLUDED
