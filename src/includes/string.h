/* $Id: string.h 503 2010-12-23 06:15:28Z solar $ */

/* 7.21 String handling <string.h>

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <types.h>
void * memcpy( void * s1, const void * s2, size_t n );
void * memmove( void * s1, const void * s2, size_t n );
char * strcpy( char * s1, const char * s2 );
int memcmp( const void * s1, const void * s2, size_t n );
int strcmp( const char * s1, const char * s2 );
void * memset( void * s, int c, size_t n );
size_t strlen( const char * s );
