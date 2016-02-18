// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_LONGJMPTRY_H

#include "axl_sys_TlsSlot.h"

namespace axl {
namespace sys {

//.............................................................................

struct LongJmpTry
{
	jmp_buf m_jmpBuf;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_MT_BEGIN_LONG_JMP_TRY() \
{ \
	axl::sys::LongJmpTry __axlLongJmpTry; \
	axl::sys::LongJmpTry* __axlPrevLongJmpTry = axl::sys::setTlsSlotValue <axl::sys::LongJmpTry> (&__axlLongJmpTry); \
	int __axlLongJmpBranch = setjmp (__axlLongJmpTry.m_jmpBuf); \
	if (!__axlLongJmpBranch) \
	{

#define AXL_MT_LONG_JMP_CATCH() \
	} \
	else \
	{ \
		{ \
			axl::sys::LongJmpTry* prev = axl::sys::setTlsSlotValue <axl::sys::LongJmpTry> (__axlPrevLongJmpTry); \
			ASSERT (prev == &__axlLongJmpTry); \
		}

#define AXL_MT_LONG_JMP_FINALLY() \
	} \
	{

#define AXL_MT_END_LONG_JMP_TRY_IMPL() \
	} \
	{ \
		axl::sys::LongJmpTry* prev = axl::sys::setTlsSlotValue <axl::sys::LongJmpTry> (__axlPrevLongJmpTry); \
		ASSERT (prev == &__axlLongJmpTry || prev == __axlPrevLongJmpTry); \
	} \

#define AXL_MT_END_LONG_JMP_TRY() \
	AXL_MT_END_LONG_JMP_TRY_IMPL() \
}

#define AXL_MT_END_LONG_JMP_TRY_EX(result) \
	AXL_MT_END_LONG_JMP_TRY_IMPL() \
	*(result) = __axlLongJmpBranch == 0; \
}

#define AXL_MT_LONG_JMP_THROW() \
do \
{ \
	axl::sys::LongJmpTry* longJmpTry = axl::sys::getTlsSlotValue <axl::sys::LongJmpTry> (); \
	ASSERT (longJmpTry); \
	longjmp (longJmpTry->m_jmpBuf, -1); \
} while (0)

//.............................................................................

} // namespace sys
} // namespace axl
