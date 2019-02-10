#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "buffer.h"

IOBuffer buffer_ctor(int bsize)
{
    IOBuffer b = (IOBuffer)malloc(sizeof(struct buffer_struct));
    b->bfr = (char *)calloc(bsize, sizeof(char));
    b->offset = 0;
    b->header.offset = -1;
    return b;
}

void buffer_dtor(IOBuffer obj)
{
    free(obj->bfr);
    obj->bfr = NULL;
    free(obj);
    obj = NULL;
}

int get_used(const IOBuffer self)
{
    return self->offset;
}

int push_alloc(IOBuffer self, int steps)
{
    if (self->header.offset >= 0) /* previously allocated memory exists */
        return -1;
    self->header.offset = self->offset;
    self->header.len = steps;
    self->offset += steps;
    return 1;
}

int pop_alloc(IOBuffer self, const char *data, int len)
{
    if (self->header.offset < 0 /* push_step has not preceded call to pop_step */
            || self->header.len < len)
        return -1;

    /* add data to allocated part of buffer */
    int tmp = self->offset;
    self->offset = self->header.offset;
    put_data(self, data, len);
    self->offset = tmp;

    /* update allocated memory */
    self->header.offset += len;
    self->header.len -= len;
    return 1;
}

void dealloc(IOBuffer self)
{
    self->header.offset = -1;
    self->header.len = 0;
}

int reset(IOBuffer self)
{
    memset(self->bfr, 0, self->offset);
    int data_len = get_used(self);
    self->offset = 0;
    return data_len;
}

void buffer_copy(const IOBuffer src, IOBuffer dst, int len)
{
    memcpy(dst->bfr + dst->offset, src->bfr, len);
}

void data_copy(IOBuffer self, const char *data, int len)
{
    memcpy(self->bfr + self->offset, data, len);
}

void put_buffer(const IOBuffer src, IOBuffer dst, int len)
{
    memcpy(dst->bfr + dst->offset, src->bfr, len);
    dst->offset += len;
}

void put_data(IOBuffer self, const char *data, int len)
{
    memcpy(self->bfr + self->offset, data, len);
    self->offset += len;
}
char *get_data(const IOBuffer self, char *dst)
{
    memcpy(dst, self->bfr, self->offset);
    return dst;
}

unsigned char get_1_byte(IOBuffer self)
{
  return self->bfr[self->offset++];
}
unsigned short get_2_bytes(IOBuffer self, enum endian e)
{
  self->offset += 2;
  if (e == endian_big) {
    return (((self->bfr[self->offset-2] & (uint64_t)0xff) << 8) |
	    ((self->bfr[self->offset-1] & (uint64_t)0xff)     ));
  } else {
    return (((self->bfr[self->offset-1] & (uint64_t)0xff) << 8) |
	    ((self->bfr[self->offset-2] & (uint64_t)0xff)     ));
  }
}
unsigned int get_4_bytes(IOBuffer self, enum endian e)
{
  self->offset += 4;
  if (e == endian_big) {
    return (((self->bfr[self->offset-4] & (uint64_t)0xff) << 24) |
	    ((self->bfr[self->offset-3] & (uint64_t)0xff) << 16) |
	    ((self->bfr[self->offset-2] & (uint64_t)0xff) <<  8) |
	    ((self->bfr[self->offset-1] & (uint64_t)0xff)      ));
  } else {
    return (((self->bfr[self->offset-1] & (uint64_t)0xff) << 24) |
	    ((self->bfr[self->offset-2] & (uint64_t)0xff) << 16) |
	    ((self->bfr[self->offset-3] & (uint64_t)0xff) <<  8) |
	    ((self->bfr[self->offset-4] & (uint64_t)0xff)      ));
  }
}
unsigned long get_8_bytes(IOBuffer self, enum endian e)
{
  printf("[ ");
  for (int i = 0; i < self->offset; ++i) {
    printf("%02x ", (unsigned char)self->bfr[i]);
  }
  printf("]\n");

  self->offset += 8;
  if (e == endian_big) {
    return (((self->bfr[self->offset-8] & (uint64_t)0xff) << 56) |
	    ((self->bfr[self->offset-7] & (uint64_t)0xff) << 48) |
	    ((self->bfr[self->offset-6] & (uint64_t)0xff) << 40) |
	    ((self->bfr[self->offset-5] & (uint64_t)0xff) << 32) |
	    ((self->bfr[self->offset-4] & (uint64_t)0xff) << 24) |
	    ((self->bfr[self->offset-3] & (uint64_t)0xff) << 16) |
	    ((self->bfr[self->offset-2] & (uint64_t)0xff) <<  8) |
	    ((self->bfr[self->offset-1] & (uint64_t)0xff)      ));
  } else {
    return (((self->bfr[self->offset-1] & (uint64_t)0xff) << 56) |
	    ((self->bfr[self->offset-2] & (uint64_t)0xff) << 48) |
	    ((self->bfr[self->offset-3] & (uint64_t)0xff) << 40) |
	    ((self->bfr[self->offset-4] & (uint64_t)0xff) << 32) |
	    ((self->bfr[self->offset-5] & (uint64_t)0xff) << 24) |
	    ((self->bfr[self->offset-6] & (uint64_t)0xff) << 16) |
	    ((self->bfr[self->offset-7] & (uint64_t)0xff) <<  8) |
	    ((self->bfr[self->offset-8] & (uint64_t)0xff)      ));
  }
}

