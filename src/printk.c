#include "printk.h"
#include "vsprintf.h"
#include <stdarg.h>
#include "spinlock.h"
#include "monitor.h"

spinlock_t printk_lock;

void printk (const char *fmt, ...)
{
	static char buf [1024];

 	va_list args;
 	int i;
 
 	va_start(args, fmt);
 	i = vsprintf(buf,fmt,args);
 	va_end(args);

 	buf[i] = '\0';
 	
 	while (0==TryLockSpinLock(&printk_lock))
	{
	}

 	monitor_write (buf);
 	
 	UnlockSpinLock(&printk_lock);
}
