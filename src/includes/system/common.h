// common.h -- Defines typedefs and some global functions.
//             From JamesM's kernel development tutorials.

#ifndef COMMON_H
#define COMMON_H

#include <types.h>
#include <string.h>

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

#define outportb(port, value) outb(port, value);
#define inportb(port) inb(port);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

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
