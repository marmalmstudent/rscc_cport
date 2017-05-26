#ifndef INTOPS_H
#define INTOPS_H

int r1b(char *arr, int sign);
int r2b(char *arr, int sign, int endian);
int r4b(char *arr, int endian);
long r8b(char *arr, int endian);
void w1b(char *arr, int val);
void w2b(char *arr, int val, int endian);
void w4b(char *arr, int val, int endian);
void w8b(char *arr, long val, int endian);

#endif // INTOPS_H
