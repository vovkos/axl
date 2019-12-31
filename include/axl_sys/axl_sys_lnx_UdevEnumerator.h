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

#define _AXL_SYS_LNX_UDEVENUMERATOR_H

#include "axl_sl_Handle.h"

namespace axl {
namespace sys {
namespace lnx {

//..............................................................................

template <typename T>
class UnrefUdevEnumerator
{
public:
	void
	operator () (udev_enumerate* h)
	{
		T::udev_enumerate_unref(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class UdevEnumeratorBase: public sl::Handle<udev_enumerate*, UnrefUdevEnumerator<T> >
{
public:
	UdevEnumeratorBase()
	{
	}

	UdevEnumeratorBase(udev_enumerate* h)
	{
		this->m_h = h;
	}

	void
	addRef()
	{
		T::udev_enumerate_ref(this->m_h);
	}

	udev*
	getUdev()
	{
		return T::udev_enumerate_get_udev(this->m_h);
	}

	bool
	addMatchSubsystem(const sl::StringRef& subsystem)
	{
		return T::udev_enumerate_add_match_subsystem_b(this->m_h, subsystem.sz());
	}

	bool
	addNoMatchSubsystem(const sl::StringRef& subsystem)
	{
		return T::udev_enumerate_add_nomatch_subsystem_b(this->m_h, subsystem.sz());
	}

	bool
	addMatchSysAttr(
		const sl::StringRef& sysAttr,
		const sl::StringRef& value
		)
	{
		return T::udev_enumerate_add_match_sysattr_b(this->m_h, sysAttr.sz(), value.sz());
	}

	bool
	addNoMatchSysAttr(
		const sl::StringRef& sysAttr,
		const sl::StringRef& value
		)
	{
		return T::udev_enumerate_add_match_sysattr_b(this->m_h, sysAttr.sz(), value.sz());
	}

	bool
	addMatchProperty(
		const sl::StringRef& prop,
		const sl::StringRef& value
		)
	{
		return T::udev_enumerate_add_match_property_b(this->m_h, prop.sz(), value.sz());
	}

	bool
	addMatchSysName(const sl::StringRef& sysName)
	{
		return T::udev_enumerate_add_match_sysname_b(this->m_h, sysName.sz());
	}

	bool
	addMatchTag(const sl::StringRef& tag)
	{
		return T::udev_enumerate_add_match_tag_b(this->m_h, tag.sz());
	}

	bool
	addMatchParentDevice(udev_device* parentDevice)
	{
		return T::udev_enumerate_add_match_parent_b(this->m_h, parentDevice);
	}

	bool
	addMatchIsInitialized()
	{
		return T::udev_enumerate_add_match_is_initialized_b(this->m_h);
	}

	bool
	scanDevices()
	{
		return T::udev_enumerate_scan_devices_b(this->m_h);
	}

	bool
	scanSubsystems()
	{
		return T::udev_enumerate_scan_subsystems_b(this->m_h);
	}

	bool
	addSysPath(const sl::StringRef& sysPath)
	{
		return T::udev_enumerate_add_syspath_b(this->m_h, sysPath.sz());
	}

	udev_list_entry*
	getListEntry()
	{
		return T::udev_enumerate_get_list_entry(this->m_h);
	}
};

//..............................................................................

template <typename T>
class UdevListEntryBase: public sl::Handle<udev_list_entry*>
{
public:
	UdevListEntryBase()
	{
	}

	UdevListEntryBase(udev_list_entry* h)
	{
		this->m_h = h;
	}

	UdevListEntryBase&
	operator ++ ()
	{
		next();
		return *this;
	}

	UdevListEntryBase
	operator ++ (int)
	{
		UdevListEntryBase old = *this;
		next();
		return old;
	}

	udev_list_entry*
	getNext()
	{
		return T::udev_list_entry_get_next(this->m_h);
	}

	bool
	next()
	{
		this->m_h = T::udev_list_entry_get_next(this->m_h);
		return this->m_h != NULL;
	}

	udev_list_entry*
	findByName(const char *name)
	{
		return T::udev_list_entry_get_by_name(this->m_h, name);
	}

	sl::StringRef
	getName()
	{
		return T::udev_list_entry_get_name(this->m_h);
	}

	sl::StringRef
	getValue()
	{
		return T::udev_list_entry_get_value(this->m_h);
	}
};

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
