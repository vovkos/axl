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

#define _AXL_SYS_LNX_UDEV_H

#include "axl_sl_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace lnx {

//..............................................................................

// unfortunately, udev does not set errno properly

template <typename T>
T
completeUdev(
	T result,
	const char* functionName
	)
{
	if (!result)
		err::setFormatStringError("'%s' failed", functionName);

	return result;
}

inline
bool
completeUdevErrno(int result)
{
	if (result != 0)
	{
		err::setError(-result);
		return false;
	}

	return true;
}

//..............................................................................

// handle closing

class UnrefUdevContext
{
public:
	void
	operator () (udev* h)
	{
		::udev_unref(h);
	}
};

class UnrefUdevDevice
{
public:
	void
	operator () (udev_device* h)
	{
		::udev_device_unref(h);
	}
};

class UnrefUdevEnumerator
{
public:
	void
	operator () (udev_enumerate* h)
	{
		::udev_enumerate_unref(h);
	}
};

class UnrefUdevMonitor
{
public:
	void
	operator () (udev_monitor* h)
	{
		::udev_monitor_unref(h);
	}
};

//..............................................................................

class UdevContext: public sl::Handle<udev*, UnrefUdevContext>
{
public:
	UdevContext()
	{
		create();
	}

	UdevContext(udev* h)
	{
		m_h = h;
	}

	bool
	create();

	void
	addRef()
	{
		::udev_ref(m_h);
	}

	udev_device*
	getDeviceFromSysPath(const sl::StringRef& sysPath)
	{
		udev_device* device = ::udev_device_new_from_syspath(m_h, sysPath.sz());
		return completeUdev(device, "udev_device_new_from_syspath");
	}

	udev_device*
	getDeviceFromDevNum(
		char type,
		dev_t devNum
		)
	{
		udev_device* device = ::udev_device_new_from_devnum(m_h, type, devNum);
		return completeUdev(device, "udev_device_new_from_devnum");
	}

	udev_device*
	getDeviceFromSubsytemSysName(
		const sl::StringRef& subsystem,
		const sl::StringRef& sysName
		)
	{
		udev_device* device = ::udev_device_new_from_subsystem_sysname(m_h, subsystem.sz(), sysName.sz());
		return completeUdev(device, "udev_device_new_from_subsystem_sysname");
	}

#if (_AXL_SYS_LNX_LIBUDEV_NEW_API)
	udev_device*
	getDeviceFromDeviceId(const sl::StringRef& deviceId)
	{
		udev_device* device = ::udev_device_new_from_device_id(m_h, (char*)deviceId.sz());
		return completeUdev(device, "udev_device_new_from_device_id");
	}
#endif

	udev_device*
	getDeviceFromEnvironment()
	{
		udev_device* device = ::udev_device_new_from_environment(m_h);
		return completeUdev(device, "udev_device_new_from_environment");
	}

	udev_enumerate*
	createEnumerator()
	{
		udev_enumerate* enumerator = ::udev_enumerate_new(m_h);
		return completeUdev(enumerator, "udev_device_new_from_environment");
	}

	udev_monitor*
	createMonitor(const sl::StringRef& name)
	{
		udev_monitor* monitor = ::udev_monitor_new_from_netlink(m_h, name.sz());
		return completeUdev(monitor, "udev_monitor_new_from_netlink");
	}
};

//..............................................................................

class UdevDevice: public sl::Handle<udev_device*, UnrefUdevDevice>
{
public:
	UdevDevice()
	{
	}

	UdevDevice(udev_device* h)
	{
		m_h = h;
	}

	void
	addRef()
	{
		::udev_device_ref(m_h);
	}

	udev*
	getUdev()
	{
		return ::udev_device_get_udev(m_h);
	}

	sl::StringRef
	getSysPath()
	{
		return ::udev_device_get_syspath(m_h);
	}

	sl::StringRef
	getSysName()
	{
		return ::udev_device_get_sysname(m_h);
	}

	sl::StringRef
	getSysNum()
	{
		return ::udev_device_get_sysnum(m_h);
	}

	sl::StringRef
	getDevPath()
	{
		return ::udev_device_get_devpath(m_h);
	}

