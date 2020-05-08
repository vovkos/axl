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

#define _AXL_SYS_PSX_DYNAMICLIBRARY_H

#include "axl_sl_Handle.h"
#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace psx {

//..............................................................................

class CloseDynamicLib
{
public:
	void
	operator () (void* h)
	{
		::dlclose(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class DynamicLib: public sl::Handle<void*, CloseDynamicLib>
{
public:
	bool
	open(
		const sl::StringRef& name,
		int flags = RTLD_LAZY | RTLD_LOCAL
		);

	void*
	getSymbol(const sl::StringRef& name);

#if (!_AXL_OS_DARWIN)
	bool
	getInfo(
		int request,
		void* p
		);
#endif
};

//..............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
