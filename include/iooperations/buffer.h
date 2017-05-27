#ifndef IOBUFFER_H
#define IOBUFFER_H

typedef struct buffer_struct *IOBuffer;

/** Constructor */
IOBuffer buffer_ctor(int len);
/** Destructor */
void buffer_dtor(IOBuffer obj);

struct buffer_struct
{
    char *bfr;
    int offset;
};

#endif // IOBUFFER_H
