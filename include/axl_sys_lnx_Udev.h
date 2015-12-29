// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_LNX_UDEV_H

#include "axl_sl_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace lnx {

//.............................................................................

// unfortunately, udev does not set errno properly

template <typename T>
T
completeUdev (
	T result,
	const char* functionName
	)
{
	if (!result)
		err::setFormatStringError ("'%s' failed", functionName);

	return result;
}

inline
bool
completeUdevErrno (int result)
{
	if (result != 0)
	{
		err::setError (-result);
		false;
	}

	return true;
}

//.............................................................................

// handle closing

class UnrefUdevContext
{
public:
	void
	operator () (udev* h)
	{
		::udev_unref (h);
	}
};

class UnrefUdevDevice
{
public:
	void
	operator () (udev_device* h)
	{
		::udev_device_unref (h);
	}
};

class UnrefUdevEnumerator
{
public:
	void
	operator () (udev_enumerate* h)
	{
		::udev_enumerate_unref (h);
	}
};

class UnrefUdevMonitor
{
public:
	void
	operator () (udev_monitor* h)
	{
		::udev_monitor_unref (h);
	}
};

//.............................................................................

class UdevContext: public sl::Handle <udev*, UnrefUdevContext>
{
public:
	UdevContext ()
	{
		create ();
	}

	UdevContext (udev* h)
	{
		m_h = h;
	}

	bool
	create ();

	void
	addRef ()
	{
		::udev_ref (m_h);
	}

	udev_device*
	getDeviceFromSysPath (const char* sysPath)
	{
		udev_device* device = ::udev_device_new_from_syspath (m_h, sysPath);
		return completeUdev (device, "udev_device_new_from_syspath");
	}

	udev_device*
	getDeviceFromDevNum (
		char type,
		dev_t devNum
		)
	{
		udev_device* device = ::udev_device_new_from_devnum (m_h, type, devNum);
		return completeUdev (device, "udev_device_new_from_devnum");
	}

	udev_device*
	getDeviceFromSubsytemSysName (
		const char* subsystem,
		const char* sysName
		)
	{
		udev_device* device = ::udev_device_new_from_subsystem_sysname (m_h, subsystem, sysName);
		return completeUdev (device, "udev_device_new_from_subsystem_sysname");
	}

	udev_device*
	getDeviceFromDeviceId (const char* deviceId)
	{
		udev_device* device = ::udev_device_new_from_device_id (m_h, (char*) deviceId);
		return completeUdev (device, "udev_device_new_from_device_id");
	}

	udev_device*
	getDeviceFromEnvironment ()
	{
		udev_device* device = ::udev_device_new_from_environment (m_h);
		return completeUdev (device, "udev_device_new_from_environment");
	}

	udev_enumerate*
	createEnumerator ()
	{
		udev_enumerate* enumerator = ::udev_enumerate_new (m_h);
		return completeUdev (enumerator, "udev_device_new_from_environment");
	}

	udev_monitor*
	createMonitor (const char* name)
	{
		udev_monitor* monitor = ::udev_monitor_new_from_netlink (m_h, name);
		return completeUdev (monitor, "udev_monitor_new_from_netlink");
	}
};

//.............................................................................

class UdevDevice: public sl::Handle <udev_device*, UnrefUdevDevice>
{
public:
	UdevDevice ()
	{
	}

	UdevDevice (udev_device* h)
	{
		m_h = h;
	}

	void
	addRef ()
	{
		::udev_device_ref (m_h);
	}

	udev*
	getUdev ()
	{
		return ::udev_device_get_udev (m_h);
	}

	const char*
	getSysPath ()
	{
		return ::udev_device_get_syspath (m_h);
	}

	const char*
	getSysName ()
	{
		return ::udev_device_get_sysname(m_h);
	}

	const char*
	getSysNum ()
	{
		return ::udev_device_get_sysnum (m_h);
	}

