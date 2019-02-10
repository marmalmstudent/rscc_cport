#ifndef IOSTRM_H
#define IOSTRM_H


#include <pthread.h>
#include "buffer.h"

/** Class hadling the input and output streams related to the socket */
typedef struct iostream_struct *IOStream;

/** Constructor */
IOStream iostrm_ctor(const char * hostname, unsigned int port);
/** Destructor */
void iostrm_dtor(IOStream obj);

/** opens the current (TCP) socket with current port and address */
int openSocket(IOStream self);
/** closes the socket */
int closeSocket(IOStream self);
/** attemps to connect the socket to the port and address */
int connectSock(IOStream self);
/** write offset bytes of data from buffer to socket */
int socketwrite(IOStream self);
/** read len bytes of data from socket to buffer */
int socketread(IOStream self, int len);
/** Thread start */
void iostrm_tstart(IOStream self, pthread_t *thrd);

/** copies data from input buffer to dst */
void get_bfr_in(IOStream self, IOBuffer dst);
/** writes data to the iobuffer. locks the mutex during the write */
void put_bfr_out(IOStream self, IOBuffer src);

#endif // IOSTRM_H
