#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "buffer.h"

bool buffer_ctor(struct data_buffer *buffer)
{
  memset(buffer->bfr, 0, sizeof(buffer->bfr));
  buffer->offset = 0;
  buffer->header.offset = -1;
  return true;
}

void buffer_dtor(struct data_buffer *buffer)
{
  (void)buffer;
}

int get_used(struct data_buffer const *buffer)
{
  return buffer->offset;
}

int push_alloc(struct data_buffer *buffer, size_t steps)
{
  if (buffer->header.offset >= 0) /* previously allocated memory exists */
    return -1;
  buffer->header.offset = buffer->offset;
  buffer->header.len = steps;
  buffer->offset += steps;
  return 1;
}

int pop_alloc(struct data_buffer *buffer, char const *data, size_t len)
{
  if (buffer->header.offset < 0 /* push_step has not preceded call to pop_step */
      || buffer->header.len < len)
    return -1;

  /* add data to allocated part of buffer */
  int tmp = buffer->offset;
  buffer->offset = buffer->header.offset;
  put_data(buffer, data, len);
  buffer->offset = tmp;

  /* update allocated memory */
  buffer->header.offset += len;
  buffer->header.len -= len;
  return 1;
}

void dealloc(struct data_buffer *buffer)
{
  buffer->header.offset = -1;
  buffer->header.len = 0;
}

int reset(struct data_buffer *buffer)
{
  memset(buffer->bfr, 0, buffer->offset);
  int data_len = get_used(buffer);
  buffer->offset = 0;
  return data_len;
}

void buffer_copy(struct data_buffer const *src, struct data_buffer *dst, size_t len)
{
  memcpy(dst->bfr + dst->offset, src->bfr, len);
}

void data_copy(struct data_buffer *buffer, char const *data, size_t len)
{
  memcpy(buffer->bfr + buffer->offset, data, len);
}

void put_buffer(struct data_buffer const *src, struct data_buffer *dst, size_t len)
{
  memcpy(dst->bfr + dst->offset, src->bfr, len);
  dst->offset += len;
}

void put_data(struct data_buffer *buffer, char const *data, size_t len)
{
  memcpy(buffer->bfr + buffer->offset, data, len);
  buffer->offset += len;
}
char *get_data(struct data_buffer const *buffer, char *dst)
{
    memcpy(dst, buffer->bfr, buffer->offset);
    return dst;
}

unsigned char get_1_byte(struct data_buffer *buffer)
{
  return buffer->bfr[buffer->offset++];
}
unsigned short get_2_bytes(struct data_buffer *buffer, enum endian e)
{
  buffer->offset += 2;
  if (e == endian_big) {
    return (((buffer->bfr[buffer->offset-2] & (uint64_t)0xff) << 8) |
	    ((buffer->bfr[buffer->offset-1] & (uint64_t)0xff)     ));
  } else {
    return (((buffer->bfr[buffer->offset-1] & (uint64_t)0xff) << 8) |
	    ((buffer->bfr[buffer->offset-2] & (uint64_t)0xff)     ));
  }
}
unsigned int get_4_bytes(struct data_buffer *buffer, enum endian e)
{
  buffer->offset += 4;
  if (e == endian_big) {
    return (((buffer->bfr[buffer->offset-4] & (uint64_t)0xff) << 24) |
	    ((buffer->bfr[buffer->offset-3] & (uint64_t)0xff) << 16) |
	    ((buffer->bfr[buffer->offset-2] & (uint64_t)0xff) <<  8) |
	    ((buffer->bfr[buffer->offset-1] & (uint64_t)0xff)      ));
  } else {
    return (((buffer->bfr[buffer->offset-1] & (uint64_t)0xff) << 24) |
	    ((buffer->bfr[buffer->offset-2] & (uint64_t)0xff) << 16) |
	    ((buffer->bfr[buffer->offset-3] & (uint64_t)0xff) <<  8) |
	    ((buffer->bfr[buffer->offset-4] & (uint64_t)0xff)      ));
  }
}
unsigned long get_8_bytes(struct data_buffer *buffer, enum endian e)
{
  buffer->offset += 8;
  if (e == endian_big) {
    return (((buffer->bfr[buffer->offset-8] & (uint64_t)0xff) << 56) |
	    ((buffer->bfr[buffer->offset-7] & (uint64_t)0xff) << 48) |
	    ((buffer->bfr[buffer->offset-6] & (uint64_t)0xff) << 40) |
	    ((buffer->bfr[buffer->offset-5] & (uint64_t)0xff) << 32) |
	    ((buffer->bfr[buffer->offset-4] & (uint64_t)0xff) << 24) |
	    ((buffer->bfr[buffer->offset-3] & (uint64_t)0xff) << 16) |
	    ((buffer->bfr[buffer->offset-2] & (uint64_t)0xff) <<  8) |
	    ((buffer->bfr[buffer->offset-1] & (uint64_t)0xff)      ));
  } else {
    return (((buffer->bfr[buffer->offset-1] & (uint64_t)0xff) << 56) |
	    ((buffer->bfr[buffer->offset-2] & (uint64_t)0xff) << 48) |
	    ((buffer->bfr[buffer->offset-3] & (uint64_t)0xff) << 40) |
	    ((buffer->bfr[buffer->offset-4] & (uint64_t)0xff) << 32) |
	    ((buffer->bfr[buffer->offset-5] & (uint64_t)0xff) << 24) |
	    ((buffer->bfr[buffer->offset-6] & (uint64_t)0xff) << 16) |
	    ((buffer->bfr[buffer->offset-7] & (uint64_t)0xff) <<  8) |
	    ((buffer->bfr[buffer->offset-8] & (uint64_t)0xff)      ));
  }
}