	const char*
	getDevPath ()
	{
		return ::udev_device_get_devpath (m_h);
	}

	const char*
	getDevNode ()
	{
		return ::udev_device_get_devnode (m_h);
	}

	dev_t
	getDevNum ()
	{
		return ::udev_device_get_devnum (m_h);
	}

	const char*
	getDevType ()
	{
		return ::udev_device_get_devtype (m_h);
	}

	const char*
	getSubsystem ()
	{
		return ::udev_device_get_subsystem (m_h);
	}

	const char*
	getDriver ()
	{
		return ::udev_device_get_driver (m_h);
	}

	udev_device*
	getParent ()
	{
		return ::udev_device_get_parent (m_h);
	}

	udev_device*
	getParentWithSubsystemDevType (
		const char* subSystem,
		const char* devType
		)
	{
		return ::udev_device_get_parent_with_subsystem_devtype (m_h, subSystem, devType);
	}

	bool
	isInitialized ()
	{
		return ::udev_device_get_is_initialized (m_h) != 0;
	}

	const char*
	getAction ()
	{
		return ::udev_device_get_action (m_h);
	}


	udev_list_entry*
	getDevLinkList ()
	{
		return ::udev_device_get_devlinks_list_entry (m_h);
	}

	udev_list_entry*
	getPropertyList ()
	{
		return ::udev_device_get_properties_list_entry (m_h);
	}

	udev_list_entry*
	getTagList ()
	{
		return ::udev_device_get_tags_list_entry (m_h);
	}

	udev_list_entry*
	getSysAttrList ()
	{
		return ::udev_device_get_sysattr_list_entry (m_h);
	}

	const char*
	getPropertyValue (const char* key)
	{
		return ::udev_device_get_property_value (m_h, key);
	}

	bool
	hasTag (const char* tag)
	{
		return ::udev_device_has_tag (m_h, tag);
	}

	const char*
	getSysAttrValue (const char* sysAttr)
	{
		return ::udev_device_get_sysattr_value (m_h, sysAttr);
	}

	int
	setSysAttrValue (
		const char* sysAttr,
		const char* value
		)
	{
		return ::udev_device_set_sysattr_value (m_h, sysAttr, (char*) value);
	}
};

//.............................................................................

class UdevEnumerator: public sl::Handle <udev_enumerate*, UnrefUdevEnumerator>
{
public:
	UdevEnumerator ()
	{
	}

	UdevEnumerator (udev_enumerate* h)
	{
		m_h = h;
	}

	void
	addRef ()
	{
		::udev_enumerate_ref (m_h);
	}

	udev*
	getUdev ()
	{
		return ::udev_enumerate_get_udev (m_h);
	}

	bool
	addMatchSubsystem (const char* subsystem)
	{
		int result = ::udev_enumerate_add_match_subsystem (m_h, subsystem);
		return completeUdevErrno (result);
	}

	bool
	addNoMatchSubsystem (const char* subsystem)
	{
		int result = ::udev_enumerate_add_nomatch_subsystem (m_h, subsystem);
		return completeUdevErrno (result);
	}

	bool
	addMatchSysAttr (
		const char* sysAttr,
		const char* value
		)
	{
		int result = ::udev_enumerate_add_match_sysattr (m_h, sysAttr, value);
		return completeUdevErrno (result);
	}

	bool
	addNoMatchSysAttr (
		const char* sysAttr,
		const char* value
		)
	{
		int result = ::udev_enumerate_add_match_sysattr (m_h, sysAttr, value);
		return completeUdevErrno (result);
	}

	bool
	addMatchProperty (
		const char* prop,
		const char* value
		)
	{
		int result = ::udev_enumerate_add_match_property (m_h, prop, value);
		return completeUdevErrno (result);
	}

	bool
	addMatchSysName (const char* sysName)
	{
		int result = ::udev_enumerate_add_match_sysname (m_h, sysName);
		return completeUdevErrno (result);
	}

