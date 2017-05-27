#include "../../include/dataoperations/intops.h"

char rs1b(char *src)
{
    union s1b_uni uni;
    memcpy(uni.c, src, sizeof(char));
    return uni.i;
}
unsigned char ru1b(char *src)
{
    union u1b_uni uni;
    memcpy(uni.c, src, sizeof(char));
    return uni.i;
}
short rs2b(char *src)
{
    union s2b_uni uni;
    memcpy(uni.c, src, sizeof(short));
    return uni.i;
}
unsigned short ru2b(char *src)
{
    union u2b_uni uni;
    memcpy(uni.c, src, sizeof(short));
    return uni.i;
}
int rs4b(char *src)
{
    union s4b_uni uni;
    memcpy(uni.c, src, sizeof(int));
    return uni.i;
}
unsigned int ru4b(char *src)
{
    union u4b_uni uni;
    memcpy(uni.c, src, sizeof(int));
    return uni.i;
}
long rs8b(char *src)
{
    union s8b_uni uni;
    memcpy(uni.c, src, sizeof(long));
    return uni.i;
}
unsigned long ru8b(char *src)
{
    union u8b_uni uni;
    memcpy(uni.c, src, sizeof(long));
    return uni.i;
}

void ws1b(char *src, char var)
{
    union s1b_uni uni;
    uni.i = var;
    memcpy(src, uni.c, sizeof(char));
}
void wu1b(char *src, unsigned char var)
{
    union u1b_uni uni;
    uni.i = var;
    memcpy(src, uni.c, sizeof(char));
}
void ws2b(char *src, short var)
{
    union s2b_uni uni;
    uni.i = var;
    memcpy(src, uni.c, sizeof(short));
}
void wu2b(char *src, unsigned short var)
{
    union u2b_uni uni;
    uni.i = var;
    memcpy(src, uni.c, sizeof(short));
}
void ws4b(char *src, int var)
{
    union s4b_uni uni;
    uni.i = var;
    memcpy(src, uni.c, sizeof(int));
}
void wu4b(char *src, unsigned int var)
{
    union u4b_uni uni;
    uni.i = var;
    memcpy(src, uni.c, sizeof(int));
}
void ws8b(char *src, long var)
{
    union s8b_uni uni;
    uni.i = var;
    memcpy(src, uni.c, sizeof(long));
}
void wu8b(char *src, unsigned long var)
{
    union u8b_uni uni;
    uni.i = var;
    memcpy(src, uni.c, sizeof(long));
}