void put_1_byte(struct data_buffer *buffer, unsigned char var)
{
  buffer->bfr[buffer->offset++] = var;
}
void put_2_bytes(struct data_buffer *buffer, unsigned short var, enum endian e)
{
  buffer->offset += 2;
  if (e == endian_big) {
    buffer->bfr[buffer->offset-2] = var >> 8 & 0xff;
    buffer->bfr[buffer->offset-1] = var      & 0xff;
  } else {
    buffer->bfr[buffer->offset-1] = var >> 8 & 0xff;
    buffer->bfr[buffer->offset-2] = var      & 0xff;
  }
}
void put_4_bytes(struct data_buffer *buffer, unsigned int var, enum endian e)
{
  buffer->offset += 4;
  if (e == endian_big) {
    buffer->bfr[buffer->offset-4] = var >> 24 & 0xff;
    buffer->bfr[buffer->offset-3] = var >> 16 & 0xff;
    buffer->bfr[buffer->offset-2] = var >>  8 & 0xff;
    buffer->bfr[buffer->offset-1] = var       & 0xff;
  } else {
    buffer->bfr[buffer->offset-1] = var >> 24 & 0xff;
    buffer->bfr[buffer->offset-2] = var >> 16 & 0xff;
    buffer->bfr[buffer->offset-3] = var >>  8 & 0xff;
    buffer->bfr[buffer->offset-4] = var       & 0xff;
  }
}
void put_8_bytes(struct data_buffer *buffer, unsigned long var, enum endian e)
{
  buffer->offset += 8;
  if (e == endian_big) {
    buffer->bfr[buffer->offset-8] = var >> 56 & 0xff;
    buffer->bfr[buffer->offset-7] = var >> 48 & 0xff;
    buffer->bfr[buffer->offset-6] = var >> 40 & 0xff;
    buffer->bfr[buffer->offset-5] = var >> 32 & 0xff;
    buffer->bfr[buffer->offset-4] = var >> 24 & 0xff;
    buffer->bfr[buffer->offset-3] = var >> 16 & 0xff;
    buffer->bfr[buffer->offset-2] = var >>  8 & 0xff;
    buffer->bfr[buffer->offset-1] = var       & 0xff;
  } else {
    buffer->bfr[buffer->offset-1] = var >> 56 & 0xff;
    buffer->bfr[buffer->offset-2] = var >> 48 & 0xff;
    buffer->bfr[buffer->offset-3] = var >> 40 & 0xff;
    buffer->bfr[buffer->offset-4] = var >> 32 & 0xff;
    buffer->bfr[buffer->offset-5] = var >> 24 & 0xff;
    buffer->bfr[buffer->offset-6] = var >> 16 & 0xff;
    buffer->bfr[buffer->offset-7] = var >>  8 & 0xff;
    buffer->bfr[buffer->offset-8] = var       & 0xff;
  }
}

void put_string(struct data_buffer *buffer, char const *str)
{
  size_t len = strlen(str);
  memcpy(buffer->bfr + buffer->offset, str, len);
  buffer->offset += len;
  put_1_byte(buffer, 10);
}
