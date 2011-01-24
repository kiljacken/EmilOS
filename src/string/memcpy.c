/* $Id: memcpy.c 416 2010-05-15 00:39:28Z solar $ */

/* memcpy( void *, const void *, size_t )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <string.h>

void * memcpy( void * _PDCLIB_restrict s1, const void * _PDCLIB_restrict s2, size_t n )
{
    char * dest = (char *) s1;
    const char * src = (const char *) s2;
    while ( n-- )
    {
        *dest++ = *src++;
    }
    return s1;
}
