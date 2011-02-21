//
// timer.h -- Defines the interface for all PIT-related functions.
//            Written for JamesM's kernel development tutorials.

#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

void init_timer (uint32_t frequency);

#ifdef __cplusplus
}
#endif

#endif