	sl::StringRef
	getDevNode()
	{
		return ::udev_device_get_devnode(m_h);
	}

	dev_t
	getDevNum()
	{
		return ::udev_device_get_devnum(m_h);
	}

	sl::StringRef
	getDevType()
	{
		return ::udev_device_get_devtype(m_h);
	}

	sl::StringRef
	getSubsystem()
	{
		return ::udev_device_get_subsystem(m_h);
	}

	sl::StringRef
	getDriver()
	{
		return ::udev_device_get_driver(m_h);
	}

	udev_device*
	getParent()
	{
		return ::udev_device_get_parent(m_h);
	}

	udev_device*
	getParentWithSubsystemDevType(
		const sl::StringRef& subSystem,
		const sl::StringRef& devType
		)
	{
		return ::udev_device_get_parent_with_subsystem_devtype(m_h, subSystem.sz(), devType.sz());
	}

	bool
	isInitialized()
	{
		return ::udev_device_get_is_initialized(m_h) != 0;
	}

	sl::StringRef
	getAction()
	{
		return ::udev_device_get_action(m_h);
	}


	udev_list_entry*
	getDevLinkList()
	{
		return ::udev_device_get_devlinks_list_entry(m_h);
	}

	udev_list_entry*
	getPropertyList()
	{
		return ::udev_device_get_properties_list_entry(m_h);
	}

	udev_list_entry*
	getTagList()
	{
		return ::udev_device_get_tags_list_entry(m_h);
	}

	udev_list_entry*
	getSysAttrList()
	{
		return ::udev_device_get_sysattr_list_entry(m_h);
	}

	sl::StringRef
	getPropertyValue(const sl::StringRef& key)
	{
		return ::udev_device_get_property_value(m_h, key.sz());
	}

	bool
	hasTag(const sl::StringRef& tag)
	{
		return ::udev_device_has_tag(m_h, tag.sz());
	}

#if (_AXL_SYS_LNX_LIBUDEV_NEW_API)
	sl::StringRef
	getSysAttrValue(const sl::StringRef& sysAttr)
	{
		return ::udev_device_get_sysattr_value(m_h, sysAttr.sz());
	}

	int
	setSysAttrValue(
		const sl::StringRef& sysAttr,
		const sl::StringRef& value
		)
	{
		return ::udev_device_set_sysattr_value(m_h, sysAttr.sz(), (char*)value.sz());
	}
#endif
};

//..............................................................................

class UdevEnumerator: public sl::Handle<udev_enumerate*, UnrefUdevEnumerator>
{
public:
	UdevEnumerator()
	{
	}

	UdevEnumerator(udev_enumerate* h)
	{
		m_h = h;
	}

	void
	addRef()
	{
		::udev_enumerate_ref(m_h);
	}

	udev*
	getUdev()
	{
		return ::udev_enumerate_get_udev(m_h);
	}

	bool
	addMatchSubsystem(const sl::StringRef& subsystem)
	{
		int result = ::udev_enumerate_add_match_subsystem(m_h, subsystem.sz());
		return completeUdevErrno(result);
	}

	bool
	addNoMatchSubsystem(const sl::StringRef& subsystem)
	{
		int result = ::udev_enumerate_add_nomatch_subsystem(m_h, subsystem.sz());
		return completeUdevErrno(result);
	}

	bool
	addMatchSysAttr(
		const sl::StringRef& sysAttr,
		const sl::StringRef& value
		)
	{
		int result = ::udev_enumerate_add_match_sysattr(m_h, sysAttr.sz(), value.sz());
		return completeUdevErrno(result);
	}

	bool
	addNoMatchSysAttr(
		const sl::StringRef& sysAttr,
		const sl::StringRef& value
		)
	{
		int result = ::udev_enumerate_add_match_sysattr(m_h, sysAttr.sz(), value.sz());
		return completeUdevErrno(result);
	}

	bool
	addMatchProperty(
		const sl::StringRef& prop,
		const sl::StringRef& value
		)
	{
		int result = ::udev_enumerate_add_match_property(m_h, prop.sz(), value.sz());
		return completeUdevErrno(result);
	}

