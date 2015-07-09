// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_TLSSLOT_H

#include "axl_mt_TlsSlot.h"

namespace axl {
namespace mt {

//.............................................................................

struct LongJmpTry
{
	jmp_buf m_jmpBuf;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#define AXL_MT_BEGIN_LONG_JMP_TRY() \
{ \
	axl::mt::LongJmpTry __axlLongJmpTry; \
	axl::mt::LongJmpTry* __axlPrevLongJmpTry = axl::mt::setTlsSlotValue <axl::mt::LongJmpTry> (&__axlLongJmpTry); \
	int branch = setjmp (__axlLongJmpTry.m_jmpBuf); \
	if (!branch) \
	{

#define AXL_MT_LONG_JMP_CATCH() \
	} \
	else \
	{ \
		{ \
			axl::mt::LongJmpTry* prev = axl::mt::setTlsSlotValue <axl::mt::LongJmpTry> (__axlPrevLongJmpTry); \
			ASSERT (prev == &__axlLongJmpTry); \
		}

#define AXL_MT_END_LONG_JMP_TRY() \
	} \
	{ \
		axl::mt::LongJmpTry* prev = axl::mt::setTlsSlotValue <axl::mt::LongJmpTry> (__axlPrevLongJmpTry); \
		ASSERT (prev == &__axlLongJmpTry || prev == __axlPrevLongJmpTry); \
	} \
}

#define AXL_MT_LONG_JMP_THROW() \
do \
{ \
	axl::mt::LongJmpTry* longJmpTry = axl::mt::getTlsSlotValue <axl::mt::LongJmpTry> (); \
	ASSERT (longJmpTry); \
	longjmp (longJmpTry->m_jmpBuf, -1); \
} while (0)

//.............................................................................

} // namespace mt {
} // namespace axl {
