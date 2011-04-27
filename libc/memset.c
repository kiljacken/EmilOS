#include "types.h"

// Write len copies of val into dest.
void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
  for ( ; len != 0; len--) *dest++ = val;
}
