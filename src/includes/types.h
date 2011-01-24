#ifndef TYPES_H
#define TYPES_H

#define NULL (void *)0

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

typedef unsigned char  uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned int  uint32_t;
typedef unsigned long  size_t;
typedef uint8_t bool;
#define true 1
#define false 0
#define asm  __asm__
#define volatile  __volatile__

#endif
