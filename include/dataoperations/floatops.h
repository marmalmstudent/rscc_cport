#ifndef FLOATOPS_H
#define FLOATOPS_H

#include <stdlib.h>

union duni
{
    char c[sizeof(double)];
    double d;
};
union funi
{
    char c[sizeof(float)];
    float f;
};

double rd(char *arr, int endian);
float rf(char *arr, int endian);
void wd(char *arr, double val, int endian);
void wf(char *arr, float val, int endian);

#endif // FLOATOPS_H
