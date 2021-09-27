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
#include "axl_sys_win_Service.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
Service::setDescription(const sl::StringRef_w& description) {
	SERVICE_DESCRIPTIONW info = { (WCHAR*)description.sz() };
	return changeServiceConfig2(SERVICE_CONFIG_DESCRIPTION, &info);
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