	bool
	addMatchTag (const char* tag)
	{
		int result = ::udev_enumerate_add_match_tag (m_h,	tag);
		return completeUdevErrno (result);
	}

	bool
	addMatchParentDevice (udev_device* parentDevice)
	{
		int result = ::udev_enumerate_add_match_parent (m_h, parentDevice);
		return completeUdevErrno (result);
	}

	bool
	addMatchIsInitialized ()
	{
		int result = ::udev_enumerate_add_match_is_initialized (m_h);
		return completeUdevErrno (result);
	}

	bool
	scanDevices ()
	{
		int result = ::udev_enumerate_scan_devices (m_h);
		return completeUdevErrno (result);
	}

	bool
	scanSubsystems ()
	{
		int result = ::udev_enumerate_scan_subsystems (m_h);
		return completeUdevErrno (result);
	}

	bool
	addSysPath (const char* sysPath)
	{
		int result = ::udev_enumerate_add_syspath (m_h, sysPath);
		return completeUdevErrno (result);
	}

	udev_list_entry*
	getListEntry ()
	{
		return ::udev_enumerate_get_list_entry (m_h);
	}
};

//.............................................................................

class UdevMonitor: public sl::Handle <udev_monitor*, UnrefUdevMonitor>
{
public:
	UdevMonitor ()
	{
	}

	UdevMonitor (udev_monitor* h)
	{
		m_h = h;
	}

	void
	addRef ()
	{
		::udev_monitor_ref (m_h);
	}

	udev*
	getUdev ()
	{
		return ::udev_monitor_get_udev (m_h);
	}

	int
	getFd ()
	{
		return ::udev_monitor_get_fd (m_h);
	}

	bool
	updateFilter ()
	{
		int result = ::udev_monitor_filter_update (m_h);
		return completeUdevErrno (result);
	}

	bool
	removeFilter ()
	{
		int result = ::udev_monitor_filter_remove (m_h);
		return completeUdevErrno (result);
	}

	bool
	addMatchSubsystemDevTypeFilter (
		const char* subsystem,
		const char* devType
		)
	{
		int result = ::udev_monitor_filter_add_match_subsystem_devtype (m_h, subsystem, devType);
		return completeUdevErrno (result);
	}

	bool
	addMatchTagFilter (const char *tag)
	{
		int result = ::udev_monitor_filter_add_match_tag (m_h, tag);
		return completeUdevErrno (result);
	}

	bool
	setReceiveBufferSize (size_t size)
	{
		int result = ::udev_monitor_set_receive_buffer_size (m_h, size);
		return completeUdevErrno (result);
	}

	bool
	enableReceiving ()
	{
		int result = ::udev_monitor_enable_receiving (m_h);
		return completeUdevErrno (result);
	}

	udev_device*
	receiveDevice ()
	{
		udev_device* device = ::udev_monitor_receive_device (m_h);
		return completeUdev (device, "udev_monitor_receive_device ");
	}
};

//.............................................................................

class UdevListEntry: public sl::Handle <udev_list_entry*>
{
public:
	UdevListEntry ()
	{
	}

	UdevListEntry (udev_list_entry* h)
	{
		m_h = h;
	}

	UdevListEntry&
	operator ++ ()
	{
		next ();
		return *this;
	}

	UdevListEntry
	operator ++ (int)
	{
		UdevListEntry old = *this;
		next ();
		return old;
	}

	udev_list_entry*
	getNext ()
	{
		return ::udev_list_entry_get_next (m_h);
	}

	bool
	next ()
	{
		m_h = ::udev_list_entry_get_next (m_h);
		return m_h != NULL;
	}

	udev_list_entry*
	findByName (const char *name)
	{
		return ::udev_list_entry_get_by_name (m_h, name);
	}

	const char*
	getName ()
	{
		return ::udev_list_entry_get_name (m_h);
	}

	const char*
	getValue ()
	{
		return ::udev_list_entry_get_value (m_h);
	}
};

//.............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
