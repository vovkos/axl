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
#include "axl_sys_psx_DynamicLib.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace psx {

//..............................................................................

bool
DynamicLib::open(
	const sl::StringRef& name,
	int flags
) {
	close();

	m_h = ::dlopen(name.sz(), flags);
	if (!m_h)
		return err::fail(::dlerror());

	return true;
}

void*
DynamicLib::getSymbol(const sl::StringRef& name) {
	void* p = ::dlsym(m_h, name.sz());
	if (!p)
		return err::fail<void*>(NULL, ::dlerror());

	return p;
}

#if (!_AXL_OS_DARWIN)

bool
DynamicLib::getInfo(
	int request,
	void* p
) {
	int result = ::dlinfo(m_h, request, p);
	if (result == -1)
		return err::fail(::dlerror());

	return true;
}

#endif

//..............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
