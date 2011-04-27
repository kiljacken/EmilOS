// common.h -- Defines typedefs and some global functions.
//             From JamesM's kernel development tutorials.

#ifndef COMMON_H
#define COMMON_H

#include "types.h"
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

#endif // COMMON_H
