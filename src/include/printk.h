// 
// printk.h - Declares screen printing functions.
//            Written for JamesM's kernel development tutorials.
//

#ifndef PRINTK_H
#define PRINTK_H

#ifdef __cplusplus
extern "C" {
#endif

void printk (const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
