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

#include <stdlib.h>

#if (defined __amd64__)
__asm__(".symver secure_getenv, __secure_getenv@GLIBC_2.2.5");
#elif (defined __i386__)
__asm__(".symver secure_getenv, __secure_getenv@GLIBC_2.0");
#else
#	error unsupported CPU architecture
#endif

//..............................................................................

char*
__wrap_secure_getenv(const char *name)
{
	return secure_getenv(name);
}

//..............................................................................