	bool
	addMatchSysName(const sl::StringRef& sysName)
	{
		int result = ::udev_enumerate_add_match_sysname(m_h, sysName.sz());
		return completeUdevErrno(result);
	}

	bool
	addMatchTag(const sl::StringRef& tag)
	{
		int result = ::udev_enumerate_add_match_tag(m_h, tag.sz());
		return completeUdevErrno(result);
	}

	bool
	addMatchParentDevice(udev_device* parentDevice)
	{
		int result = ::udev_enumerate_add_match_parent(m_h, parentDevice);
		return completeUdevErrno(result);
	}

	bool
	addMatchIsInitialized()
	{
		int result = ::udev_enumerate_add_match_is_initialized(m_h);
		return completeUdevErrno(result);
	}

	bool
	scanDevices()
	{
		int result = ::udev_enumerate_scan_devices(m_h);
		return completeUdevErrno(result);
	}

	bool
	scanSubsystems()
	{
		int result = ::udev_enumerate_scan_subsystems(m_h);
		return completeUdevErrno(result);
	}

	bool
	addSysPath(const sl::StringRef& sysPath)
	{
		int result = ::udev_enumerate_add_syspath(m_h, sysPath.sz());
		return completeUdevErrno(result);
	}

	udev_list_entry*
	getListEntry()
	{
		return ::udev_enumerate_get_list_entry(m_h);
	}
};

//..............................................................................

class UdevMonitor: public sl::Handle<udev_monitor*, UnrefUdevMonitor>
{
public:
	UdevMonitor()
	{
	}

	UdevMonitor(udev_monitor* h)
	{
		m_h = h;
	}

	void
	addRef()
	{
		::udev_monitor_ref(m_h);
	}

	udev*
	getUdev()
	{
		return ::udev_monitor_get_udev(m_h);
	}

	int
	getFd()
	{
		return ::udev_monitor_get_fd(m_h);
	}

	bool
	updateFilter()
	{
		int result = ::udev_monitor_filter_update(m_h);
		return completeUdevErrno(result);
	}

	bool
	removeFilter()
	{
		int result = ::udev_monitor_filter_remove(m_h);
		return completeUdevErrno(result);
	}

	bool
	addMatchSubsystemDevTypeFilter(
		const sl::StringRef& subsystem,
		const sl::StringRef& devType
		)
	{
		int result = ::udev_monitor_filter_add_match_subsystem_devtype(m_h, subsystem.sz(), devType.sz());
		return completeUdevErrno(result);
	}

	bool
	addMatchTagFilter(const char *tag)
	{
		int result = ::udev_monitor_filter_add_match_tag(m_h, tag);
		return completeUdevErrno(result);
	}

	bool
	setReceiveBufferSize(size_t size)
	{
		int result = ::udev_monitor_set_receive_buffer_size(m_h, size);
		return completeUdevErrno(result);
	}

	bool
	enableReceiving()
	{
		int result = ::udev_monitor_enable_receiving(m_h);
		return completeUdevErrno(result);
	}

	udev_device*
	receiveDevice()
	{
		udev_device* device = ::udev_monitor_receive_device(m_h);
		return completeUdev(device, "udev_monitor_receive_device ");
	}
};

//..............................................................................

class UdevListEntry: public sl::Handle<udev_list_entry*>
{
public:
	UdevListEntry()
	{
	}

	UdevListEntry(udev_list_entry* h)
	{
		m_h = h;
	}

	UdevListEntry&
	operator ++ ()
	{
		next();
		return *this;
	}

	UdevListEntry
	operator ++ (int)
	{
		UdevListEntry old = *this;
		next();
		return old;
	}

	udev_list_entry*
	getNext()
	{
		return ::udev_list_entry_get_next(m_h);
	}

	bool
	next()
	{
		m_h = ::udev_list_entry_get_next(m_h);
		return m_h != NULL;
	}

	udev_list_entry*
	findByName(const char *name)
	{
		return ::udev_list_entry_get_by_name(m_h, name);
	}

	sl::StringRef
	getName()
	{
		return ::udev_list_entry_get_name(m_h);
	}

	sl::StringRef
	getValue()
	{
		return ::udev_list_entry_get_value(m_h);
	}
};

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
