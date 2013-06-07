// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
#		error Unsupported compiler
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

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (_AXL_CPU == AXL_CPU_X86)
#	define _AXL_CPU_STRING "x86"
#elif (_AXL_CPU == AXL_CPU_AMD64)
#	define _AXL_CPU_STRING "amd64"
#else
#	error _AXL_CPU is set to unknown CPU arch id
#endif


//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (_AXL_CPU == AXL_CPU_AMD64)
#	define _AXL_PTR_BITNESS 64
#else
#	define _AXL_PTR_BITNESS 32
#endif

//.............................................................................

// runtime environment ids

#define AXL_ENV_WIN   1  // win32 / win64 user mode module
#define AXL_ENV_NT    2  // NT native / kernel mode module
#define AXL_ENV_POSIX 3  // Unix / Linux 

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#ifndef _AXL_ENV 
#	if (_AXL_CPP == AXL_CPP_MSC)
#		define _AXL_ENV AXL_ENV_WIN // default runtime environment is Win32 / Win64
#	elif (_AXL_CPP == AXL_CPP_GCC)
#		define _AXL_ENV AXL_ENV_POSIX
#	endif
#elif (_AXL_ENV != AXL_ENV_WIN && _AXL_ENV != AXL_ENV_NT && _AXL_ENV != AXL_ENV_POSIX)
#	error _AXL_ENV is set to unknown runtime environment id
#endif

//.............................................................................

#if (_AXL_CPP == AXL_CPP_MSC)
#	define AXL_CDECL      __cdecl
#	define AXL_STDCALL    __stdcall
#	define AXL_SELECT_ANY __declspec (selectany)
#	define AXL_NO_VTABLE  __declspec (novtable)
#elif (_AXL_CPP == AXL_CPP_GCC)
#	define AXL_CDECL      __attribute__ ((cdecl))
#	define AXL_STDCALL    __attribute__ ((stdcall))
#	define AXL_SELECT_ANY __attribute__ ((weak))
#	define AXL_NO_VTABLE  
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

typedef int              bool_t;
typedef unsigned int     uint_t;
typedef unsigned char    uchar_t;
typedef unsigned short   ushort_t;
typedef unsigned long    ulong_t;

typedef uint8_t          byte_t;
typedef uint16_t         word_t;
typedef uint64_t         qword_t;

#if (_AXL_CPP == AXL_CPP_MSC)
typedef ulong_t          dword_t;
#else
typedef uint32_t         dword_t;
#endif

typedef void*            handle_t;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef char             utf8_t;

#if (WCHAR_MAX <= 0xffff)
typedef wchar_t          utf16_t;
typedef int32_t          utf32_t;
#else
typedef int16_t          utf16_t;
typedef wchar_t          utf32_t;
#endif

//.............................................................................

// vararg routines

#ifndef va_start
#	error include <stdarg.h> prior to inclusion of AXL headers
#endif

// this struct wrap serves two purposes:
// first, makes sure va_list isn't get modified if passed as argument 
// second, makes it possible to simply assign one to another
// thanks a lot gcc

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

// number of elements in array

#ifndef countof
#	define countof(a) (sizeof (a) / sizeof (a [0]))
#endif

// pvoid_cast is mostly used for casting member function pointers to void*

template <typename T>
void* 
pvoid_cast (T x)
{
#ifdef _DEBUG
	switch (sizeof (x))
	{
	case sizeof (void*):
		break;

	case sizeof (void*) * 2:
		ASSERT (((void**) &x) [1] == NULL);
		break;

	default:
		ASSERT (false);
	}
#endif

	return *(void**) &x;
}

//.............................................................................

// non-conflicting min / max macros

#define AXL_MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define AXL_MAX(a, b)  (((a) > (b)) ? (a) : (b))

//.............................................................................

// TODO messages 

// usage: #pragma AXL_TODO ("implement something")

#define AXL_LTOA2(l) #l
#define AXL_LTOA(l)  AXL_LTOA2 (l)
#define AXL_MSG(s)   message (__FILE__ "(" AXL_LTOA (__LINE__) ") : " s)
#define AXL_TODO(s)  AXL_MSG ("TODO : " s)

//.............................................................................

// axl subsystems identifiers

#define AXL_SUBSYS_G    0
#define AXL_SUBSYS_MEM  1
#define AXL_SUBSYS_OBJ  2
#define AXL_SUBSYS_REF  3
#define AXL_SUBSYS_RTL  4
#define AXL_SUBSYS_EXE  5
#define AXL_SUBSYS_IO   8
#define AXL_SUBSYS_GUI  9
#define AXL_SUBSYS_LOG  10
#define AXL_SUBSYS_JNC  12

#define AXL_SUBSYS_MASK(SubSys) (1 << (SubSys))
#define AXL_SUBSYS_MASK_ALL (-1)

//.............................................................................

#if (_AXL_CPP == AXL_CPP_MSC)
#	pragma warning (disable: 4146)
#	pragma warning (disable: 4267)
#endif

#if (_AXL_CPP == AXL_CPP_GCC)
#	pragma GCC diagnostic ignored "-Wunused-parameter"
#	pragma GCC diagnostic ignored "-Wparentheses"
#	pragma GCC diagnostic ignored "-Wsign-compare"
#	pragma GCC diagnostic ignored "-Wformat"
#	pragma GCC diagnostic ignored "-Wmultichar"
#endif


//.............................................................................

