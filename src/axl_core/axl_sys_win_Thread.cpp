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

#include "pch.h"
#include "axl_sys_win_Thread.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
Thread::create (
	SECURITY_ATTRIBUTES* secAttr,
	size_t stackSize,
	PTHREAD_START_ROUTINE threadFunc,
	void* context,
	uint_t flags
	)
{
	close ();

	dword_t threadId;
	m_h = ::CreateThread (secAttr, stackSize, threadFunc, context, flags, &threadId);
	if (!m_h)
	{
		err::setLastSystemError ();
		return false;
	}

	m_threadId = threadId;
	return true;
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
