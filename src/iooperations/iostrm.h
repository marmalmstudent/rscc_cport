#ifndef IOSTRM_H
#define IOSTRM_H

#include <stdbool.h>
#include <pthread.h>
#include "buffer.h"

#define IO_STREAM_BSIZE 5000

struct bfr
{
  char data[IO_STREAM_BSIZE];
  int offset;
};

/** Class hadling the input and output streams related to the socket */
struct io_stream
{
  int streamopen; // condition for shutting down the thread
  int tret;       // thread return valus

  int sockfd;     // file descriptor
  int port;
  char host[64];

  struct bfr bfr_in; // where data from the socket is put
  struct bfr bfr_out; // where data to the socket is put
};

/** Constructor */
bool iostrm_ctor(struct io_stream *stream, const char * hostname, unsigned int port);
/** Destructor */
void iostrm_dtor(struct io_stream *stream);

/** opens the current (TCP) socket with current port and address */
bool openSocket(struct io_stream *stream);
/** closes the socket */
int closeSocket(struct io_stream *stream);
/** write offset bytes of data from buffer to socket */
int socketwrite(struct io_stream *stream);
/** read len bytes of data from socket to buffer */
int socketread(struct io_stream *stream, int len);
/** Thread start */
void iostrm_tstart(struct io_stream *stream, pthread_t *thrd);

/** copies data from input buffer to dst */
void get_bfr_in(struct io_stream *stream, struct data_buffer *dst);
/** writes data to the iobuffer. locks the mutex during the write */
void put_bfr_out(struct io_stream *stream, struct data_buffer const *src);

#endif // IOSTRM_H
