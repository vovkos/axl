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
#include <fcntl.h>

#if (__i386__)
__asm__(".symver fcntl, fcntl@GLIBC_2.0");
#else
#	error unsupported CPU architecture
#endif

//..............................................................................

int
__wrap_fcntl(
	int fd,
	int cmd,
	char* arg
) {
	return fcntl(fd, cmd, arg);
}

//..............................................................................
