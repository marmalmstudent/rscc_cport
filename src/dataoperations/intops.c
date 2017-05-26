#include "../../include/dataoperations/intops.h"

int r1b(char *arr, int sign)
{
    return (int) (sign > 0 ? (unsigned char)*arr : *arr);
}

int r2b(char *arr, int sign, int endian)
{
    unsigned short out;
    if (endian < 0) // little endian
        out = ((r1b(arr+1, 1) << 8) | r1b(arr, 1));
    else
        out = ((r1b(arr, 1) << 8) | r1b(arr+1, 1));
    if (sign < 0)
        return (int) ((short) out);
    return (int) out;
}
int r4b(char *arr, int endian)
{
    unsigned int out;
    if (endian < 0) // little endian
        out = ((r2b(arr+2, 1, endian) << 16) | r2b(arr, 1, endian));
    else
        out = ((r2b(arr, 1, endian) << 16) | r2b(arr+2, 1, endian));
    return (int) out;
}
long r8b(char *arr, int endian)
{
    unsigned long out;
    if (endian < 0) // little endian
        out = (((unsigned long) r4b(arr+4, endian) << 32)
               | (unsigned long) r4b(arr, endian));
    else
        out = (((unsigned long) r4b(arr, endian) << 32)
               | (unsigned long) r4b(arr+4, endian));
    return (long) out;
}

void w1b(char *arr, int val)
{
    *arr = (char) val;
}

void w2b(char *arr, int val, int endian)
{
    if (endian < 0)
    {// little endian
        w1b(arr+1, val >> 8);
        w1b(arr, val);
    }
    else
    {
        w1b(arr, val >> 8);
        w1b(arr+1, val);
    }
}

void w4b(char *arr, int val, int endian)
{
    if (endian < 0)
    {// little endian
        w2b(arr+2, val >> 16, endian);
        w2b(arr, val, endian);
    }
    else
    {
        w2b(arr, val >> 16, endian);
        w2b(arr+2, val, endian);
    }
}
void w8b(char *arr, long val, int endian)
{
    if (endian < 0)
    {// little endian
        w4b(arr+4, (int) (val >> 32), endian);
        w4b(arr, (int) val, endian);
    }
    else
    {
        w4b(arr, (int) (val >> 32), endian);
        w4b(arr+4, (int) val, endian);
    }
}
