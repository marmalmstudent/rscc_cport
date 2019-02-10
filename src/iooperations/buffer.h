#ifndef IOBUFFER_H
#define IOBUFFER_H

#include <stdbool.h>
#include <unistd.h>

enum endian {
  endian_big,
  endian_little
};

struct allocated_block {
  ssize_t offset; /* the beginning of the allocaded space */
  size_t len; /* the size of the allocated space */
};

struct data_buffer {
  char bfr[4096];
  size_t offset;
  struct allocated_block header;
};

/** Constructor */
bool buffer_ctor(struct data_buffer *buffer);
/** Destructor */
void buffer_dtor(struct data_buffer *buffer);

/** moves the buffer offset forward steps steps and saves the offset
    and length for later use by calling pop_steps. */
int push_alloc(struct data_buffer *buffer, size_t steps);
/** writes len bytes from data to buffer where the offset were before
    push_step was called.*/
int pop_alloc(struct data_buffer *buffer, const char *data, size_t len);
/** discards the allocaded space in the buffer created by push_step. */
void dealloc(struct data_buffer *buffer);
/** overwrites the buffer and sets last index to beginning of buffer */
int reset(struct data_buffer *buffer);

/** returns the number of used slots in the buffer */
int get_used(struct data_buffer const *buffer);
/** copies len entries of buffer data from src to dst */
void buffer_copy(struct data_buffer const *src, struct data_buffer *dst, size_t len);
/** copies len bytes from data to this buffer */
void data_copy(struct data_buffer *buffer, char const *data, size_t len);

/** adds len entries of buffer data from src to dst */
void put_buffer(struct data_buffer const *src, struct data_buffer *dst, size_t len);
/** adds len bytes from data to this buffer */
void put_data(struct data_buffer *buffer, char const *data, size_t len);
/** copies the contents of the buffer to dst. */
char *get_data(struct data_buffer const *buffer, char *dst);

unsigned char get_1_byte(struct data_buffer *buffer);
unsigned short get_2_bytes(struct data_buffer *buffer, enum endian e);
unsigned int get_4_bytes(struct data_buffer *buffer, enum endian e);
unsigned long get_8_bytes(struct data_buffer *buffer, enum endian e);

void put_1_byte(struct data_buffer *buffer, unsigned char var);
void put_2_bytes(struct data_buffer *buffer, unsigned short var, enum endian e);
void put_4_bytes(struct data_buffer *buffer, unsigned int var, enum endian e);
void put_8_bytes(struct data_buffer *buffer, unsigned long var, enum endian e);
void put_string(struct data_buffer *buffer, char const *str);

#endif // IOBUFFER_H
