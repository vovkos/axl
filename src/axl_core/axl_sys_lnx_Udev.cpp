#include "pch.h"
#include "axl_sys_lnx_Udev.h"

namespace axl {
namespace sys {
namespace lnx {

//..............................................................................

bool
UdevContext::create ()
{
	close ();

	m_h = udev_new ();
	return completeUdev (m_h != NULL, "udev_new");
}

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
