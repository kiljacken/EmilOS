#include "types.h"

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
char *strcat(char *dest, const char *src)
{
  while (*dest)
    *dest = *dest+1;// = *dest++;

  while (*src)
    *dest++ = *src++;
  *dest = '\0';
  return dest;
}
