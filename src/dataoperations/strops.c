#include <string.h>
#include <ctype.h>

#include <rsc/io/strops.h>

int skipLeadingNonChars(const char *src)
{
    int len = strlen(src), s;
    for (s = 0; s < len; ++s)
        if ((*src++ >= 'a' && *src++ <= 'z')
                || (*src++ >= 'A' && *src++ <= 'Z')
                || (*src++ >= '0' && *src++ <= '9'))
            break;
    return s;
}

int skipTrailingNonChars(const char *src)
{
    int len = strlen(src), e;
    for (src += len-1, e = len; e > 0; --e, --src)
        if ((*src >= 'a' && *src <= 'z')
                || (*src >= 'A' && *src <= 'Z')
                || (*src >= '0' && *src <= '9'))
            break;
    return e;
}

void addCharacters(const char *src, char *dst, int len)
{
    char const *p = dst;
    int l = strlen(src);
    for (int i = 0; i < len; i++)
    {
        if (i >= l)
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
    printf("dst: '%s'", p);
}

long stringLength12ToLong(const char *src)
{
    int s = skipLeadingNonChars(src);
    int e = skipTrailingNonChars(src);
    if (e <= s)
        return 0L;
    int len = e - s;
    char tmp[len];

    src += s;
    for (int i = s, k = 0; i < e; i++) // copy string
        if ((*src >= 'a' && *src <= 'z')
                || (*src >= '0' && *src <= '9'))
            tmp[k++] = *src++;
        else if (*src >= 'A' && *src <= 'Z')
            tmp[k++] =  tolower(*src++);
        else
        {
            tmp[k++] = ' ';
            ++src;
        }
    if (len > 12)
        len = 12;
    long l = 0L;
    for (int j = 0, k = 0; j < len; j++)
    {
        l *= 37L;
        if (tmp[k] >= 'a' && tmp[k] <= 'z')
            l += (1 + tmp[k++]) - 'a';
        else if (tmp[k] >= '0' && tmp[k] <= '9')
            l += 27 + (tmp[k++] - '0');
    }
    return l;
}

void getBytes(const char *src, char *dst, int len)
{
    memcpy(dst, src, len);
}

int addString(const char *src, char *dst)
{
    int len = strlen(src);
    memcpy(dst, src, len);
    char *bgn = dst;
    *dst += len;
    *dst++ = 10;
    return dst - bgn;
}
