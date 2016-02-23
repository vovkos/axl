// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_G_DEF_H

//.............................................................................

#ifndef __cplusplus
#	error AXL library requires a C++ compiler
#endif

#ifndef ASSERT
#	error define ASSERT (e) prior to inclusion of AXL headers
#endif

//.............................................................................

// AXL library version

#define _AXL_VER 0x0300

//.............................................................................

// C++ compiler ids

#define AXL_CPP_MSC  1  // Microsoft Visual C++ compiler (cl.exe)
#define AXL_CPP_GCC  2  // GNU C++ compiler
#define AXL_CPP_ICC  3  // Intel C++ compiler

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#ifndef _AXL_CPP // detect C++ compiler
#	ifdef _MSC_VER
#		define _AXL_CPP AXL_CPP_MSC
#	elif (defined __GNUC__)
#		define _AXL_CPP AXL_CPP_GCC
#	else
#		error unsupported compiler
#	endif
#elif (_AXL_CPP != AXL_CPP_MSC && _AXL_CPP != AXL_CPP_GCC)
#	error _AXL_CPP is set to unknown C++ compiler id
#endif

//.............................................................................

// CPU arch ids

#define AXL_CPU_X86   1  // Intel i386
#define AXL_CPU_AMD64 2  // AMD64 / Intel x86_64

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#ifndef _AXL_CPU // detect CPU arch
#	if (_AXL_CPP == AXL_CPP_MSC)
#		if (defined _M_IX86)
#			define _AXL_CPU AXL_CPU_X86
#		elif (defined _M_AMD64)
#			define _AXL_CPU AXL_CPU_AMD64
#		endif
#	elif (_AXL_CPP == AXL_CPP_GCC)
#		if defined __i386__
#			define _AXL_CPU AXL_CPU_X86
#		elif defined __amd64__
#			define _AXL_CPU AXL_CPU_AMD64
#		endif
#	endif
#endif

#if (_AXL_CPU == AXL_CPU_X86)
#	define _AXL_CPU_STRING "x86"
#elif (_AXL_CPU == AXL_CPU_AMD64)
#	define _AXL_CPU_STRING "amd64"
#else
#	error _AXL_CPU is set to unknown CPU arch id
#endif

#if (_AXL_CPU == AXL_CPU_AMD64)
#	define _AXL_PTR_BITNESS 64
#	define _AXL_PTR_SIZE    8
#else
#	define _AXL_PTR_BITNESS 32
#	define _AXL_PTR_SIZE    4
#endif

//.............................................................................

// runtime environment ids

#define AXL_ENV_WIN   1  // win32 / win64 user mode module
#define AXL_ENV_NT    2  // NT native / kernel mode module
#define AXL_ENV_POSIX 3  // Unix / Linux / MacOSX

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#ifndef _AXL_ENV
#	if (_AXL_CPP == AXL_CPP_MSC)
#		define _AXL_ENV AXL_ENV_WIN // default runtime environment is Win32 / Win64
#	elif (_AXL_CPP == AXL_CPP_GCC)
#		define _AXL_ENV AXL_ENV_POSIX
#	endif
#endif

#if (_AXL_ENV < AXL_ENV_WIN || _AXL_ENV > AXL_ENV_POSIX)
#	error _AXL_ENV is set to unknown runtime environment id
#endif

//.............................................................................

// POSIX flavors

#define AXL_POSIX_LINUX   1
#define AXL_POSIX FREEBSD 2
#define AXL_POSIX_DARWIN  3

#if (_AXL_ENV != AXL_ENV_POSIX)
#	undef _AXL_POSIX
#else
#	if (!defined _AXL_POSIX)
#		ifdef __linux__
#			define _AXL_POSIX AXL_POSIX_LINUX
#		elif (defined __FreeBSD__)
#			define _AXL_POSIX AXL_POSIX_FREEBSD
#		elif (defined __APPLE__ && defined __MACH__)
#			define _AXL_POSIX AXL_POSIX_DARWIN
#		endif
#	endif
#
#	if (_AXL_POSIX < AXL_POSIX_LINUX || _AXL_POSIX > AXL_POSIX_DARWIN)
#		error _AXL_POSIX is set to unknown POSIX id
#	endif
#endif

//.............................................................................

#if (_AXL_CPP == AXL_CPP_MSC)
#	define AXL_CDECL      __cdecl
#	define AXL_STDCALL    __stdcall
#	define AXL_SELECT_ANY __declspec (selectany)
#	define AXL_NO_VTABLE  __declspec (novtable)
#	define AXL_EXPORT     __declspec (dllexport)
#
#	ifdef _DEBUG
#		define _AXL_DEBUG 1
#	else
#		undef _AXL_DEBUG
#	endif
#elif (_AXL_CPP == AXL_CPP_GCC)
#	if (_AXL_CPU == AXL_CPU_X86)
#		define AXL_CDECL   __attribute__ ((cdecl))
#		define AXL_STDCALL __attribute__ ((stdcall))
#	else
#		define AXL_CDECL
#		define AXL_STDCALL
#	endif
#	define AXL_SELECT_ANY  __attribute__ ((weak))
#	define AXL_NO_VTABLE
#	define AXL_EXPORT __attribute__ ((visibility ("default")))
#
#	ifdef NDEBUG
#		undef _DEBUG
#		undef _AXL_DEBUG
#	else
#		define _DEBUG
#		define _AXL_DEBUG 1
#	endif
#endif