void put_1_byte(IOBuffer self, unsigned char var)
{
  self->bfr[self->offset++] = var;
}
void put_2_bytes(IOBuffer self, unsigned short var, enum endian e)
{
  self->offset += 2;
  if (e == endian_big) {
    self->bfr[self->offset-2] = var >> 8 & 0xff;
    self->bfr[self->offset-1] = var      & 0xff;
  } else {
    self->bfr[self->offset-1] = var >> 8 & 0xff;
    self->bfr[self->offset-2] = var      & 0xff;
  }
}
void put_4_bytes(IOBuffer self, unsigned int var, enum endian e)
{
  self->offset += 4;
  if (e == endian_big) {
    self->bfr[self->offset-4] = var >> 24 & 0xff;
    self->bfr[self->offset-3] = var >> 16 & 0xff;
    self->bfr[self->offset-2] = var >>  8 & 0xff;
    self->bfr[self->offset-1] = var       & 0xff;
  } else {
    self->bfr[self->offset-1] = var >> 24 & 0xff;
    self->bfr[self->offset-2] = var >> 16 & 0xff;
    self->bfr[self->offset-3] = var >>  8 & 0xff;
    self->bfr[self->offset-4] = var       & 0xff;
  }
}
void put_8_bytes(IOBuffer self, unsigned long var, enum endian e)
{
  self->offset += 8;
  if (e == endian_big) {
    self->bfr[self->offset-8] = var >> 56 & 0xff;
    self->bfr[self->offset-7] = var >> 48 & 0xff;
    self->bfr[self->offset-6] = var >> 40 & 0xff;
    self->bfr[self->offset-5] = var >> 32 & 0xff;
    self->bfr[self->offset-4] = var >> 24 & 0xff;
    self->bfr[self->offset-3] = var >> 16 & 0xff;
    self->bfr[self->offset-2] = var >>  8 & 0xff;
    self->bfr[self->offset-1] = var       & 0xff;
  } else {
    self->bfr[self->offset-1] = var >> 56 & 0xff;
    self->bfr[self->offset-2] = var >> 48 & 0xff;
    self->bfr[self->offset-3] = var >> 40 & 0xff;
    self->bfr[self->offset-4] = var >> 32 & 0xff;
    self->bfr[self->offset-5] = var >> 24 & 0xff;
    self->bfr[self->offset-6] = var >> 16 & 0xff;
    self->bfr[self->offset-7] = var >>  8 & 0xff;
    self->bfr[self->offset-8] = var       & 0xff;
  }
}

void put_string(IOBuffer self, char const *str)
{
  size_t len = strlen(str);
  memcpy(self->bfr + self->offset, str, len);
  self->offset += len;
  put_1_byte(self, 10);
}
