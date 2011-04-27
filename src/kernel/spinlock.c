#include "types.h"
#include "spinlock.h"
#include "thread.h"

void LockSpinLock(spinlock_t *SpinLock)
{
	if (!SpinLock->lock) {
		SpinLock->tid = get_tid();
		SpinLock->lock=1;
	}
}

void UnlockSpinLock(spinlock_t *SpinLock)
{
	if (get_tid() == SpinLock->tid) {
		SpinLock->tid =0;
		SpinLock->lock=0;
	}
}

int TryLockSpinLock(spinlock_t *SpinLock)
{
	if (SpinLock->lock==1) {
		return 0;
	} else {
		SpinLock->tid = get_tid();
		SpinLock->lock=1;
		return 1;
	}
}
