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

#define _AXL_SYS_WIN_SEMAPHORE_H

#include "axl_sys_win_WaitableHandle.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class Semaphore: public WaitableHandle
{
public:
	bool
	create(
		SECURITY_ATTRIBUTES* secAttr,
		uint_t initialCount,
		uint_t maxCount,
		const sl::StringRef_w& name = NULL
		)
	{
		close();

		m_h = ::CreateSemaphore(secAttr, initialCount, maxCount, name.sz());
		return err::complete(m_h != NULL);
	}

	bool
	open(
		uint_t access,
		bool doInheritHandle,
		const sl::StringRef_w& name
		)
	{
		close();

		m_h = ::OpenSemaphore(access, doInheritHandle, name.sz());
		return err::complete(m_h != NULL);
	}

	bool
	signal(uint_t count = 1)
	{
		bool_t result = ::ReleaseSemaphore(m_h, count, NULL);
		return err::complete(result);
	}
};

//..............................................................................

} // namespace axl
} // namespace sys
} // namespace win
