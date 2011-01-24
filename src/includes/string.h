/* $Id: string.h 503 2010-12-23 06:15:28Z solar $ */

/* 7.21 String handling <string.h>

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <types.h>

/* String function conventions */

/*
   In any of the following functions taking a size_t n to specify the length of
   an array or size of a memory region, n may be 0, but the pointer arguments to
   the call shall still be valid unless otherwise stated.
*/

/* Copying functions */

/* Copy a number of n characters from the memory area pointed to by s2 to the
   area pointed to by s1. If the two areas overlap, behaviour is undefined.
   Returns the value of s1.
*/
void * memcpy( void * _PDCLIB_restrict s1, const void * _PDCLIB_restrict s2, size_t n );

/* Copy a number of n characters from the memory area pointed to by s2 to the
   area pointed to by s1. The two areas may overlap.
   Returns the value of s1.
*/
void * memmove( void * _PDCLIB_restrict s1, const void * _PDCLIB_restrict s2, size_t n );

/* Copy the character array s2 (including terminating '\0' byte) into the
   character array s1.
   Returns the value of s1.
*/
char * strcpy( char * _PDCLIB_restrict s1, const char * _PDCLIB_restrict s2 );


/* Comparison functions */

/* Compare the first n characters of the memory areas pointed to by s1 and s2.
   Returns 0 if s1 == s2, a negative number if s1 < s2, and a positive number if
   s1 > s2.
*/
int memcmp( const void * s1, const void * s2, size_t n );

/* Compare the character arrays s1 and s2.
   Returns 0 if s1 == s2, a negative number if s1 < s2, and a positive number if
   s1 > s2.
*/
int strcmp( const char * s1, const char * s2 );


/* Miscellaneous functions */

/* Write the character c (interpreted as unsigned char) to the first n
   characters of the memory area pointed to by s.
   Returns s.
*/
void * memset( void * s, int c, size_t n );

/* Returns the length of the string s (excluding terminating '\0').
*/
size_t strlen( const char * s );

#endif
