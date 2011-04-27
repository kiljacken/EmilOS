#include "types.h"

// Compare two strings. Should return -1 if
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2)
{
  while (*str1 && *str2 && (*str1++ == *str2++))
    ;

  if (*str1 == '\0' && *str2 == '\0')
    return 0;

  if (*str1 == '\0')
    return -1;
  else return 1;
}
