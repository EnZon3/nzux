#include "string.h"

void* memcpy(void* restrict dest, const void* restrict src, size_t n) {
	unsigned char* dst = (unsigned char*) dest;
	const unsigned char* srce = (const unsigned char*) src;
	for (size_t i = 0; i < n; i++)
		dst[i] = srce[i];
	return dest;
}