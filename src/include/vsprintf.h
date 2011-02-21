#ifndef VSPRINTF_H
#define VSPRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

int vsprintf(char *buf, const char *fmt, va_list args);

#ifdef __cplusplus
}
#endif

#endif
