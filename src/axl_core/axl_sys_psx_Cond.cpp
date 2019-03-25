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
#include "axl_sys_psx_Cond.h"
#include "axl_sys_Time.h"
#include "axl_sys_Event.h"

namespace axl {
namespace sys {
namespace psx {

//..............................................................................

bool
Cond::wait(
	pthread_mutex_t* mutex,
	uint_t timeout
	)
{
	int result;

	switch (timeout)
	{
	case -1:
		result = ::pthread_cond_wait(&m_cond, mutex);
		break;

	default:
		timespec timespec = { 0 };
		sys::getAbsTimespecFromTimeout(timeout, &timespec);
		result = ::pthread_cond_timedwait(&m_cond, mutex, &timespec);
	}

	return result == 0 ? true : err::fail(result);
}

//..............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
