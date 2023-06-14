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

#pragma once

#define _AXL_G_PCH_H

#ifndef __cplusplus
#	error AXL library requires a C++ compiler
#endif

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// 1) detect build/target environment section

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// detect C++ compiler

#ifdef _MSC_VER
#	define _AXL_CPP_MSC 1
#	define AXL_CPP_MSC_VERSION_MAJOR (_MSC_VER / 100)
#	define AXL_CPP_MSC_VERSION_MINOR (_MSC_VER % 100)
#	define AXL_CPP_MSC_VERSION (((_MSC_VER / 100) << 8) | (_MSC_VER % 100))
#	define AXL_CPP_STRING "Microsoft Visual C++"
#	if (AXL_CPP_MSC_VERSION >= 0x1000)
#		define _AXL_CPP_HAS_RVALUE_REF 1
#	endif
#elif (defined __GNUC__)
#	define _AXL_CPP_GCC 1
#	define AXL_CPP_GCC_VERSION_MAJOR __GNUC__
#	define AXL_CPP_GCC_VERSION_MINOR __GNUC_MINOR__
#	define AXL_CPP_GCC_VERSION ((__GNUC__ << 8) | __GNUC_MINOR__)
#	ifdef __clang__
#		define _AXL_CPP_CLANG 1
#		define AXL_CPP_CLANG_VERSION_MAJOR __clang_major__
#		define AXL_CPP_CLANG_VERSION_MINOR __clang_minor__
#		define AXL_CPP_CLANG_VERSION ((__clang_major__ << 8) | __clang_minor__)
#		define AXL_CPP_STRING "Clang/LLVM"
#	elif (defined __ICC)
#		define _AXL_CPP_ICC 1
#		define AXL_CPP_ICC_VERSION_MAJOR (__ICC / 100)
#		define AXL_CPP_ICC_VERSION_MINOR (__ICC % 100)
#		define AXL_CPP_ICC_VERSION (((__ICC / 100) << 8) | (__ICC % 100))
#		define AXL_CPP_STRING "Intel C++"
#	else
#		define AXL_CPP_STRING "GNU C++"
#	endif
#	if (__cplusplus >= 201103L)
#		define _AXL_CPP_HAS_RVALUE_REF 1
#	endif
#else
#	error unsupported C++ compiler
#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// detect CPU architecture

#if (_AXL_CPP_MSC)
#	if (defined _M_IX86)
#		define _AXL_CPU_IA32  1
#		define _AXL_CPU_X86   1
#	elif (defined _M_AMD64)
#		define _AXL_CPU_AMD64 1
#	elif (defined _M_ARM)
#		define _AXL_CPU_ARM32 1
#	elif (defined _M_ARM64)
#		define _AXL_CPU_ARM64 1
#	endif
#elif (_AXL_CPP_GCC)
#	if defined __i386__
#		define _AXL_CPU_IA32  1
#		define _AXL_CPU_X86   1
#	elif (defined __amd64__)
#		define _AXL_CPU_AMD64 1
#	elif (defined __arm__)
#		define _AXL_CPU_ARM32 1
#	elif (defined __aarch64__)
#		define _AXL_CPU_ARM64 1
#	endif
#endif

#if (_AXL_CPU_X86)
#	define AXL_CPU_STRING  "x86"
#	define AXL_PTR_SIZE    4
#elif (_AXL_CPU_AMD64)
#	define AXL_CPU_STRING  "amd64"
#	define AXL_PTR_SIZE    8
#elif (_AXL_CPU_ARM32)
#	define AXL_CPU_STRING  "arm32"
#	define AXL_PTR_SIZE    4
#elif (_AXL_CPU_ARM64)
#	define AXL_CPU_STRING  "arm64"
#	define AXL_PTR_SIZE    8
#else
#	error unsupported CPU architecture
#endif

#define AXL_PTR_BITS (AXL_PTR_SIZE * 8) // often times it's more natural to use bit size

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// detect OS

#ifdef _WIN32
#	define _AXL_OS_WIN 1
#	define AXL_OS_STRING "Microsoft Windows"
#elif (defined __unix__)
#	define _AXL_OS_POSIX 1
#	ifdef __linux__
#		define _AXL_OS_LINUX 1
#		define AXL_OS_STRING "Linux"
#	elif (defined __sun__)
#		define _AXL_OS_SOLARIS 1
#		define AXL_OS_STRING "Solaris"
#	elif (defined __FreeBSD__ || defined __OpenBSD__ || defined __NetBSD__)
#		define _AXL_OS_BSD 1
#		define AXL_OS_STRING "BSD"
#	endif
#elif (defined __APPLE__ && defined __MACH__)
#	define _AXL_OS_POSIX  1
#	define _AXL_OS_BSD    1
#	define _AXL_OS_DARWIN 1
#	define AXL_OS_STRING "Apple Darwin"
#else
#	error unsupported OS
#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// detect Debug build

