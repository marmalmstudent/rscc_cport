#include "../../include/client/iobuffer.h"

/** writes len bytes from data to buffer */
static int write_to_buffer(IOBuffer self, char * data, int len);

IOBuffer iobuffer_ctor(pthread_mutex_t *mutx, pthread_cond_t *cond)
{
    IOBuffer b = (IOBuffer)malloc(sizeof(struct iobuffer_struct));
    memset(b->buffer, 0, BUFF_SIZE);
    b->mutx = mutx;
    b->cond = cond;
    b->write_to_buffer = &write_to_buffer;
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
        return -1; // buffer overflow
    pthread_mutex_lock(self->mutx); // lock to this thread
    while(len-- > 0)
        self->buffer[self->offset++] = *(data++);
    pthread_mutex_unlock(self->mutx); // unlock
    pthread_cond_signal(self->cond); // notify thread
    return 1;
}
