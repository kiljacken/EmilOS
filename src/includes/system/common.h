// common.h -- Defines typedefs and some global functions.
//             From JamesM's kernel development tutorials.

#ifndef COMMON_H
#define COMMON_H

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

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

#define outportb(port, value) outb(port, value);
#define inportb(port) inb(port);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

#define bit_get(p,m) ((p) & (m)) 
#define bit_set(p,m) ((p) |= (m)) 
#define bit_clear(p,m) ((p) &= ~(m)) 
#define bit_flip(p,m) ((p) ^= (m)) 
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m)) 
#define BIT(x) (0x01 << (x)) 
#define LONGBIT(x) ((unsigned long)0x00000001 << (x))

#ifdef DEBUG
#define DEBUG_MSG(msg) debug_msg(msg, __FILE__, __LINE__);
#else
#define DEBUG_MSG(msg)
#endif

// function/variable modifier.
#define PUBLIC  // none 
#define PRIVATE static

#define UNUSED __attribute__((unused)) 

extern void panic(const char *message, const char *file, u32int line);
extern void panic_assert(const char *file, u32int line, const char *desc);
extern void debug_msg(const char *message, const char *file, u32int line);

#endif // COMMON_H
