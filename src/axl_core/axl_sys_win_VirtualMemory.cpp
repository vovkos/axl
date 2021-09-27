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
#include "axl_sys_win_VirtualMemory.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

void*
VirtualMemory::alloc(
	void* p0,
	size_t size,
	uint_t allocationFlags,
	uint_t protectionFlags
) {
	release();

	void* p = ::VirtualAlloc(p0, size, allocationFlags, protectionFlags);
	if (!p) {
		err::setLastSystemError();
		return NULL;
	}

	m_p = p;
	m_size = size;
	return p;
}

void
VirtualMemory::release() {
	if (!m_p)
		return;

	::VirtualFree(m_p, m_size, MEM_RELEASE);
	m_p = NULL;
	m_size = 0;
}

bool
VirtualMemory::protect(
	uint_t protectionFlags,
	uint_t* prevProtectionFlags0
) {
	dword_t prevProtectionFlags;
	bool_t result = ::VirtualProtect(m_p, m_size, protectionFlags, &prevProtectionFlags);
	if (!result) {
		err::setLastSystemError();
		return false;
	}

	if (prevProtectionFlags0)
		*prevProtectionFlags0 = prevProtectionFlags;

	return true;
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
