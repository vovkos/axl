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
#include "axl_iok_RegistryEntry.h"
#include "axl_sys_drw_MachError.h"

namespace axl {
namespace iok {

//..............................................................................

RegistryEntry
RegistryEntry::findConformingEntry(
	const char* className,
	const char* plane
) const {
	RegistryEntry entry = *this;
	while (entry) {
		if (entry.conformsTo(className))
			return entry;
		entry = entry.getParentEntry(plane);
	}

	return NULL;
}

//..............................................................................

} // namespace iok
} // namespace axl
