#include "common.h"
#include "spinlock.h"

void LockSpinLock(SPIN_LOCK *SpinLock)
{
	*SpinLock=1;
}

void UnlockSpinLock(SPIN_LOCK *SpinLock)
{
	*SpinLock=0;
}

int TryLockSpinLock(SPIN_LOCK *SpinLock)
{
	if (*SpinLock==1) { return 0; }
	else { *SpinLock=1; return 1; }
}
