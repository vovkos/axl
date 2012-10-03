// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_G_DEF_H

//.............................................................................

// version

#define _AXL_VER 0x0300

//.............................................................................

// environment 

#define AXL_ENV_WIN 1  // win32 / win64 user mode module
#define AXL_ENV_NT  2  // NT native / kernel mode module

// hopefully more some day ;)

#if (!defined (_AXL_ENV))
#define _AXL_ENV AXL_ENV_WIN // defaulting to Win32 / Win64
#elif (_AXL_ENV == AXL_ENV_NT)
#else
#error _AXL_ENV set to unknown environment
#endif

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#ifdef _WIN64
#define AXL_PTR_BITNESS 64
#else
#define AXL_PTR_BITNESS 32
#endif // _WIN64

//.............................................................................

// convenient aliases

typedef char             int8_t;
typedef unsigned char    uint8_t;
typedef short            int16_t;
typedef unsigned short   uint16_t;
typedef long             int32_t;
typedef unsigned long    uint32_t;
typedef __int64          int64_t;
typedef unsigned __int64 uint64_t;

typedef int              bool_t;
typedef unsigned int     uint_t;
typedef unsigned char    uchar_t, byte_t;
typedef unsigned short   ushort_t, word_t;
typedef unsigned long    ulong_t, dword_t;
typedef __int64          longlong_t;
typedef unsigned __int64 ulonglong_t;
typedef unsigned __int64 qword_t;
typedef void*            handle_t;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#ifndef _INTPTR_T_DEFINED
#ifdef  _WIN64
typedef __int64          intptr_t;
#else
typedef _W64 int         intptr_t;
#endif
#define _INTPTR_T_DEFINED
#endif

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#ifndef _UINTPTR_T_DEFINED
#ifdef  _WIN64
typedef unsigned __int64  uintptr_t;
#else
typedef _W64 unsigned int uintptr_t;
#endif
#define _UINTPTR_T_DEFINED
#endif

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#ifdef _UNICODE
typedef wchar_t tchar_t;
#else
typedef char tchar_t;
#endif

//.............................................................................

#ifndef ASSERT
#define ASSERT(e) _ASSERTE (e)
#endif

//.............................................................................

#define AXL_CDECL      __cdecl
#define AXL_STDCALL    __stdcall
#define AXL_NO_VTABLE  __declspec (novtable)
#define AXL_SELECT_ANY __declspec (selectany)

//.............................................................................

#ifndef HIDWORD
#define HIDWORD(l) ((DWORD) ((((ulonglong_t) (l)) >> 32) & 0xffffffff))
#endif

//.............................................................................

// va_start_e can be used as an expression

#if defined (_M_IX86)
#define va_start_e(v) ((va_list) _ADDRESSOF (v) + _INTSIZEOF (v))
#elif defined (_M_AMD64)
#define va_start_e(v) ((va_list) _ADDRESSOF (v) + sizeof (__int64))
#else
#error Unsupported processor
#endif

//.............................................................................

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
		ASSERT (((void**) &x)[1] == NULL);
		break;

	default:
		ASSERT (false);
	}
#endif

	return *(void**) &x;
}

//.............................................................................

// offset of parent class

#define offsetof_class(c, p) \
	((size_t) ((p*) (c*) 1) - 1)

#define countof(a) \
	(sizeof (a) / sizeof (a [0]))

//.............................................................................

// TODO messages 

// usage: #pragma AXL_TODO ("implement something")

#define AXL_LTOA2(l) #l
#define AXL_LTOA(l)  AXL_LTOA2 (l)
#define AXL_MSG(s)   message (__FILE__ "(" AXL_LTOA (__LINE__) ") : " s)
#define AXL_MSG(s)   message (__FILE__ "(" AXL_LTOA (__LINE__) ") : " s)
#define AXL_TODO(s)  AXL_MSG ("TODO : " s)

//.............................................................................

// GUIDs that could be put into a namespace

#define AXL_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	extern const GUID AXL_SELECT_ANY name = \
		{ l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

//.............................................................................

// axl subsystems identifiers

#define AXL_SUBSYS_G    0
#define AXL_SUBSYS_MEM  1
#define AXL_SUBSYS_OBJ  2
#define AXL_SUBSYS_REF  3
#define AXL_SUBSYS_RTL  4
#define AXL_SUBSYS_EXE  5
#define AXL_SUBSYS_SYS  6
#define AXL_SUBSYS_SYSX 7
#define AXL_SUBSYS_IO   8
#define AXL_SUBSYS_GR   9
#define AXL_SUBSYS_LOG  10
#define AXL_SUBSYS_SCR  11
#define AXL_SUBSYS_NJC  12

#define AXL_SUBSYS_MASK(SubSys) (1 << (SubSys))
#define AXL_SUBSYS_MASK_ALL (-1)

//.............................................................................
