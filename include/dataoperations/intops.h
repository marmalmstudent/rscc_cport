#ifndef INTOPS_H
#define INTOPS_H

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
