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

#include <math.h>

#if (defined __amd64__)
__asm__(".symver exp,  exp@GLIBC_2.2.5");
__asm__(".symver exp2, exp2@GLIBC_2.2.5");
__asm__(".symver log,  log@GLIBC_2.2.5");
__asm__(".symver log2, log2@GLIBC_2.2.5");
__asm__(".symver pow,  pow@GLIBC_2.2.5");
#elif (defined __i386__)
__asm__(".symver exp,  exp@GLIBC_2.0");
__asm__(".symver exp2, exp2@GLIBC_2.1");
__asm__(".symver log,  log@GLIBC_2.0");
__asm__(".symver log2, log2@GLIBC_2.1");
__asm__(".symver pow,  pow@GLIBC_2.0");
#else
#	error unsupported CPU architecture
#endif

//..............................................................................

double
__wrap_exp(double x) {
	return exp(x);
}

double
__wrap_exp2(double x) {
	return exp2(x);
}

double
__wrap_log(double x) {
	return log(x);
}

double
__wrap_log2(double x) {
	return log2(x);
}

double
__wrap_pow(
	double x,
	double y
) {
	return pow(x, y);
}

//..............................................................................
