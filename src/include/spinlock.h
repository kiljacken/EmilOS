#ifndef SPINLOCK_H
#define SPINLOCK_H

#include "types.h"

typedef struct spinlock {
	uint32_t tid;
	uint8_t lock;
} spinlock_t;

void LockSpinLock(spinlock_t *SpinLock);
void UnlockSpinLock(spinlock_t *SpinLock);
int TryLockSpinLock(spinlock_t *SpinLock);

#endif
