#include "pch.h"
#include "axl_iok_RegistryEntry.h"
#include "axl_sys_drw_MachError.h"

namespace axl {
namespace iok {

//..............................................................................

sl::String
RegistryEntry::getName () const
{
	io_name_t buffer = { 0 };
	::IORegistryEntryGetName (m_p, buffer);
	return buffer;
}

sl::String
RegistryEntry::getNameInPlane (const char* plane) const
{
	io_name_t buffer = { 0 };
	::IORegistryEntryGetNameInPlane (m_p, plane, buffer);
	return buffer;
}

sl::String
RegistryEntry::getPath (const char* plane) const
{
	io_string_t buffer = { 0 };
	::IORegistryEntryGetPath (m_p, plane, buffer);
	return buffer;
}

cf::MutableDictionaryRef
RegistryEntry::getAllProperties () const
{
	CFMutableDictionaryRef p = NULL;
	::IORegistryEntryCreateCFProperties (m_p, &p, kCFAllocatorDefault, 0);
	return cf::MutableDictionaryRef (p, true);
}

RegistryEntry
RegistryEntry::getParentEntry (const char* plane) const
{
	io_registry_entry_t p = MACH_PORT_NULL;
	::IORegistryEntryGetParentEntry (m_p, plane, &p);
	return RegistryEntry (p, true);
}

RegistryEntry
RegistryEntry::getChildEntry (const char* plane) const
{
	io_registry_entry_t p = MACH_PORT_NULL;
	::IORegistryEntryGetChildEntry (m_p, plane, &p);
	return RegistryEntry (p, true);
}

//..............................................................................

Iterator
findMatchingServices (CFMutableDictionaryRef dict)
{
	io_iterator_t p = MACH_PORT_NULL;

	::CFRetain (dict);
	kern_return_t result = ::IOServiceGetMatchingServices (kIOMasterPortDefault, dict, &p);
	if (result != 0)
	{
		::CFRelease (dict);
		return Iterator ();
	}

	return Iterator (p, true);
}

//..............................................................................

} // namespace iok
} // namespace axl
