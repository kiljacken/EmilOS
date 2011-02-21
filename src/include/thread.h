// 
// thread.h -- Declares functions and structures for multithreading.
//             Written for JamesM's kernel development tutorials.
//

#ifndef THREAD_H
#define THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef struct
{
  uint32_t id;                  // Thread ID.
  uint32_t esp, ebp, ebx, esi, edi;
} thread_t;

thread_t *init_threading ();

thread_t *create_thread (int (*fn)(void*), void *arg, uint32_t *stack);

void switch_thread (thread_t *next);

#ifdef __cplusplus
}
#endif

#endif