#if (_AXL_CPP_MSC)
#	ifdef _DEBUG
#		define _AXL_DEBUG 1
#	endif
#else
#	ifndef NDEBUG
#		define _AXL_DEBUG 1
#	endif
#endif

#if (_AXL_DEBUG)
#	define AXL_DEBUG_STRING "Debug"
#	define AXL_DEBUG_SUFFIX " " AXL_DEBUG_STRING
#else
#	define AXL_DEBUG_STRING ""
#	define AXL_DEBUG_SUFFIX ""
#endif

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// 2) standard includes

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// C/C++ headers

#ifndef __STDC_LIMIT_MACROS
#	define __STDC_LIMIT_MACROS 1
#endif

#ifndef __STDC_CONSTANT_MACROS
#	define __STDC_CONSTANT_MACROS 1
#endif

#if (_AXL_OS_WIN)
#	ifndef _CRT_SECURE_NO_WARNINGS
#		define _CRT_SECURE_NO_WARNINGS 1 // useless warnings about "unsafe" string functions
#	endif

#	ifndef _SCL_SECURE_NO_WARNINGS
#		define _SCL_SECURE_NO_WARNINGS 1 // useless warnings about "unsafe" iterator operations
#	endif

#	ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#		define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#	endif
#endif

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include <errno.h>

#include <new>
#include <cstdlib>
#include <utility>
#include <algorithm>

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// OS-specific headers

#if (_AXL_OS_WIN)
#	ifndef _WIN32_WINNT
#		define _WIN32_WINNT 0x0600 // Windows Vista
#	endif

#	define WIN32_LEAN_AND_MEAN // prevent winsock.h vs winsock2.h conflict
#	define NOMINMAX            // prevent min/max macro conflicts

#	include <windows.h>
#	include <winsock2.h>
#	include <oleauto.h>
#	include <shellapi.h>
#	include <setupapi.h>
#	include <crtdbg.h>
#	include <intrin.h>
#elif (_AXL_OS_POSIX)
#	include <unistd.h>
#	include <assert.h>
#	include <pthread.h>
#	include <semaphore.h>
#	include <fcntl.h>
#	include <dlfcn.h>
#	include <limits.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <sys/mman.h>
#	include <sys/socket.h>
#	include <sys/ioctl.h>
#	include <netinet/in.h>
#	include <netinet/ip.h>
#	include <arpa/inet.h>
#	include <termios.h>
#	include <libgen.h>
#	include <dirent.h>

#	if (_AXL_OS_DARWIN)
#		include <machine/endian.h>
#		include <mach/mach_error.h>
#		include <mach/mach_init.h>
#		include <mach/task.h>
#		include <mach/semaphore.h>
#		include <sys/time.h>
#	else
#		include <byteswap.h>
#	endif
#endif

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// 3) possibly conflicting complementary definitions

// items here sort of complement standard C types/operators/functions
// so prefixing them with 'axl_' would look non-consistent

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// common type aliases

// stdint.h already defines:
//   int8_t
//   uint8_t
//   int16_t
//   uint16_t
//   int32_t
//   uint32_t
//   int64_t
//   uint64_t
//   intptr_t
//   uintptr_t

typedef int               bool_t;
typedef unsigned int      uint_t;
typedef unsigned char     uchar_t;
typedef unsigned short    ushort_t;
typedef unsigned long     ulong_t;

typedef uint8_t           byte_t;
typedef uint16_t          word_t;
typedef uint64_t          qword_t;

#if (_AXL_CPP_MSC)
typedef ulong_t           dword_t;
#else
typedef uint32_t          dword_t;
#endif

typedef void*             handle_t;
typedef char              utf8_t;

#if (WCHAR_MAX <= 0xffff)
typedef wchar_t           utf16_t;
typedef int32_t           utf32_t;
#else
typedef int16_t           utf16_t;
typedef wchar_t           utf32_t;
#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (_AXL_OS_POSIX)
#	undef basename
#	define _stricmp    strcasecmp
#	define _strnicmp   strncasecmp
#	define _wcsicmp    wcscasecmp
#	define _wcsnicmp   wcsncasecmp
#	define _snprintf   snprintf
#	define _snwprintf  swprintf
#	define _alloca     alloca
#	define _atoi64     atoll
#	define _strtoi64   strtoll
#	define _strtoui64  strtoull

