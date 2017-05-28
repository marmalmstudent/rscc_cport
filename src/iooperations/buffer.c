#include "../../include/iooperations/buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/dataoperations/intops.h"

struct buffer_struct
{
    char *bfr;
    int offset;
};

IOBuffer buffer_ctor(int bsize)
{
    IOBuffer b = (IOBuffer)malloc(sizeof(struct buffer_struct));
    b->bfr = (char *)calloc(bsize, sizeof(char));
    b->offset = 0;
    return b;
}

void buffer_dtor(IOBuffer obj)
{
    free(obj->bfr);
    free(obj);
}

int get_used_size(IOBuffer self)
{
    return self->offset;
}

int reset(IOBuffer self)
{
    memset(self->bfr, 0, self->offset);
    int data_len = get_used_size(self);
    self->offset = 0;
    return data_len;
}

void add_data(IOBuffer self, const char *data, int len)
{
    memcpy(self->bfr + self->offset, data, len);
    self->offset += len;
}

char *get_data_end(IOBuffer self)
{
    return self->bfr + self->offset;
}
char *get_data(IOBuffer self)
{
    return self->bfr;
}
void copy_data(IOBuffer self, char *dst)
{
    memcpy(dst, self->bfr, self->offset);
}


char getSigned1Byte(IOBuffer self)
{
    return rs1b(self->bfr + self->offset++);
}
short getSigned2Bytes(IOBuffer self, int endian)
{
    short ret;
    if (endian >= 0)
    { // big endian
        char be_arr[2];
        for (int i = 1; i >= 0; --i) be_arr[i] = self->bfr[self->offset+i];
        ret = rs2b(be_arr);
    }
    else
        ret = rs2b(self->bfr + self->offset - 2);
    self->offset += 2;
    return ret;
}
int getSigned4Bytes(IOBuffer self, int endian)
{
    int ret;
    if (endian >= 0)
    { // big endian
        char be_arr[4];
        for (int i = 3; i >= 0; --i) be_arr[i] = self->bfr[self->offset+i];
        ret = rs4b(be_arr);
    }
    else
        ret = rs4b(self->bfr + self->offset);
    self->offset += 4;
    return ret;
}
long getSigned8Bytes(IOBuffer self, int endian)
{
    long ret;
    if (endian >= 0)
    { // big endian
        char be_arr[8];
        for (int i = 7; i >= 0; --i) be_arr[i] = self->bfr[self->offset+i];
        ret = rs8b(be_arr);
    }
    else
        ret = rs8b(self->bfr + self->offset);
    self->offset += 8;
    return ret;
}

void putSigned1Byte(IOBuffer self, char var)
{
    ws1b(self->bfr + self->offset++, var);
}
void putSigned2Bytes(IOBuffer self, short var, int endian)
{
    if (endian >= 0)
    { // big endian
        char tmp[2];
        ws2b(tmp, var);
        for (int i = 1; i >= 0; --i) self->bfr[self->offset++] = tmp[i];
    }
    else
    {
        ws2b(self->bfr + self->offset, var);
        self->offset += 2;
    }
}
void putSigned4Bytes(IOBuffer self, int var, int endian)
{
    if (endian >= 0)
    { // big endian
        char tmp[4];
        ws4b(tmp, var);
        for (int i = 3; i >= 0; --i) self->bfr[self->offset++] = tmp[i];
    }
    else
    {
        ws4b(self->bfr + self->offset, var);
        self->offset += 4;
    }
}
void putSigned8Bytes(IOBuffer self, long var, int endian)
{
    if (endian >= 0)
    { // big endian
        char tmp[8];
        ws8b(tmp, var);
        for (int i = 7; i >= 0; --i) self->bfr[self->offset++] = tmp[i];
    }
    else
    {
        ws8b(self->bfr + self->offset, var);
        self->offset += 8;
    }
}

unsigned char getUnsigned1Byte(IOBuffer self)
{
    return ru1b(self->bfr + self->offset++);
}
unsigned short getUnsigned2Bytes(IOBuffer self, int endian)
{
    unsigned short ret;
    if (endian >= 0)
    { // big endian
        char be_arr[2];
        for (int i = 1; i >= 0; --i) be_arr[i] = self->bfr[self->offset+i];
        ret = ru2b(be_arr);
    }
    else
        ret = ru2b(self->bfr + self->offset - 2);
    self->offset += 2;
    return ret;
}
unsigned int getUnsigned4Bytes(IOBuffer self, int endian)
{
    unsigned int ret;
    if (endian >= 0)
    { // big endian
        char be_arr[4];
        for (int i = 3; i >= 0; --i) be_arr[i] = self->bfr[self->offset+i];
        ret = ru4b(be_arr);
    }
    else
        ret = ru4b(self->bfr + self->offset);
    self->offset += 4;
    return ret;
}
unsigned long getUnsigned8Bytes(IOBuffer self, int endian)
{
    unsigned long ret;
    if (endian >= 0)
    { // big endian
        char be_arr[8];
        for (int i = 7; i >= 0; --i) be_arr[i] = self->bfr[self->offset+i];
        ret = ru8b(be_arr);
    }
    else
        ret = ru8b(self->bfr + self->offset);
    self->offset += 8;
    return ret;
}

void putUnsigned1Byte(IOBuffer self, unsigned char var)
{
    wu1b(self->bfr + self->offset++, var);
}
void putUnsigned2Bytes(IOBuffer self, unsigned short var, int endian)
{
    if (endian >= 0)
    { // big endian
        char tmp[2];
        wu2b(tmp, var);
        for (int i = 1; i >= 0; --i) self->bfr[self->offset++] = tmp[i];
    }
    else
    {
        wu2b(self->bfr + self->offset, var);
        self->offset += 2;
    }
}
void putUnsigned4Bytes(IOBuffer self, unsigned int var, int endian)
{
    if (endian >= 0)
    { // big endian
        char tmp[4];
        wu4b(tmp, var);
        for (int i = 3; i >= 0; --i) self->bfr[self->offset++] = tmp[i];
    }
    else
    {
        wu4b(self->bfr + self->offset, var);
        self->offset += 4;
    }
}
void putUnsigned8Bytes(IOBuffer self, unsigned long var, int endian)
{
    if (endian >= 0)
    { // big endian
        char tmp[8];
        wu8b(tmp, var);
        for (int i = 7; i >= 0; --i) self->bfr[self->offset++] = tmp[i];
    }
    else
    {
        wu8b(self->bfr + self->offset, var);
        self->offset += 8;
    }
}
