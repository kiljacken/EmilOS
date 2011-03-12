#include "common.h"
#include "math.h"

uint32_t abs(uint32_t x) {
	if (x<0) {
		x=-x;
	}
	return x;
}
