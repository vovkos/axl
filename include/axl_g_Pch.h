// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

//.............................................................................

// C standard headers

#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#if (_WIN32)
#	define _CRT_SECURE_NO_WARNINGS  // useless warnings about "unsafe" string functions
#	define _SCL_SECURE_NO_WARNINGS  // useless warnings about "unsafe" iterator operations
#endif

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include <errno.h>

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// C++ standard headers

#include <new>
#include <typeinfo>

//.............................................................................

#if (_WIN32)
#	include <crtdbg.h>
#	define ASSERT _ASSERTE
#
#	ifndef _WIN32_WINNT             // Specifies that the minimum required platform is Windows Vista.
#		define _WIN32_WINNT 0x0600  // Change this to the appropriate value to target other versions of Windows.
#	endif
#
#	define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
#
#	include <tchar.h>
#	include <guiddef.h>
#	include <cguid.h>
#	include <winsock2.h>
#	include <ws2ipdef.h>
#	include <commctrl.h>
#	include <shellapi.h>
#endif

//.............................................................................

#if (__GNUC__)
#	include <assert.h>
#	define ASSERT assert
#
#	include <unistd.h>
#	include <pthread.h>
#	include <semaphore.h>
#	include <fcntl.h>
#	include <limits.h>
#	include <byteswap.h>
#	include <sys/stat.h>
#	include <sys/mman.h>
#	include <sys/socket.h>
#	include <sys/ioctl.h>
#	include <netinet/in.h>
#	include <netinet/ip.h>
#	include <arpa/inet.h>
#	include <termios.h>
#	include <libgen.h>
#	undef basename
#
#	define _stricmp    strcasecmp
#	define _wcsicmp    wcscasecmp
#	define _snprintf   snprintf
#	define _snwprintf  swprintf
#	define _alloca     alloca
#	define _strtoi64   strtoll
#	define _strtoui64  strtoull
#
#	define MAKEWORD(a, b) ((word_t)(((byte_t)((a) & 0xff)) | ((word_t)((byte_t)((b) & 0xff))) << 8))
#	define MAKELONG(a, b) ((int32_t)(((word_t)((a) & 0xffff)) | ((dword_t)((word_t)((b) & 0xffff))) << 16))
#	define LOWORD(l)      ((word_t)((l) & 0xffff))
#	define HIWORD(l)      ((word_t)(((l) >> 16) & 0xffff))
#	define LOBYTE(w)      ((byte_t)((w) & 0xff))
#	define HIBYTE(w)      ((byte_t)(((w) >> 8) & 0xff))
#	define RGB(r,g,b)     ((dword_t)(((byte_t)(r)|((word_t)((byte_t)(g))<<8))|(((dword_t)(byte_t)(b))<<16)))
#endif

//.............................................................................

// strnchr / wcsnchr

inline
const char*
strchr_e (
	const char* p,
	const char* pEnd,
	char c
	)
{
	while (p < pEnd)
	{
		if (*p == c)
			return p;

		p++;
	}

	return NULL;
}

inline
char*
strchr_e (
	char* p,
	char* pEnd,
	char c
	)
{
	while (p < pEnd)
	{
		if (*p == c)
			return p;

		p++;
	}

	return NULL;
}

inline
const wchar_t*
wcschr_e (
	const wchar_t* p,
	const wchar_t* pEnd,
	wchar_t c
	)
{
	while (p < pEnd)
	{
		if (*p == c)
			return p;

		p++;
	}

	return NULL;
}

inline
wchar_t*
wcschr_e (
	wchar_t* p,
	wchar_t* pEnd,
	wchar_t c
	)
{
	while (p < pEnd)
	{
		if (*p == c)
			return p;

		p++;
	}

	return NULL;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const char*
strnchr (
	const char* p,
	size_t Length,
	char c
	)
{
	return strchr_e (p, p + Length, c);
}

inline
char*
strnchr (
	char* p,
	size_t Length,
	char c
	)
{
	return strchr_e (p, p + Length, c);
}

inline
const wchar_t*
wcsnchr (
	const wchar_t* p,
	size_t Length,
	wchar_t c
	)
{
	return wcschr_e (p, p + Length, c);
}

inline
wchar_t*
wcsnchr (
	wchar_t* p,
	size_t Length,
	wchar_t c
	)
{
	return wcschr_e (p, p + Length, c);
}

//.............................................................................

#include "axl_g_Def.h"
#include "axl_g_WarningSuppression.h"