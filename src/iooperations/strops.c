#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <rsc/io/strops.h>

void
addCharacters(char const *src,
	      char *dst,
	      size_t len)
{
  size_t l = strlen(src);
  for (size_t i = 0; i < len; i++) {
    if (i >= l) {
      *dst++ = ' ';
    } else if (isalnum(*src)) {
      *dst++ = *src++;
    } else {
      *dst++ = '_';
      ++src;
    }
  }
}

long
stringLength12ToLong(char const *src)
{
  char *p;
  char *tmp = p = strdup(src);
  for (; !isalnum(*p); *p++ = '\0');
  for (char *end = p + strlen(p); end-- > p && !isalnum(*p); *end = '\0');
  size_t len = strlen(p);
  if (strlen(p) == 0) {
    free(tmp);
    return 0L;
  }

  for (char *c = p; *c; ++c) {
    if (islower(*c) || isdigit(*c)) {
      continue;
    } else if (isupper(*c)) {
      *c = tolower(*c);
    } else {
      *c = ' ';
    }
  }
  
  if (len > 12) {
    len = 12;
  }
  long l = 0L;
  for (; len--; ++p) {
    l *= 37L;
    if (islower(*p)) {
      l += 1 + (*p - 'a');
    } else if (isdigit(*p)) {
      l += 27 + (*p - '0');
    }
  }
  free(tmp);
  return l;
}
