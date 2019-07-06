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
