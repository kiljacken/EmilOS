#ifndef SPINLOCK_H
#define SPINLOCK_H

#include "common.h"

typedef unsigned int SPIN_LOCK;

void LockSpinLock(SPIN_LOCK *SpinLock);
void UnlockSpinLock(SPIN_LOCK *SpinLock);
int TryLockSpinLock(SPIN_LOCK *SpinLock);



#endif
