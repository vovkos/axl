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

class RegistryEntry: public ObjectBase<io_registry_entry_t> {
public:
	RegistryEntry() {}

	RegistryEntry(const RegistryEntry& src) {
		copy(src);
	}

	RegistryEntry(
		io_registry_entry_t p,
		bool isAttach = false
	) {
		isAttach ? copy(p) : attach(p);
	}

	bool
	isInPlane(const char* plane) const {
		ASSERT(m_p);
		return ::IORegistryEntryInPlane(m_p, plane);
	}

	sl::String
	getName() const;

	sl::String
	getNameInPlane(const char* plane) const;

	sl::String
	getPath(const char* plane = kIOServicePlane) const;

	sl::String
	getLocationInPlane(const char* plane = kIOServicePlane) const;

	cf::MutableDictionary
	getAllProperties() const;

	cf::Type
	getProperty(const char* key) const {
		ASSERT(m_p);
		CFTypeRef p = ::IORegistryEntryCreateCFProperty(m_p, cf::String(key), kCFAllocatorDefault, 0);
		return cf::Type(p, true);
	}

	RegistryEntry
	getParentEntry(const char* plane = kIOServicePlane) const;

	RegistryEntry
	getChildEntry(const char* plane = kIOServicePlane) const;

	RegistryEntry
	findConformingEntry(
		const char* className,
		const char* plane = kIOServicePlane
	) const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
sl::String
RegistryEntry::getName() const {
	ASSERT(m_p);
	io_name_t buffer;
	buffer[0] = 0;
	::IORegistryEntryGetName(m_p, buffer);
	return buffer;
}

inline
sl::String
RegistryEntry::getNameInPlane(const char* plane) const {
	ASSERT(m_p);
	io_name_t buffer;
	buffer[0] = 0;
	::IORegistryEntryGetNameInPlane(m_p, plane, buffer);
	return buffer;
}

inline
sl::String
RegistryEntry::getPath(const char* plane) const {
	ASSERT(m_p);
	io_string_t buffer;
	buffer[0] = 0;
	::IORegistryEntryGetPath(m_p, plane, buffer);
	return buffer;
}

inline
sl::String
RegistryEntry::getLocationInPlane(const char* plane) const {
	ASSERT(m_p);
	io_string_t buffer;
	buffer[0] = 0;
	::IORegistryEntryGetLocationInPlane(m_p, plane, buffer);
	return buffer;
}

inline
cf::MutableDictionary
RegistryEntry::getAllProperties() const {
	ASSERT(m_p);
	CFMutableDictionaryRef p = NULL;
	::IORegistryEntryCreateCFProperties(m_p, &p, kCFAllocatorDefault, 0);
	return cf::MutableDictionary(p, true);
}

inline
RegistryEntry
RegistryEntry::getParentEntry(const char* plane) const {
	ASSERT(m_p);
	io_registry_entry_t p = MACH_PORT_NULL;
	::IORegistryEntryGetParentEntry(m_p, plane, &p);
	return RegistryEntry(p, true);
}

inline
RegistryEntry
RegistryEntry::getChildEntry(const char* plane) const {
	ASSERT(m_p);
	io_registry_entry_t p = MACH_PORT_NULL;
	::IORegistryEntryGetChildEntry(m_p, plane, &p);
	return RegistryEntry(p, true);
}

//..............................................................................

inline
cf::MutableDictionary
createServiceMatchingDictionary(const char* service) {
	CFMutableDictionaryRef p = ::IOServiceMatching(service);
	return cf::MutableDictionary(p, true);
}

inline
Iterator
findMatchingServices(CFMutableDictionaryRef dict) {
	io_iterator_t p = MACH_PORT_NULL;

	::CFRetain(dict);
	kern_return_t result = ::IOServiceGetMatchingServices(kIOMasterPortDefault, dict, &p);
	if (result != 0) {
		::CFRelease(dict);
		return Iterator();
	}

	return Iterator(p, true);
}

//..............................................................................

} // namespace iok
} // namespace axl