//.............................................................................

#if (_AXL_CPP == AXL_CPP_GCC)
#	define AXL_NO_ASAN __attribute__((no_sanitize_address))
#else
#	define AXL_NO_ASAN
#endif

//.............................................................................

// common type aliases

#ifndef INTPTR_MIN
#	error include <stdint.h> prior to inclusion of AXL headers
#endif

// stdint.h defines:
//
// int8_t
// uint8_t
// int16_t
// uint16_t
// int32_t
// uint32_t
// int64_t
// uint64_t
// intptr_t
// uintptr_t

typedef int               bool_t;
typedef unsigned int      uint_t;
typedef unsigned char     uchar_t;
typedef unsigned short    ushort_t;
typedef unsigned long     ulong_t;

typedef uint8_t           byte_t;
typedef uint16_t          word_t;
typedef uint64_t          qword_t;

#if (_AXL_CPP == AXL_CPP_MSC)
typedef ulong_t           dword_t;
#else
typedef uint32_t          dword_t;
#endif

#if (_AXL_PTR_BITNESS == 64)
#	if (_AXL_CPP == AXL_CPP_GCC)
typedef __int128          int128_t;
typedef unsigned __int128 uint128_t;
typedef int128_t          intdptr_t;
typedef uint128_t         uintdptr_t;
#	endif
#else
typedef int64_t           intdptr_t;
typedef uint64_t          uintdptr_t;
#endif

typedef void*             handle_t;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef char              utf8_t;

#if (WCHAR_MAX <= 0xffff)
typedef wchar_t           utf16_t;
typedef int32_t           utf32_t;
#else
typedef int16_t           utf16_t;
typedef wchar_t           utf32_t;
#endif

//.............................................................................

// vararg routines

#ifndef va_start
#	error include <stdarg.h> prior to inclusion of AXL headers
#endif

// this struct wrap serves two purposes:
// first, makes sure va_list isn't get modified if passed as argument
// second, makes it possible to simply assign one to another

struct axl_va_list
{
	va_list m_va;

	axl_va_list ()
	{
	}

	axl_va_list (va_list va)
	{
#if (_AXL_CPP == AXL_CPP_GCC)
		va_copy (m_va, va);
#else
		m_va = va;
#endif
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_VA_START(va, a) \
	va_start (va.m_va, a)

#define AXL_VA_ARG(va, t) \
	va_arg (va.m_va, t)

#define AXL_VA_DECL(va, a) \
	axl_va_list va; \
	AXL_VA_START (va, a);

//.............................................................................

// pvoid_cast is mostly used for casting member function pointers to void*

template <typename T>
void*
pvoid_cast (T x)
{
	ASSERT (sizeof (x) == sizeof (void*) || sizeof (x) == sizeof (void*) * 2);
	return *(void**) &x;
}

#if (_AXL_CPP != AXL_CPP_GCC)

// this overload is to make it's ok to pvoid_cast (NULL) on 64-bit systems
// gcc takes care of it automatically (it will not attempt to use 'int' for NULL)

inline
void*
pvoid_cast (int x)
{
	return (void*) (intptr_t) x;
}

#endif

//.............................................................................

// non-conflicting portable macro utilities

#ifndef countof
#	define countof(a) (sizeof (a) / sizeof (a [0]))
#endif

#ifndef lengthof
#	define lengthof(s) (sizeof (s) / sizeof (s [0]) - 1)
#endif

#ifndef offsetof
#	define offsetof(Type, m_field) ((size_t) &((Type*) 1)->m_field - 1)
#endif

#define AXL_MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define AXL_MAX(a, b)  (((a) > (b)) ? (a) : (b))

#define AXL_OFFSETOF(Type, m_field) \
	((size_t) &((Type*) 1)->m_field - 1)

#define AXL_CONTAINING_RECORD(address, Type, m_field) \
	((Type*) ((char*) (address) - AXL_OFFSETOF (Type, m_field)))

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// ensure class is never accidentally copied

#define AXL_DISABLE_COPY(Class) \
private: \
	Class (const Class&);  \
	void \
	operator = (const Class&); \

//.............................................................................

// TODO messages

// usage: #pragma AXL_TODO ("implement something")

#define AXL_LTOA2(l) #l
#define AXL_LTOA(l)  AXL_LTOA2 (l)
#define AXL_MSG(s)   message (__FILE__ "(" AXL_LTOA (__LINE__) ") : " s)
#define AXL_TODO(s)  AXL_MSG ("TODO : " s)

//.............................................................................
