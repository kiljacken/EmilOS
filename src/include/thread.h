// 
// thread.h -- Declares functions and structures for multithreading.
//             Written for JamesM's kernel development tutorials.
//

#ifndef THREAD_H
#define THREAD_H

#include "types.h"

typedef struct
{
  uint32_t id;                  // Thread ID.
  uint32_t esp, ebp, ebx, esi, edi;
  uint8_t dead; // Not zero when thread has finished
} thread_t;

thread_t *init_threading ();

thread_t *create_thread (int (*fn)(void*), void *arg, uint32_t *stack);

void switch_thread (thread_t *next);

uint32_t get_tid();

#endif
