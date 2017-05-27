#include "../../include/dataoperations/floatops.h"

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

double rd(char *arr, int endian)
{
    int dval = sizeof(double);
    union duni uni;
    if (endian < 0)
    {
        arr += dval;
        for (int i = 0; i < dval;)
            uni.c[i++] = *--arr;
    }
    else
        for (int i = 0; i < dval;)
            uni.c[i++] = *arr++;
    return uni.d;
}

float rf(char *arr, int endian)
{
    int dval = sizeof(float);
    union funi uni;
    if (endian < 0)
    {
        arr += dval;
        for (int i = 0; i < dval;)
            uni.c[i++] = *--arr;
    }
    else
        for (int i = 0; i < dval;)
            uni.c[i++] = *arr++;
    return uni.f;
}

void wd(char *arr, double val, int endian)
{
    int dval = sizeof(double);
    union duni uni;
    uni.d = val;
    if (endian < 0)
    {
        arr += dval;
        for (int i = 0; i < dval;)
            *--arr = uni.c[i++];
    }
    else
        for (int i = 0; i < dval;)
            *arr++ = uni.c[i++];
}

void wf(char *arr, float val, int endian)
{
    int fval = sizeof(float);
    union funi uni;
    uni.f = val;
    if (endian < 0)
    {
        arr += fval;
        for (int i = 0; i < fval;)
            *--arr = uni.c[i++];
    }
    else
        for (int i = 0; i < fval;)
            *arr++ = uni.c[i++];
}
