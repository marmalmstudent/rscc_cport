#include "../../include/client/dataoperations.h"

void addCharacters(const char *src, char *dst, int len)
{
    int l = strlen(src);
    for (int i = 0; i < len; i++)
    {
        if (i > l)
            *dst++ = ' ';
        else if ((*src >= 'a' && *src <= 'z')
                || (*src >= 'A' && *src <= 'Z')
                || (*src >= '0' && *src <= '9'))
            *dst++ = *src++;
        else
        {
            *dst++ = '_';
            ++src;
        }
    }
}

long stringLength12ToLong(char *src)
{
    int len1 = strlen(src), s, e;
    for (s = 0; s < len1; ++s) // discard leading whitespace
        if ((src[s] >= 'a' && src[s] <= 'z')
                || (src[s] >= 'A' && src[s] <= 'Z')
                || (src[s] >= '0' && src[s] <= '9'))
            break;
    for (e = len1; e >= 0; --e) // discard trailing whitespace
        if ((src[e-1] >= 'a' && src[e-1] <= 'z')
                || (src[e-1] >= 'A' && src[e-1] <= 'Z')
                || (src[e-1] >= '0' && src[e-1] <= '9'))
            break;
    if (e <= s)
        return 0;

    int len2 = e - s;
    char tmp[len2];
    src += s;
    for (int i = s, k = 0; i < e; i++) // copy string
        if ((*src >= 'a' && *src <= 'z')
                || (*src >= '0' && *src <= '9'))
            tmp[k++] = *src++;
        else if (*src >= 'A' && *src <= 'Z')
            tmp[k++] =  32 + *src++;
        else
        {
            tmp[k++] = ' ';
            ++src;
        }
    if (len2 > 12)
        len2 = 12;
    long l = 0L;
    for (int j = 0, k = 0; j < len2; j++)
    {
        l *= 37L;
        if (tmp[k] >= 'a' && tmp[k] <= 'z')
            l += (1 + tmp[k++]) - 97;
        else if (tmp[k] >= '0' && tmp[k] <= '9')
            l += (27 + tmp[k++]) - 48;
    }
    return l;
}
