#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "buffer.h"

#include "iostrm.h"

static pthread_mutex_t mutx;
static pthread_cond_t cond;

/** writes src data to b's buffer */
static void bfr_add(struct bfr *b, struct data_buffer const *src)
{
  get_data(src, b->data);
  b->offset += get_used(src);
}

/** resets the buffer offset to zero and overwrites data with zeros */
static void bfr_reset(struct bfr *b)
{
  memset(b->data, 0, b->offset);
  b->offset = 0;
}

/** run the thread */
static struct io_stream *iostrm_trun(struct io_stream *stream)
{
  while(stream->streamopen) {
    pthread_mutex_lock(&mutx); // start of synchronize block in java
    if (stream->bfr_out.offset == 0) {
      pthread_cond_wait(&cond, &mutx);
    }
    pthread_mutex_unlock(&mutx); // end of synchronize block in java
    if (stream->streamopen)
      if (socketwrite(stream) < 0)
	fprintf(stderr, "ERROR writing to socket");
  }
  return NULL;
}

bool iostrm_ctor(struct io_stream *stream, const char * hostname, unsigned int port)
{
  /* Port nad hostname */
  stream->port = port;
  strncpy(stream->host, hostname, sizeof(stream->host)-1);

  /* in and out streams */
  memset(&stream->bfr_in, 0, sizeof(struct bfr));
  memset(&stream->bfr_out, 0, sizeof(struct bfr));
  return true;
}

void iostrm_dtor(struct io_stream *stream)
{
  (void)stream;
}

/* write offset bytes of data from buffer to socket */
int socketwrite(struct io_stream *stream)
{
  int flag = 0;
  int buff_len = stream->bfr_out.offset;
  int byteswritten = send(stream->sockfd, stream->bfr_out.data, buff_len, 0);
  bfr_reset(&stream->bfr_out);
  if (byteswritten != buff_len) {
    flag = 1; // could not write entire buffer to socket
  } else if (byteswritten < 0) {
    flag = -1; // could not write any data to socket
  }
  return flag;
}

/* read len bytes of data from socket to buffer */
int socketread(struct io_stream *stream, int len)
{
  int flag = 0, len_read;
  if (len <= IO_STREAM_BSIZE) {
    len_read = len;
  } else {
    len_read = IO_STREAM_BSIZE;
    flag |= 2; // buffer size not big enough
  }
  int bytesread = recv(stream->sockfd, stream->bfr_in.data + stream->bfr_in.offset, len_read, 0);
  if (bytesread >= 0) {
    if (bytesread != len_read) {
      flag |= 1; // could not read all data
    }
    stream->bfr_in.offset += bytesread;
  } else {
    flag = -1; // an error occured
  }
  return flag;
}

/* opens the current (TCP) socket with current port and address */
bool openSocket(struct io_stream *stream)
{ 
  stream->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (stream->sockfd < 0) {
    fprintf(stderr, "ERROR creating socket: %s\n", strerror(errno));
    return false;
  }
  
  /* socket addres */
  struct sockaddr_in addr;
  memset(&addr, 0 , sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(stream->port); // host to network short
  if (inet_pton(AF_INET, stream->host, &addr.sin_addr) <= 0) {
    fprintf(stderr, "ERROR converting IPv4 address '%s' to binary form: %s\n", stream->host, strerror(errno));
    return false;
  }
  
  if (connect(stream->sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0) {
    fprintf(stderr, "ERROR connecting to server: %s\n", strerror(errno));
    return false;
  }
  return true;
}

/* closes the socket */
int closeSocket(struct io_stream *stream)
{
  stream->streamopen = 0; // close stream
  pthread_cond_signal(&cond); // notify thread
  return close(stream->sockfd);
}

/* start the thread */
void iostrm_tstart(struct io_stream *stream, pthread_t *thrd)
{
  stream->streamopen = 1;
  stream->tret = pthread_create(thrd, NULL, (void*(*)(void*))iostrm_trun, (void *)stream);
}

void put_bfr_out(struct io_stream *stream, struct data_buffer const *src)
{
  pthread_mutex_lock(&mutx); // lock to this thread
  bfr_add(&stream->bfr_out, src);
  pthread_mutex_unlock(&mutx); // unlock
  pthread_cond_signal(&cond); // notify thread
}

void get_bfr_in(struct io_stream *stream, struct data_buffer *dst)
{
  data_copy(dst, stream->bfr_in.data, stream->bfr_in.offset);
  bfr_reset(&stream->bfr_in);
}
