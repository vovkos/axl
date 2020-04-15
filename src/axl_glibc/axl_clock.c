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

#include <time.h>

#if (defined __amd64__)
__asm__(".symver clock_gettime, clock_gettime@GLIBC_2.2.5");
#elif (defined __i386__)
__asm__(".symver clock_gettime, clock_gettime@GLIBC_2.2");
#else
#	error unsupported CPU architecture
#endif

//..............................................................................

int
__wrap_clock_gettime(
	clockid_t id,
	struct timespec *ts
	)
{
	return clock_gettime(id, ts);
}

//..............................................................................
