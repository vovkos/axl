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

#define _AXL_IOK_REGISTRYENTRY_H

#include "axl_iok_Iterator.h"
#include "axl_cf_Dictionary.h"
#include "axl_cf_String.h"

namespace axl {
namespace iok {

//..............................................................................

class RegistryEntry: public ObjectBase<io_registry_entry_t>
{
public:
	RegistryEntry()
	{
	}

	RegistryEntry(const RegistryEntry& src)
	{
		copy(src);
	}

	RegistryEntry(
		io_registry_entry_t p,
		bool isAttach = false
		)
	{
		isAttach ? copy(p) : attach(p);
	}

	bool
	isInPlane(const char* plane) const
	{
		return ::IORegistryEntryInPlane(m_p, plane);
	}

	sl::String
	getName() const;

	sl::String
	getNameInPlane(const char* plane) const;

	sl::String
	getPath(const char* plane = kIOServicePlane) const;

	cf::MutableDictionaryRef
	getAllProperties() const;

	cf::TypeRef
	getProperty(const char* key) const
	{
		CFTypeRef p = ::IORegistryEntryCreateCFProperty(m_p, cf::StringRef(key), kCFAllocatorDefault, 0);
		return cf::TypeRef(p, true);
	}

	RegistryEntry
	getParentEntry(const char* plane = kIOServicePlane) const;

	RegistryEntry
	getChildEntry(const char* plane = kIOServicePlane) const;
};

//..............................................................................

inline
cf::MutableDictionaryRef
createServiceMatchingDictionary(const char* service)
{
	CFMutableDictionaryRef p = ::IOServiceMatching(service);
	return cf::MutableDictionaryRef(p, true);
}

Iterator
findMatchingServices(CFMutableDictionaryRef dict);

//..............................................................................

} // namespace iok
} // namespace axl
