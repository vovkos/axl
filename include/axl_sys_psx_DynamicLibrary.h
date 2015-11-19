// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_PSX_DYNAMICLIBRARY_H

#include "axl_sl_Handle.h"

namespace axl {
namespace sys {
namespace psx {

//.............................................................................

class CloseDynamicLibrary
{
public:
	void
	operator () (void* h)
	{
		::dlclose (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class DynamicLibrary: public sl::Handle <void*, CloseDynamicLibrary>
{
public:
	bool
	open (
		const char* name,
		int flags = RTLD_LAZY | RTLD_LOCAL
		);

	void*
	getSymbol (const char* name);
};

//.............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
