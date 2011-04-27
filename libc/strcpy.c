// Copy the NULL-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src)
{
  while (*src)
    *dest++ = *src++;
  *dest = '\0';
  return dest;
}
