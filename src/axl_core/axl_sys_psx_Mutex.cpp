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
#include "axl_sys_psx_Mutex.h"
#include "axl_err_Error.h"
#include "axl_sys_Time.h"

namespace axl {
namespace sys {
namespace psx {

//..............................................................................

#if (!_AXL_OS_DARWIN)
bool
MutexAttr::getProcessShared(int* value) const
{
	int result = ::pthread_mutexattr_getpshared(&m_attr, value);
	return result == 0 ? true : err::fail(result);
}

bool
MutexAttr::setProcessShared(int value)
{
	int result = ::pthread_mutexattr_setpshared(&m_attr, value);
	return result == 0 ? true : err::fail(result);
}
#endif

bool
MutexAttr::getType(int* value) const
{
	int result = ::pthread_mutexattr_gettype(&m_attr, value);
	return result == 0 ? true : err::fail(result);
}

bool
MutexAttr::setType(int value)
{
	int result = ::pthread_mutexattr_settype(&m_attr, value);
	return result == 0 ? true : err::fail(result);
}

//..............................................................................

bool
Mutex::tryLock()
{
	int result = ::pthread_mutex_trylock(&m_mutex);
	return result == 0 ? true : err::fail(result);
}

#if (!_AXL_OS_DARWIN)
bool
Mutex::lock(uint_t timeout)
{
	int result;

	switch (timeout)
	{
	case 0:
		result = ::pthread_mutex_trylock(&m_mutex);
		break;

	case -1:
		result = ::pthread_mutex_lock(&m_mutex);
		break;

	default:
		timespec ts = { 0 };
		sys::getAbsTimespecFromTimeout(timeout, &ts);
		result = ::pthread_mutex_timedlock(&m_mutex, &ts);
	}

	return result == 0 ? true : err::fail(result);
}
#endif

//..............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
