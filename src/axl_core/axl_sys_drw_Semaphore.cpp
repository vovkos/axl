#include "pch.h"
#include "axl_sys_drw_Semaphore.h"

namespace axl {
namespace sys {
namespace drw {

//..............................................................................

bool
Semaphore::wait (uint_t timeout)
{
	mach_timespec_t tspec;
	tspec.tv_sec = timeout / 1000;
	tspec.tv_nsec = (timeout % 1000) * 1000000;

	mach_error_t result = ::semaphore_timedwait (m_semaphore, tspec);
	return result == 0 ? true : err::fail (sys::drw::MachError (result));
}

//..............................................................................

} // namespace drw
} // namespace sys
} // namespace axl
