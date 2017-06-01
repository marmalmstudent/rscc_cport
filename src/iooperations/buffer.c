#include "../../include/iooperations/buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>
#include "../../include/dataoperations/intops.h"

struct step_struct
{
    int offset; /* the beginning of the allocaded space */
    int len; /* the size of the allocated space */
};

struct buffer_struct
{
    char *bfr;
    int offset;
    struct step_struct header;
};

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
    return ru1b(self->bfr + self->offset++);
}
unsigned short get_2_bytes(IOBuffer self, int endian)
{
    unsigned short ret = ru2b(self->bfr + self->offset);
    self->offset += 2;
    return endian >= 0 ? be16toh(ret) : le16toh(ret);
}
unsigned int get_4_bytes(IOBuffer self, int endian)
{
    unsigned int ret = ru4b(self->bfr + self->offset);
    self->offset += 4;
    return endian >= 0 ? be32toh(ret) : le32toh(ret);
}
unsigned long get_8_bytes(IOBuffer self, int endian)
{
    unsigned long ret = ru8b(self->bfr + self->offset);
    self->offset += 8;
    return endian >= 0 ? be64toh(ret) : le64toh(ret);
}

void put_1_byte(IOBuffer self, unsigned char var)
{
    wu1b(self->bfr + self->offset++, var);
}
void put_2_bytes(IOBuffer self, unsigned short var, int endian)
{
    wu2b(self->bfr + self->offset,
         endian >= 0 ? htobe16(var) : htole16(var));
    self->offset += 2;
}
void put_4_bytes(IOBuffer self, unsigned int var, int endian)
{
    wu4b(self->bfr + self->offset,
         endian >= 0 ? htobe32(var) : htole32(var));
    self->offset += 4;
}
void put_8_bytes(IOBuffer self, unsigned long var, int endian)
{
    wu8b(self->bfr + self->offset,
         endian >= 0 ? htobe64(var) : htole64(var));
    self->offset += 8;
}
