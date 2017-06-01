#ifndef IOBUFFER_H
#define IOBUFFER_H

#define BIG_ENDIAN_ 1
#define LITTLE_ENDIAN_ -1
typedef struct buffer_struct *IOBuffer;

/** Constructor */
IOBuffer buffer_ctor(int len);
/** Destructor */
void buffer_dtor(IOBuffer obj);

/** moves the buffer offset forward steps steps and saves the offset
    and length for later use by calling pop_steps. */
int push_alloc(IOBuffer self, int steps);
/** writes len bytes from data to buffer where the offset were before
    push_step was called.*/
int pop_alloc(IOBuffer self, const char *data, int len);
/** discards the allocaded space in the buffer created by push_step. */
void dealloc(IOBuffer self);
/** overwrites the buffer and sets last index to beginning of buffer */
int reset(IOBuffer self);

/** returns the number of used slots in the buffer */
int get_used(const IOBuffer self);
/** copies len entries of buffer data from src to dst */
void buffer_copy(const IOBuffer src, IOBuffer dst, int len);

/** adds len bytes from data to this buffer */
void put_data(IOBuffer self, const char *data, int len);
/** copies the contents of the buffer to dst. */
char *get_data(const IOBuffer self, char *dst);

unsigned char get_1_byte(IOBuffer self);
unsigned short get_2_bytes(IOBuffer self, int endian);
unsigned int get_4_bytes(IOBuffer self, int endian);
unsigned long get_8_bytes(IOBuffer self, int endian);

void put_1_byte(IOBuffer self, unsigned char var);
void put_2_bytes(IOBuffer self, unsigned short var, int endian);
void put_4_bytes(IOBuffer self, unsigned int var, int endian);
void put_8_bytes(IOBuffer self, unsigned long var, int endian);


#endif // IOBUFFER_H
