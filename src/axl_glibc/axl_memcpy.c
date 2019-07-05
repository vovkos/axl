#include <string.h>

__asm__(".symver memcpy, memcpy@GLIBC_2.2.5");

void*
__wrap_memcpy(
	void* dst,
	const void *src,
	size_t size
	)
{
	return memcpy(dst, src, size);
}
