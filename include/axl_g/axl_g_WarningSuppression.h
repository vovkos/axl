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

// no pragma once -- this is to allow multiple inclusions of this file for re-enabling
// warning suppression (GCC loses warning suppression set up from precompiled headers)

#if (_AXL_CPP_MSC)
#	pragma warning (disable: 4146) // warning C4146: unary minus operator applied to unsigned type, result still unsigned
#	pragma warning (disable: 4267) // warning C4267: 'var' : conversion from 'size_t' to 'type', possible loss of data
#	pragma warning (disable: 4355) // warning C4355: 'this' : used in base member initializer list
#	pragma warning (disable: 4477) // warning C4477: 'printf' : format string '...' requires an argument of type '...', but variadic argument ... has type '...'
#	pragma warning (disable: 4838) // warning C4838: conversion from 'int' to 'std::size_t' requires a narrowing conversion

	// many binary distributions of libs only come with Release build; static versions of those link to LIBCMT 
	// thus producing the following warning when linked to your executables on Debug builds:

	// warning LNK4098: defaultlib 'LIBCMT' conflicts with use of other libs; use /NODEFAULTLIB:library
#	if (_AXL_DEBUG)
#		pragma comment (linker, "/NODEFAULTLIB:LIBCMT")	
#	endif
#endif

#if (_AXL_CPP_GCC)
#	pragma GCC diagnostic ignored "-Wdeprecated-declarations"
//#	pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#	ifdef __cplusplus
#		pragma GCC diagnostic ignored "-Winvalid-offsetof"
#	endif
//#	pragma GCC diagnostic ignored "-Wmissing-braces"
#	pragma GCC diagnostic ignored "-Wmultichar"
#	pragma GCC diagnostic ignored "-Wformat"
#	if (__cplusplus >= 201103L)
#		pragma GCC diagnostic ignored "-Wnarrowing"
#	endif
//#	pragma GCC diagnostic ignored "-Wparentheses"
//#	pragma GCC diagnostic ignored "-Wsign-compare"
//#	pragma GCC diagnostic ignored "-Wunused-parameter"
//#	pragma GCC diagnostic ignored "-Wunused-result"
//#	pragma GCC diagnostic ignored "-Wunused-variable"

#	if (_AXL_CPP_CLANG)
#		pragma GCC diagnostic ignored "-Wdangling-else"
#		pragma GCC diagnostic ignored "-Wincompatible-ms-struct"
#		pragma GCC diagnostic ignored "-Wlogical-op-parentheses"
#		pragma GCC diagnostic ignored "-Wswitch"
#	endif
#endif

//..............................................................................