#	define MAKEWORD(a, b) ((word_t)(((byte_t)((a) & 0xff)) | ((word_t)((byte_t)((b) & 0xff))) << 8))
#	define MAKELONG(a, b) ((int32_t)(((word_t)((a) & 0xffff)) | ((dword_t)((word_t)((b) & 0xffff))) << 16))
#	define LOWORD(l)      ((word_t)((l) & 0xffff))
#	define HIWORD(l)      ((word_t)(((l) >> 16) & 0xffff))
#	define LOBYTE(w)      ((byte_t)((w) & 0xff))
#	define HIBYTE(w)      ((byte_t)(((w) >> 8) & 0xff))
#	define RGB(r, g, b)   ((dword_t)(((byte_t)(r)|((word_t)((byte_t)(g)) << 8))|(((dword_t)(byte_t)(b)) << 16)))
#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// sizeof/countof/lengthof/offsetof/containerof

#ifndef countof
#	define countof(a) (sizeof(a) / sizeof((a) [0]))
#endif

#ifndef lengthof
#	define lengthof(s) (countof(s) - 1)
#endif

#ifndef containerof
#	define containerof(p, T, f) \
		((T*)((char*)(p) - offsetof(T, f)))
#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// NULL-checked strlen/wcslen

inline
size_t
strlen_s(const char* p) {
	return p ? strlen(p) : 0;
}

