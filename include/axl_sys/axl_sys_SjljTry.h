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

#define _AXL_SYS_SJLJTRY_H

#include "axl_sys_TlsSlot.h"

namespace axl {
namespace sys {

//..............................................................................

struct SjljFrame
{
	jmp_buf m_jmpBuf;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
sjljThrow()
{
	SjljFrame* sjljFrame = getTlsPtrSlotValue<SjljFrame> ();
	ASSERT(sjljFrame);
	longjmp(sjljFrame->m_jmpBuf, -1);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_SYS_BEGIN_SJLJ_TRY() \
{ \
	axl::sys::SjljFrame __axlSjljFrame; \
	axl::sys::SjljFrame* __axlSjljPrevFrame = axl::sys::setTlsPtrSlotValue<axl::sys::SjljFrame> (&__axlSjljFrame); \
	int __axlSjljBranch = setjmp(__axlSjljFrame.m_jmpBuf); \
	if (!__axlSjljBranch) \
	{

#define AXL_SYS_SJLJ_CATCH() \
	} \
	else \
	{ \
		{ \
			axl::sys::SjljFrame* prev = axl::sys::setTlsPtrSlotValue<axl::sys::SjljFrame> (__axlSjljPrevFrame); \
			ASSERT(prev == &__axlSjljFrame); \
		}

#define AXL_SYS_SJLJ_FINALLY() \
	} \
	{ \
		{ \
			axl::sys::SjljFrame* prev = axl::sys::setTlsPtrSlotValue<axl::sys::SjljFrame> (__axlSjljPrevFrame); \
			ASSERT(prev == &__axlSjljFrame || prev == __axlSjljPrevFrame); \
		}

#define AXL_SYS_END_SJLJ_TRY_IMPL() \
	} \
	{ \
		axl::sys::SjljFrame* prev = axl::sys::setTlsPtrSlotValue<axl::sys::SjljFrame> (__axlSjljPrevFrame); \
		ASSERT(prev == &__axlSjljFrame || prev == __axlSjljPrevFrame); \
	} \

#define AXL_SYS_END_SJLJ_TRY() \
	AXL_SYS_END_SJLJ_TRY_IMPL() \
}

#define AXL_SYS_END_SJLJ_TRY_EX(result) \
	AXL_SYS_END_SJLJ_TRY_IMPL() \
	*(result) = __axlSjljBranch == 0; \
}

#define AXL_SYS_SJLJ_THROW() \
	axl::sys::sjljThrow()

//..............................................................................

} // namespace sys
} // namespace axl
