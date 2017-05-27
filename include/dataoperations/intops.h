#ifndef INTOPS_H
#define INTOPS_H

#include <string.h>

union s1b_uni { char c[sizeof(char)]; char i; };
union u1b_uni { char c[sizeof(char)]; unsigned char i; };
union s2b_uni { char c[sizeof(short)]; short i; };
union u2b_uni { char c[sizeof(short)]; unsigned short i; };
union s4b_uni { char c[sizeof(int)]; int i; };
union u4b_uni { char c[sizeof(int)]; unsigned int i; };
union s8b_uni { char c[sizeof(long)]; long i; };
union u8b_uni { char c[sizeof(long)]; unsigned long i; };

char rs1b(char *src);
unsigned char ru1b(char *src);
short rs2b(char *src);
unsigned short ru2b(char *src);
int rs4b(char *src);
unsigned int ru4b(char *src);
long rs8b(char *src);
unsigned long ru8b(char *src);

void ws1b(char *src, char varl);
void wu1b(char *src, unsigned char varl);
void ws2b(char *src, short varl);
void wu2b(char *src, unsigned short varl);
void ws4b(char *src, int varl);
void wu4b(char *src, unsigned int varl);
void ws8b(char *src, long varl);
void wu8b(char *src, unsigned long varl);

#endif // INTOPS_H
