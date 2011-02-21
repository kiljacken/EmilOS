//
// panic.h -- Defines the interface for bringing the system to an abnormal halt.
//            Written for JamesM's kernel development tutorials.
//

#ifndef PANIC_H
#define PANIC_H

#ifdef __cplusplus
extern "C" {
#endif

void panic (const char *msg);

#ifdef __cplusplus
}
#endif

#endif
