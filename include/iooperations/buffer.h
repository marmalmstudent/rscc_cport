#ifndef IOBUFFER_H
#define IOBUFFER_H

#define BIG_ENDIAN_ 1
#define LITTLE_ENDIAN_ -1
typedef struct buffer_struct *IOBuffer;

/** Constructor */
IOBuffer buffer_ctor(int len);
/** Destructor */
void buffer_dtor(IOBuffer obj);
/** returns the number of used slots in the buffer */
int get_used_size(IOBuffer self);
/** moves the buffer offset forward steps steps and saves the offset
    and length for later use by calling pop_steps. */
int push_step(IOBuffer self, int steps);
/** writes len bytes from data to buffer where the offset were before
    push_step was called.*/
int pop_step(IOBuffer self, const char *data, int len);
/** discards the allocaded space in the buffer created by push_step. */
void reset_step(IOBuffer self);
/** overwrites the buffer and sets last index to beginning of buffer */
int reset(IOBuffer self);
void add_data(IOBuffer self, const char *data, int len);
/** returns the buffer starting from just after the last entered value. */
char *get_data_end(IOBuffer self);
/** returns the buffer starting from the beginning. */
char *get_data(IOBuffer self);
/** copies the contents of the buffer to dst. */
void copy_data(IOBuffer self, char *dst);

char getSigned1Byte(IOBuffer self);
short getSigned2Bytes(IOBuffer self, int endian);
int getSigned4Bytes(IOBuffer self, int endian);
long getSigned8Bytes(IOBuffer self, int endian);

void putSigned1Byte(IOBuffer self, char var);
void putSigned2Bytes(IOBuffer self, short var, int endian);
void putSigned4Bytes(IOBuffer self, int var, int endian);
void putSigned8Bytes(IOBuffer self, long var, int endian);

unsigned char getUnsigned1Byte(IOBuffer self);
unsigned short getUnsigned2Bytes(IOBuffer self, int endian);
unsigned int getUnsigned4Bytes(IOBuffer self, int endian);
unsigned long getUnsigned8Bytes(IOBuffer self, int endian);

void putUnsigned1Byte(IOBuffer self, unsigned char var);
void putUnsigned2Bytes(IOBuffer self, unsigned short var, int endian);
void putUnsigned4Bytes(IOBuffer self, unsigned int var, int endian);
void putUnsigned8Bytes(IOBuffer self, unsigned long var, int endian);


#endif // IOBUFFER_H
