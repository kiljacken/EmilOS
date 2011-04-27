#include "types.h"

// Copy len bytes from src to dest.
void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
  for(; len != 0; len--) *dest++ = *src++;
}

