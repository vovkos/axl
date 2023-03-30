//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include <string.h>

#if (defined __amd64__)
__asm__(".symver memcpy, memcpy@GLIBC_2.2.5");
#elif (defined __i386__)
__asm__(".symver memcpy, memcpy@GLIBC_2.0");
#elif (defined __aarch64__)
__asm__(".symver memcpy, memcpy@GLIBC_2.17");
#else
#	error unsupported CPU architecture
#endif

//..............................................................................

void*
__wrap_memcpy(
	void* dst,
	const void *src,
	size_t size
) {
	return memcpy(dst, src, size);
}

//..............................................................................
