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
#include "axl_sys_lnx_Udev.h"

namespace axl {
namespace sys {
namespace lnx {

//..............................................................................

bool
UdevContext::create()
{
	close();

	m_h = udev_new();
	return completeUdev(m_h != NULL, "udev_new");
}

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
