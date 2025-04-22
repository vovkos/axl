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

#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <spawn.h>
#include <time.h>

// secure_getenv is in <stdlib.h> but it's a GNU extension
// and its declaration may be ifdef-ed out

char*
secure_getenv(const char *name);

//..............................................................................

#if (defined __amd64__)
__asm__(".symver clock_gettime, clock_gettime@GLIBC_2.2.5");
__asm__(".symver memcpy,        memcpy@GLIBC_2.2.5");
__asm__(".symver posix_spawn,   posix_spawn@GLIBC_2.2.5");
__asm__(".symver posix_spawnp,  posix_spawnp@GLIBC_2.2.5");
__asm__(".symver secure_getenv, __secure_getenv@GLIBC_2.2.5");
__asm__(".symver exp,           exp@GLIBC_2.2.5");
__asm__(".symver exp2,          exp2@GLIBC_2.2.5");
__asm__(".symver log,           log@GLIBC_2.2.5");
__asm__(".symver log2,          log2@GLIBC_2.2.5");
__asm__(".symver log2f,         log2f@GLIBC_2.2.5");
__asm__(".symver pow,           pow@GLIBC_2.2.5");
#elif (defined __i386__)
__asm__(".symver clock_gettime, clock_gettime@GLIBC_2.2");
__asm__(".symver fcntl,         fcntl@GLIBC_2.0");
__asm__(".symver memcpy,        memcpy@GLIBC_2.0");
__asm__(".symver posix_spawn,   posix_spawn@GLIBC_2.2");
__asm__(".symver posix_spawnp,  posix_spawnp@GLIBC_2.2");
__asm__(".symver secure_getenv, __secure_getenv@GLIBC_2.0");
__asm__(".symver exp,           exp@GLIBC_2.0");
__asm__(".symver exp2,          exp2@GLIBC_2.1");
__asm__(".symver log,           log@GLIBC_2.0");
__asm__(".symver log2,          log2@GLIBC_2.1");
__asm__(".symver log2f,         log2f@GLIBC_2.1");
__asm__(".symver pow,           pow@GLIBC_2.0");
#elif (defined __aarch64__)
__asm__(".symver memcpy,        memcpy@GLIBC_2.17");
__asm__(".symver secure_getenv, secure_getenv@GLIBC_2.17");
__asm__(".symver exp,           exp@GLIBC_2.17");
__asm__(".symver exp2,          exp2@GLIBC_2.17");
__asm__(".symver log,           log@GLIBC_2.17");
__asm__(".symver log2,          log2@GLIBC_2.17");
__asm__(".symver log2f,         log2f@GLIBC_2.17");
__asm__(".symver pow,           pow@GLIBC_2.17");
#else
#	error unsupported CPU architecture
#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int
__wrap_clock_gettime(
	clockid_t id,
	struct timespec *ts
) {
	return clock_gettime(id, ts);
}

int
__wrap_fcntl(
	int fd,
	int cmd,
	char* arg
) {
	return fcntl(fd, cmd, arg);
}

void*
__wrap_memcpy(
	void* dst,
	const void *src,
	size_t size
) {
	return memcpy(dst, src, size);
}

int
__wrap_posix_spawn(
	pid_t* pid,
	const char* path,
	const posix_spawn_file_actions_t* file_actions,
	const posix_spawnattr_t* attrp,
	char* const argv[],
	char* const envp[]
) {
	return posix_spawn(pid, path, file_actions, attrp, argv, envp);
}

int
__wrap_posix_spawnp(
	pid_t* pid,
	const char* file,
	const posix_spawn_file_actions_t* file_actions,
	const posix_spawnattr_t* attrp,
	char* const argv[],
	char* const envp[]
) {
	return posix_spawnp(pid, file, file_actions, attrp, argv, envp);
}

char*
__wrap_secure_getenv(const char *name) {
	return secure_getenv(name);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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

float
__wrap_log2f(float x) {
	return log2f(x);
}

double
__wrap_pow(
	double x,
	double y
) {
	return pow(x, y);
}

//..............................................................................
