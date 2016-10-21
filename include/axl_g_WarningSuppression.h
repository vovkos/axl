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
#endif

#if (_AXL_CPP_GCC)

#	pragma GCC diagnostic ignored "-Wdeprecated-declarations"
//#	pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#	ifdef __cplusplus
#		pragma GCC diagnostic ignored "-Winvalid-offsetof"
#	endif
//#	pragma GCC diagnostic ignored "-Wmissing-braces"
#	pragma GCC diagnostic ignored "-Wmultichar"
#	pragma GCC diagnostic ignored "-Wnarrowing"
//#	pragma GCC diagnostic ignored "-Wparentheses"
//#	pragma GCC diagnostic ignored "-Wsign-compare"
//#	pragma GCC diagnostic ignored "-Wunused-parameter"
//#	pragma GCC diagnostic ignored "-Wunused-result"
//#	pragma GCC diagnostic ignored "-Wunused-variable"

#endif

#if (_AXL_CPP_CLANG)

#	pragma GCC diagnostic ignored "-Wdangling-else"
#	pragma GCC diagnostic ignored "-Wformat"
#	pragma GCC diagnostic ignored "-Wincompatible-ms-struct"
#	pragma GCC diagnostic ignored "-Wlogical-op-parentheses"
#	pragma GCC diagnostic ignored "-Wswitch"

#endif

//..............................................................................


/*

axl_create_compiler_flag_setting (
	GCC_FLAG_WARNING_UNUSED_RESULT
	DESCRIPTION "GNU C++ shows warnings on unused result"
	DEFAULT "-Wno-unused-result"
	 "-Wno-unused-result"
	)

axl_create_compiler_flag_setting (
	GCC_FLAG_WARNING_UNUSED_VARIABLE
	DESCRIPTION "GNU C++ shows warnings on unused variables"
	DEFAULT "-Wno-unused-variable"
	"" "-Wno-unused-variable"
	)

axl_create_compiler_flag_setting (
	GCC_FLAG_WARNING_SWITCH
	DESCRIPTION "GNU C++ shows warnings on missing enum member(s) in switch"
	DEFAULT "-Wno-switch"
	 "-Wno-switch"
	)

axl_create_compiler_flag_setting (
	GCC_FLAG_WARNING_MISSING_BRACES
	DESCRIPTION "GNU C++ shows warnings on missing braces in curly initializers"
	DEFAULT "-Wno-missing-braces"
	 "-Wno-missing-braces"
	)

axl_create_compiler_flag_setting (
	GCC_FLAG_WARNING_NARROWING
	DESCRIPTION "GNU C++ produces warnings/errors on narrowing conversions"
	 "-Wno-narrowing"
	)

axl_create_compiler_flag_setting (
	GCC_FLAG_INCOMPATIBLE_MS_STRUCT
	DESCRIPTION "GNU C++ shows warnings on potentially MSVC-incompatible structs"
	DEFAULT "-Wno-incompatible-ms-struct"
	 "-Wno-incompatible-ms-struct"
	)

*/
