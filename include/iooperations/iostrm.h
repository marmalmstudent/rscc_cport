#ifndef IOSTRM_H
#define IOSTRM_H


#include <pthread.h>

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
/** reads len bytes of data from the standard input */
int stdinread(IOStream self, int len);
/** write offset bytes of data from buffer to socket */
int socketwrite(IOStream self);
/** read len bytes of data from socket to buffer */
int socketread(IOStream self, int len);
/** Thread start */
void iostrm_tstart(IOStream self, pthread_t *thrd);
/** resets the instream buffer */
void reset_inbuffer(IOStream self);
/** resets the outstream buffer */
void reset_outbuffer(IOStream self);

/* will be removed later */
void print_inbuffer(IOStream self);

#endif // IOSTRM_H
