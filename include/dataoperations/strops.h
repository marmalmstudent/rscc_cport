#ifndef STROPS_H
#define STROPS_H

#include <string.h>
#include <ctype.h> // for tolower & toupper

void addCharacters(const char *src, char *dst, int len);
long stringLength12ToLong(char *src);

#endif // STROPS_H
