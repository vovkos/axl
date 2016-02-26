// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_SJLJTRY_H

#include "axl_sys_TlsSlot.h"

namespace axl {
namespace sys {

//.............................................................................

struct SjljTry
{
	SjljTry* m_prev;
	jmp_buf m_jmpBuf;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SYS_BEGIN_SJLJ_TRY() \
{ \
	axl::sys::SjljTry __axlSjljTry; \
	__axlSjljTry.m_prev = axl::sys::setTlsSlotValue <axl::sys::SjljTry> (&__axlSjljTry); \
	int __axlSjljBranch = setjmp (__axlSjljTry.m_jmpBuf); \
	if (!__axlSjljBranch) \
	{

#define AXL_SYS_SJLJ_CATCH() \
	} \
	else \
	{ \
		{ \
			axl::sys::SjljTry* prev = axl::sys::setTlsSlotValue <axl::sys::SjljTry> (__axlSjljTry.m_prev); \
			ASSERT (prev == &__axlSjljTry); \
		}

#define AXL_SYS_SJLJ_FINALLY() \
	} \
	{

#define AXL_SYS_END_SJLJ_TRY_IMPL() \
	} \
	{ \
		axl::sys::SjljTry* prev = axl::sys::setTlsSlotValue <axl::sys::SjljTry> (__axlSjljTry.m_prev); \
		ASSERT (prev == &__axlSjljTry || prev == __axlSjljTry.m_prev); \
	} \

#define AXL_SYS_END_SJLJ_TRY() \
	AXL_SYS_END_SJLJ_TRY_IMPL() \
}

#define AXL_SYS_END_SJLJ_TRY_EX(result) \
	AXL_SYS_END_SJLJ_TRY_IMPL() \
	*(result) = __axlSjljBranch == 0; \
}

#define AXL_SYS_SJLJ_THROW() \
do \
{ \
	axl::sys::SjljTry* sjljTry = axl::sys::getTlsSlotValue <axl::sys::SjljTry> (); \
	ASSERT (sjljTry); \
	longjmp (sjljTry->m_jmpBuf, -1); \
} while (0)

//.............................................................................

} // namespace sys
} // namespace axl
