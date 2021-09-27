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
#include "axl_sys_drw_Semaphore.h"

namespace axl {
namespace sys {
namespace drw {

//..............................................................................

bool
Semaphore::wait(uint_t timeout) {
	mach_timespec_t tspec;
	tspec.tv_sec = timeout / 1000;
	tspec.tv_nsec = (timeout % 1000) * 1000000;

	mach_error_t result = ::semaphore_timedwait(m_semaphore, tspec);
	return result == 0 ? true : err::fail(sys::drw::MachError(result));
}

//..............................................................................

} // namespace drw
} // namespace sys
} // namespace axl
