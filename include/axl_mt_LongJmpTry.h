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

//.............................................................................

#define AXL_MT_BEGIN_LONG_JMP_TRY() \
{ \
	axl::mt::LongJmpTry longJmpTry; \
	axl::mt::LongJmpTry* prevLongJmpTry = axl::mt::setTlsSlotValue <axl::mt::LongJmpTry> (&longJmpTry); \
	int branch = setjmp (longJmpTry.m_jmpBuf); \
	if (!branch) \
	{

#define AXL_MT_LONG_JMP_CATCH() \
		axl::mt::setTlsSlotValue <axl::mt::LongJmpTry> (prevLongJmpTry); \
	} \
	else \
	{ \
		axl::mt::setTlsSlotValue <axl::mt::LongJmpTry> (prevLongJmpTry);

#define AXL_MT_END_LONG_JMP_TRY() \
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