inline
size_t
wcslen_s(const wchar_t* p) {
	return p ? wcslen(p) : 0;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// 4) non-confliciting macro definitions

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// declaration options/attributes

#if (_AXL_CPP_MSC)
#	define AXL_CDECL       __cdecl
#	define AXL_STDCALL     __stdcall
#	define AXL_SELECT_ANY  __declspec(selectany)
#	define AXL_EXPORT      __declspec(dllexport)

#	define AXL_GCC_ALIGN(n)
#	define AXL_GCC_MSC_STRUCT
#	define AXL_GCC_NO_ASAN
#elif (_AXL_CPP_GCC)
#	if (_AXL_CPU_X86)
#		define AXL_CDECL   __attribute__((cdecl))
#		define AXL_STDCALL __attribute__((stdcall))
#	else
#		define AXL_CDECL
#		define AXL_STDCALL
#	endif
#	define AXL_SELECT_ANY  __attribute__((weak))
#	define AXL_EXPORT      __attribute__((visibility("default")))

#	define AXL_GCC_ALIGN(n) __attribute__((aligned(n)))

#	ifdef __has_attribute
#		if (__has_attribute(ms_struct))
#			define AXL_GCC_MSC_STRUCT __attribute__((ms_struct))
#		else
#			define AXL_GCC_MSC_STRUCT
#		endif
#   else
#		define AXL_GCC_MSC_STRUCT
#   endif

#	ifdef __has_feature
#		if (__has_feature(address_sanitizer))
#	 		define _AXL_GCC_ASAN 1
#		endif
#	elif (defined(__SANITIZE_ADDRESS__))
# 		define _AXL_GCC_ASAN 1
#	endif

#	define AXL_GCC_NO_ASAN __attribute__((no_sanitize_address))
#endif

#if (__cpp_noexcept_function_type)
#	define AXL_NOEXCEPT noexcept
#else
#	define AXL_NOEXCEPT throw()
#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// min/max are WAY too conflict-prone, so we use non-conflicting versions
// with 'AXL_' prefix

#define AXL_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define AXL_MAX(a, b) (((a) > (b)) ? (a) : (b))

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// ensure class is never accidentally copied

#define AXL_DISABLE_COPY(Class) \
private: \
	Class(const Class&);  \
	void \
	operator = (const Class&); \

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// ASSERT macro

#if (_AXL_OS_WIN)
#	define AXL_ASSERT _ASSERTE // from crtdbg.h
#else
#	define AXL_ASSERT assert   // from assert.h
#endif

#ifndef ASSERT
#	define ASSERT AXL_ASSERT
#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// TODO reminders

// usage:
//   AXL_TODO("implement new feature")

#define AXL_ITOA_IMPL(n) #n
#define AXL_ITOA(n)  AXL_ITOA_IMPL(n)

#if (_AXL_CPP_MSC)
#	define AXL_SRC_POS  __FILE__ "(" AXL_ITOA(__LINE__) ")"
#	define AXL_TODO(s) __pragma(message(AXL_SRC_POS ": TODO: " s))
#elif (_AXL_CPP_GCC)
#	if (_AXL_TODO)
#		define AXL_SRC_POS  __FILE__ ":" AXL_ITOA(__LINE__)
#		define AXL_PRAGMA_IMPL(p) _Pragma(#p)
#		define AXL_TODO(s) AXL_PRAGMA_IMPL(message "TODO: " s)
#	else
#		define AXL_TODO(s)
#	endif
#endif

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// 5) convenient vararg facilities

// this wrapper class serves the following purposes:
//   * makes sure va_list isn't get modified if passed as an argument;
//   * makes it possible to simply assign one to another.
//   * ensures va_end is called at the end

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class axl_va_list {
protected:
	va_list m_va;
	bool m_isInitialized;

public:
	axl_va_list() {
		m_isInitialized = false;
	}

	axl_va_list(const axl_va_list& src) {
		m_isInitialized = false;
		copy(src);
	}

	axl_va_list(va_list va) {
		m_isInitialized = false;
		copy(va);
	}

	~axl_va_list() {
		end();
	}

	axl_va_list&
	operator = (const axl_va_list& src) {
		copy(src);
		return *this;
	}

	axl_va_list&
	operator = (va_list va) {
		copy(va);
		return *this;
	}

	operator va_list& () {
		ASSERT(m_isInitialized);
		return m_va;
	}

	va_list*
	initialize() {
		m_isInitialized = true;
		return &m_va;
	}

	void
	end() {
		if (m_isInitialized) {
			va_end(m_va);
			m_isInitialized = false;
		}
	}

	void
	copy(const axl_va_list& src) {
		copy((va_list&) src.m_va);
	}

	void
	copy(va_list va) {
		end();

#ifdef va_copy
		va_copy(m_va, va);
#else
		m_va = va;
#endif
		m_isInitialized = true;
	}

	template <typename T>
	T
	arg() {
		ASSERT(m_isInitialized);
		return va_arg(m_va, T);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// use axl_va_start/axl_va_arg and (optionally) axl_va_end just like you would
// use va_start/va_arg/va_end...

#define axl_va_start(va, a) \
	va_start(*(va).initialize(), a)

#define axl_va_arg(va, T) \
	((va).arg<T> ())

#define axl_va_end(va) \
	((va).end())

// ...or use this single-line macro and then iterate via va.arg <T> ()

#define AXL_VA_DECL(va, a) \
	axl_va_list va; \
	axl_va_start(va, a);

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// 6) debugging facilities

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// TRACE macro (redirectable to a file)

#if (_AXL_DEBUG)
AXL_SELECT_ANY FILE* axl_g_traceFile = stdout;

#	if (_AXL_OS_WIN)
void
axl_trace_va(
	const char* formatString,
	axl_va_list va
);
#	else
inline
void
axl_trace_va(
	const char* formatString,
	axl_va_list va
) {
	vfprintf(axl_g_traceFile, formatString, va);
	fflush(axl_g_traceFile);
}
#	endif

inline
void
axl_trace(
	const char* formatString,
	...
) {
	AXL_VA_DECL(va, formatString);
	axl_trace_va(formatString, va);
}

#	define AXL_TRACE axl_trace
#else
#	define AXL_TRACE (void)
#endif

#ifndef TRACE
#	define TRACE AXL_TRACE
#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// break into the debugger

#if (_AXL_CPP_MSC)
#	define AXL_DEBUG_BREAK() (_CrtDbgBreak())
#elif (_AXL_CPP_GCC && (_AXL_CPU_X86 || _AXL_CPU_AMD64))
#	define AXL_DEBUG_BREAK() do { __asm__("int3"); } while (0)
#elif (_AXL_OS_POSIX)
#	include <signal.h>
#	define AXL_DEBUG_BREAK() (raise(SIGTRAP))
#else
#	error unsupported platform
#endif

//..............................................................................

// warnings suppression (GCC loses diagnostic pragmas defined in PCHs)

#if (_AXL_CPP_MSC)
#	pragma warning(disable: 4091) // warning C4091: 'typedef ': ignored on left of '...' when no variable is declared
#	pragma warning(disable: 4146) // warning C4146: unary minus operator applied to unsigned type, result still unsigned
#	pragma warning(disable: 4267) // warning C4267: 'var' : conversion from 'size_t' to 'type', possible loss of data
#	pragma warning(disable: 4355) // warning C4355: 'this' : used in base member initializer list
#	pragma warning(disable: 4477) // warning C4477: 'printf' : format string '...' requires an argument of type '...', but variadic argument ... has type '...'
#	pragma warning(disable: 4838) // warning C4838: conversion from 'int' to 'std::size_t' requires a narrowing conversion

#	if (_AXL_DEBUG)
		// many binary distributions of libs only come with Release build; static versions of those link to LIBCMT
		// thus producing the following warning when linked to your executables on Debug builds:

		// warning LNK4098: defaultlib 'LIBCMT' conflicts with use of other libs; use /NODEFAULTLIB:library
#		pragma comment(linker, "/NODEFAULTLIB:LIBCMT")
#	endif
#endif

//..............................................................................
