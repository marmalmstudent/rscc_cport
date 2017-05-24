#ifndef IOBUFFER_H_INCLUDED
#define IOBUFFER_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFF_SIZE 0x10000
typedef struct iobuffer_struct *IOBuffer;

/** Constructor */
IOBuffer iobuffer_ctor(pthread_mutex_t *mutx, pthread_cond_t *cond);
/** Destructor */
void iobuffer_dtor(IOBuffer obj);

/** The IOBuffer declaration */
struct iobuffer_struct
{
    pthread_mutex_t *mutx;
    pthread_cond_t *cond;

    char buffer[BUFF_SIZE];
    int offset;

    int (* write_to_buffer)(IOBuffer self, char * data, int len);
};

#endif // IOBUFFER_H_INCLUDED
