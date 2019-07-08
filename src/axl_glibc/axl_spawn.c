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

#include <spawn.h>

#if (defined __amd64__)
__asm__(".symver posix_spawn,  posix_spawn@GLIBC_2.2.5");
__asm__(".symver posix_spawnp, posix_spawnp@GLIBC_2.2.5");
#elif (defined __i386__)
__asm__(".symver posix_spawn,  posix_spawn@GLIBC_2.2");
__asm__(".symver posix_spawnp, posix_spawnp@GLIBC_2.2");
#else
#	error unsupported CPU architecture
#endif

//..............................................................................

int
__wrap_posix_spawn(
	pid_t* pid,
	const char* path,
	const posix_spawn_file_actions_t* file_actions,
	const posix_spawnattr_t* attrp,
	char* const argv[],
	char* const envp[]
	)
{
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
	)
{
	return posix_spawnp(pid, file, file_actions, attrp, argv, envp);
}

//..............................................................................
