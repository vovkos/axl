
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

#include "axl_sys_lnx_UdevContext.h"
#include "axl_sys_lnx_UdevDevice.h"
#include "axl_sys_lnx_UdevEnumerator.h"
#include "axl_sys_lnx_UdevMonitor.h"
#include "axl_sl_Singleton.h"
#include "axl_sys_DynamicLib.h"
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

class Udev
{
public:
	// context API

	static
	struct udev*
	udev_ref(struct udev* context)
	{
		return ::udev_ref(context);
	}

	static
	struct udev*
	udev_unref(struct udev* context)
	{
		return ::udev_unref(context);
	}

	static
	struct udev*
	udev_new()
	{
		udev* context = ::udev_new();
		return completeUdev(context, "udev_new");
	}

	static
	struct udev_device*
	udev_device_new_from_syspath(
		struct udev* context,
		const char* syspath
		)
	{
		udev_device* device = ::udev_device_new_from_syspath(context, syspath);
		return completeUdev(device, "udev_device_new_from_syspath");
	}

	static
	struct udev_device*
	udev_device_new_from_devnum(
		struct udev* context,
		char type,
		dev_t devnum
		)
	{
		udev_device* device = ::udev_device_new_from_devnum(context, type, devnum);
		return completeUdev(device, "udev_device_new_from_devnum");
	}

	static
	struct udev_device*
	udev_device_new_from_subsystem_sysname(
		struct udev* context,
		const char* subsystem,
		const char* sysname
		)
	{
		udev_device* device = ::udev_device_new_from_subsystem_sysname(context, subsystem, sysname);
		return completeUdev(device, "udev_device_new_from_subsystem_sysname");
	}

#if (_AXL_SYS_LNX_LIBUDEV_NEW_API)
	static
	struct udev_device*
	udev_device_new_from_device_id(
		struct udev* context,
		const char* id
		)
	{
		udev_device* device = ::udev_device_new_from_device_id(context, id);
		return completeUdev(device, "udev_device_new_from_device_id");
	}
#endif

	static
	struct udev_device*
	udev_device_new_from_environment(struct udev* context)
	{
		udev_device* device = ::udev_device_new_from_environment(context);
		return completeUdev(device, "udev_device_new_from_environment");
	}

	static
	struct udev_enumerate*
	udev_enumerate_new(struct udev* context)
	{
		udev_enumerate* enumerator = ::udev_enumerate_new(context);
		return completeUdev(enumerator, "udev_device_new_from_environment");
	}

	static
	struct udev_monitor*
	udev_monitor_new_from_netlink(
		struct udev* context,
		const char* name
		)
	{
		udev_monitor* monitor = ::udev_monitor_new_from_netlink(context, name);
		return completeUdev(monitor, "udev_monitor_new_from_netlink");
	}

	// device API

	static
	struct udev_device*
	udev_device_ref(struct udev_device* device)
	{
		return ::udev_device_ref(device);
	}

	static
	struct udev_device*
	udev_device_unref(struct udev_device* device)
	{
		return ::udev_device_unref(device);
	}

	static
	struct udev*
	udev_device_get_udev(struct udev_device* device)
	{
		return ::udev_device_get_udev(device);
	}

	static
	const char*
	udev_device_get_syspath(struct udev_device* device)
	{
		return ::udev_device_get_syspath(device);
	}

	static
	const char*
	udev_device_get_sysname(struct udev_device* device)
	{
		return ::udev_device_get_sysname(device);
	}

	static
	const char*
	udev_device_get_sysnum(struct udev_device* device)
	{
		return ::udev_device_get_sysnum(device);
	}

	static
	const char*
	udev_device_get_devpath(struct udev_device* device)
	{
		return ::udev_device_get_devpath(device);
	}

	static
	const char*
	udev_device_get_devnode(struct udev_device* device)
	{
		return ::udev_device_get_devnode(device);
	}

	static
	dev_t
	udev_device_get_devnum(struct udev_device* device)
	{
		return ::udev_device_get_devnum(device);
	}

	static
	const char*
	udev_device_get_devtype(struct udev_device* device)
	{
		return ::udev_device_get_devtype(device);
	}

	static
	const char*
	udev_device_get_subsystem(struct udev_device* device)
	{
		return ::udev_device_get_subsystem(device);
	}

	static
	const char*
	udev_device_get_driver(struct udev_device* device)
	{
		return ::udev_device_get_driver(device);
	}

	static
	struct udev_device*
	udev_device_get_parent(struct udev_device* device)
	{
		return ::udev_device_get_parent(device);
	}

	static
	struct udev_device*
	udev_device_get_parent_with_subsystem_devtype(
		struct udev_device* device,
		const char* subsystem,
		const char* devtype
		)
	{
		return ::udev_device_get_parent_with_subsystem_devtype(device, subsystem, devtype);
	}

	static
	bool
	udev_device_get_is_initialized(struct udev_device* device)
	{
		return ::udev_device_get_is_initialized(device) != 0;
	}

	static
	const char*
	udev_device_get_action(struct udev_device* device)
	{
		return ::udev_device_get_action(device);
	}

	static
	struct udev_list_entry*
	udev_device_get_devlinks_list_entry(struct udev_device* device)
	{
		return ::udev_device_get_devlinks_list_entry(device);
	}

	static
	struct udev_list_entry*
	udev_device_get_properties_list_entry(struct udev_device* device)
	{
		return ::udev_device_get_properties_list_entry(device);
	}

	static
	struct udev_list_entry*
	udev_device_get_tags_list_entry(struct udev_device* device)
	{
		return ::udev_device_get_tags_list_entry(device);
	}

	static
	struct udev_list_entry*
	udev_device_get_sysattr_list_entry(struct udev_device* device)
	{
		return ::udev_device_get_sysattr_list_entry(device);
	}

	static
	const char*
	udev_device_get_property_value(
		struct udev_device* device,
		const char* key
		)
	{
		return ::udev_device_get_property_value(device, key);
	}

	static
	int
	udev_device_has_tag(
		struct udev_device* device,
		const char* tag
		)
	{
		return ::udev_device_has_tag(device, tag);
	}

#if (_AXL_SYS_LNX_LIBUDEV_NEW_API)
	static
	const char*
	udev_device_get_sysattr_value(
		struct udev_device* device,
		const char* sysattr
		)
	{
		return ::udev_device_get_sysattr_value(device, sysattr);
	}

	static
	int
	udev_device_set_sysattr_value(
		struct udev_device* device,
		const char* sysattr,
		const char* value
		)
	{
		return ::udev_device_set_sysattr_value(device, sysattr, value);
	}
#endif

	// enumerator API

	static
	struct udev_enumerate*
	udev_enumerate_ref(struct udev_enumerate* enumerate)
	{
		return ::udev_enumerate_ref(enumerate);
	}

	static
	struct udev_enumerate*
	udev_enumerate_unref(struct udev_enumerate* enumerate)
	{
		return ::udev_enumerate_unref(enumerate);
	}

	static
	struct udev*
	udev_enumerate_get_udev(struct udev_enumerate* enumerate)
	{
		return ::udev_enumerate_get_udev(enumerate);
	}

	static
	bool
	udev_enumerate_add_match_subsystem_b(
		struct udev_enumerate* enumerate,
		const char* subsystem
		)
	{
		int result = ::udev_enumerate_add_match_subsystem(enumerate, subsystem);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_nomatch_subsystem_b(
		struct udev_enumerate* enumerate,
		const char* subsystem
		)
	{
		int result = ::udev_enumerate_add_nomatch_subsystem(enumerate, subsystem);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_sysattr_b(
		struct udev_enumerate* enumerate,
		const char* sysattr,
		const char* value
		)
	{
		int result = ::udev_enumerate_add_match_sysattr(enumerate, sysattr, value);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_nomatch_sysattr_b(
		struct udev_enumerate* enumerate,
		const char* sysattr,
		const char* value
		)
	{
		int result = ::udev_enumerate_add_nomatch_sysattr(enumerate, sysattr, value);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_property_b(
		struct udev_enumerate* enumerate,
		const char* property,
		const char* value
		)
	{
		int result = ::udev_enumerate_add_match_property(enumerate, property, value);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_sysname_b(
		struct udev_enumerate* enumerate,
		const char* sysname
		)
	{
		int result = ::udev_enumerate_add_match_sysname(enumerate, sysname);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_tag_b(
		struct udev_enumerate* enumerate,
		const char* tag
		)
	{
		int result = ::udev_enumerate_add_match_tag(enumerate, tag);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_parent_b(
		struct udev_enumerate* enumerate,
		struct udev_device* parent
		)
	{
		int result = ::udev_enumerate_add_match_parent(enumerate, parent);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_is_initialized_b(struct udev_enumerate* enumerate)
	{
		return ::udev_enumerate_add_match_is_initialized(enumerate) != 0;
	}

	static
	bool
	udev_enumerate_scan_devices_b(struct udev_enumerate* enumerate)
	{
		int result = ::udev_enumerate_scan_devices(enumerate);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_scan_subsystems_b(struct udev_enumerate* enumerate)
	{
		int result = ::udev_enumerate_scan_subsystems(enumerate);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_syspath_b(
		struct udev_enumerate* enumerate,
		const char* syspath
		)
	{
		int result = ::udev_enumerate_add_syspath(enumerate, syspath);
		return completeUdevErrno(result);
	}

	static
	struct udev_list_entry*
	udev_enumerate_get_list_entry(struct udev_enumerate* enumerate)
	{
		return ::udev_enumerate_get_list_entry(enumerate);
	}

	// list entry API

	static
	struct udev_list_entry*
	udev_list_entry_get_next(struct udev_list_entry* entry)
	{
		return ::udev_list_entry_get_next(entry);
	}

	static
	struct udev_list_entry*
	udev_list_entry_get_by_name(
		struct udev_list_entry* entry,
		const char* name
		)
	{
		return ::udev_list_entry_get_by_name(entry, name);
	}

	static
	const char*
	udev_list_entry_get_name(struct udev_list_entry* entry)
	{
		return ::udev_list_entry_get_name(entry);
	}

	static
	const char*
	udev_list_entry_get_value(struct udev_list_entry* entry)
	{
		return ::udev_list_entry_get_value(entry);
	}

	// monitor API

	static
	struct udev_monitor*
	udev_monitor_ref(struct udev_monitor* monitor)
	{
		return ::udev_monitor_ref(monitor);
	}

	static
	struct udev_monitor*
	udev_monitor_unref(struct udev_monitor* monitor)
	{
		return ::udev_monitor_unref(monitor);
	}

	static
	struct udev*
	udev_monitor_get_udev(struct udev_monitor* monitor)
	{
		return ::udev_monitor_get_udev(monitor);
	}

	static
	int
	udev_monitor_get_fd(struct udev_monitor* monitor)
	{
		return ::udev_monitor_get_fd(monitor);
	}

	static
	bool
	udev_monitor_filter_update_b(struct udev_monitor* monitor)
	{
		int result = ::udev_monitor_filter_update(monitor);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_filter_remove_b(struct udev_monitor* monitor)
	{
		int result = ::udev_monitor_filter_remove(monitor);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_filter_add_match_subsystem_devtype_b(
		struct udev_monitor* monitor,
		const char* subsystem,
		const char* devtype
		)
	{
		int result = ::udev_monitor_filter_add_match_subsystem_devtype(monitor, subsystem, devtype);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_filter_add_match_tag_b(
		struct udev_monitor* monitor,
		const char* tag
		)
	{
		int result = ::udev_monitor_filter_add_match_tag(monitor, tag);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_set_receive_buffer_size_b(
		struct udev_monitor* monitor,
		int size
		)
	{
		int result = ::udev_monitor_set_receive_buffer_size(monitor, size);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_enable_receiving_b(struct udev_monitor* monitor)
	{
		int result = ::udev_monitor_enable_receiving(monitor);
		return completeUdevErrno(result);
	}

	static
	struct udev_device*
	udev_monitor_receive_device(struct udev_monitor* monitor)
	{
		udev_device* device = ::udev_monitor_receive_device(monitor);
		return completeUdev(device, "udev_monitor_receive_device ");
	}
};

//..............................................................................

class UdevLib: public DynamicLib
{
protected:
	typedef
	struct udev*
	udev_ref_func(struct udev* context);

	typedef
	struct udev*
	udev_unref_func(struct udev* context);

	typedef
	struct udev*
	udev_new_func();

	typedef
	struct udev_device*
	udev_device_new_from_syspath_func(
		struct udev* context,
		const char* syspath
		);

	typedef
	struct udev_device*
	udev_device_new_from_devnum_func(
		struct udev* context,
		char type,
		dev_t devnum
		);

	typedef
	struct udev_device*
	udev_device_new_from_subsystem_sysname_func(
		struct udev* context,
		const char* subsystem,
		const char* sysname
		);

	typedef
	struct udev_device*
	udev_device_new_from_device_id_func(
		struct udev* context,
		const char* id
		);

	typedef
	struct udev_device*
	udev_device_new_from_environment_func(struct udev* context);

	typedef
	struct udev_enumerate*
	udev_enumerate_new_func(struct udev* context);

	typedef
	struct udev_monitor*
	udev_monitor_new_from_netlink_func(
		struct udev* context,
		const char* name
		);

	typedef
	struct udev_device*
	udev_device_ref_func(struct udev_device* device);

	typedef
	struct udev_device*
	udev_device_unref_func(struct udev_device* device);

	typedef
	struct udev*
	udev_device_get_udev_func(struct udev_device* device);

	typedef
	const char*
	udev_device_get_syspath_func(struct udev_device* device);

	typedef
	const char*
	udev_device_get_sysname_func(struct udev_device* device);

	typedef
	const char*
	udev_device_get_sysnum_func(struct udev_device* device);

	typedef
	const char*
	udev_device_get_devpath_func(struct udev_device* device);

	typedef
	const char*
	udev_device_get_devnode_func(struct udev_device* device);

	typedef
	dev_t
	udev_device_get_devnum_func(struct udev_device* device);

	typedef
	const char*
	udev_device_get_devtype_func(struct udev_device* device);

	typedef
	const char*
	udev_device_get_subsystem_func(struct udev_device* device);

	typedef
	const char*
	udev_device_get_driver_func(struct udev_device* device);

	typedef
	struct udev_device*
	udev_device_get_parent_func(struct udev_device* device);

	typedef
	struct udev_device*
	udev_device_get_parent_with_subsystem_devtype_func(
		struct udev_device* device,
		const char* subsystem,
		const char* devtype
		);

	typedef
	int
	udev_device_get_is_initialized_func(struct udev_device* device);

	typedef
	const char*
	udev_device_get_action_func(struct udev_device* device);

	typedef
	struct udev_list_entry*
	udev_device_get_devlinks_list_entry_func(struct udev_device* device);

	typedef
	struct udev_list_entry*
	udev_device_get_properties_list_entry_func(struct udev_device* device);

	typedef
	struct udev_list_entry*
	udev_device_get_tags_list_entry_func(struct udev_device* device);

	typedef
	struct udev_list_entry*
	udev_device_get_sysattr_list_entry_func(struct udev_device* device);

	typedef
	const char*
	udev_device_get_property_value_func(
		struct udev_device* device,
		const char* key
		);

	typedef
	int
	udev_device_has_tag_func(
		struct udev_device* device,
		const char* tag
		);

	typedef
	const char*
	udev_device_get_sysattr_value_func(
		struct udev_device* device,
		const char* sysattr
		);

	typedef
	int
	udev_device_set_sysattr_value_func(
		struct udev_device* device,
		const char* sysattr,
		const char* value
		);

	typedef
	struct udev_enumerate*
	udev_enumerate_ref_func(struct udev_enumerate* enumerate);

	typedef
	struct udev_enumerate*
	udev_enumerate_unref_func(struct udev_enumerate* enumerate);

	typedef
	struct udev*
	udev_enumerate_get_udev_func(struct udev_enumerate* enumerate);

	typedef
	int
	udev_enumerate_add_match_subsystem_func(
		struct udev_enumerate* enumerate,
		const char* subsystem
		);

	typedef
	int
	udev_enumerate_add_nomatch_subsystem_func(
		struct udev_enumerate* enumerate,
		const char* subsystem
		);

	typedef
	int
	udev_enumerate_add_match_sysattr_func(
		struct udev_enumerate* enumerate,
		const char* sysattr,
		const char* value
		);

	typedef
	int
	udev_enumerate_add_nomatch_sysattr_func(
		struct udev_enumerate* enumerate,
		const char* sysattr,
		const char* value
		);

	typedef
	int
	udev_enumerate_add_match_property_func(
		struct udev_enumerate* enumerate,
		const char* property,
		const char* value
		);

	typedef
	int
	udev_enumerate_add_match_sysname_func(
		struct udev_enumerate* enumerate,
		const char* sysname
		);

	typedef
	int
	udev_enumerate_add_match_tag_func(
		struct udev_enumerate* enumerate,
		const char* tag
		);

	typedef
	int
	udev_enumerate_add_match_parent_func(
		struct udev_enumerate* enumerate,
		struct udev_device* parent
		);

	typedef
	int
	udev_enumerate_add_match_is_initialized_func(struct udev_enumerate* enumerate);

	typedef
	int
	udev_enumerate_scan_devices_func(struct udev_enumerate* enumerate);

	typedef
	int
	udev_enumerate_scan_subsystems_func(struct udev_enumerate* enumerate);

	typedef
	int
	udev_enumerate_add_syspath_func(
		struct udev_enumerate* enumerate,
		const char* syspath
		);

	typedef
	struct udev_list_entry*
	udev_enumerate_get_list_entry_func(struct udev_enumerate* enumerate);

	typedef
	struct udev_list_entry*
	udev_list_entry_get_next_func(struct udev_list_entry* entry);

	typedef
	struct udev_list_entry*
	udev_list_entry_get_by_name_func(
		struct udev_list_entry* entry,
		const char* name
		);

	typedef
	const char*
	udev_list_entry_get_name_func(struct udev_list_entry* entry);

	typedef
	const char*
	udev_list_entry_get_value_func(struct udev_list_entry* entry);

	typedef
	struct udev_monitor*
	udev_monitor_ref_func(struct udev_monitor* monitor);

	typedef
	struct udev_monitor*
	udev_monitor_unref_func(struct udev_monitor* monitor);

	typedef
	struct udev*
	udev_monitor_get_udev_func(struct udev_monitor* monitor);

	typedef
	int
	udev_monitor_get_fd_func(struct udev_monitor* monitor);

	typedef
	int
	udev_monitor_filter_update_func(struct udev_monitor* monitor);

	typedef
	int
	udev_monitor_filter_remove_func(struct udev_monitor* monitor);

	typedef
	int
	udev_monitor_filter_add_match_subsystem_devtype_func(
		struct udev_monitor* monitor,
		const char* subsystem,
		const char* devtype
		);

	typedef
	int
	udev_monitor_filter_add_match_tag_func(
		struct udev_monitor* monitor,
		const char* tag
		);

	typedef
	int
	udev_monitor_set_receive_buffer_size_func(
		struct udev_monitor* monitor,
		int size
		);

	typedef
	int
	udev_monitor_enable_receiving_func(struct udev_monitor* monitor);

	typedef
	struct udev_device*
	udev_monitor_receive_device_func(struct udev_monitor* monitor);

protected:
	enum FuncId
	{
		FuncId_udev_ref,
		FuncId_udev_unref,
		FuncId_udev_new,
		FuncId_udev_device_new_from_syspath,
		FuncId_udev_device_new_from_devnum,
		FuncId_udev_device_new_from_subsystem_sysname,
		FuncId_udev_device_new_from_device_id,
		FuncId_udev_device_new_from_environment,
		FuncId_udev_enumerate_new,
		FuncId_udev_monitor_new_from_netlink,
		FuncId_udev_device_ref,
		FuncId_udev_device_unref,
		FuncId_udev_device_get_udev,
		FuncId_udev_device_get_syspath,
		FuncId_udev_device_get_sysname,
		FuncId_udev_device_get_sysnum,
		FuncId_udev_device_get_devpath,
		FuncId_udev_device_get_devnode,
		FuncId_udev_device_get_devnum,
		FuncId_udev_device_get_devtype,
		FuncId_udev_device_get_subsystem,
		FuncId_udev_device_get_driver,
		FuncId_udev_device_get_parent,
		FuncId_udev_device_get_parent_with_subsystem_devtype,
		FuncId_udev_device_get_is_initialized,
		FuncId_udev_device_get_action,
		FuncId_udev_device_get_devlinks_list_entry,
		FuncId_udev_device_get_properties_list_entry,
		FuncId_udev_device_get_tags_list_entry,
		FuncId_udev_device_get_sysattr_list_entry,
		FuncId_udev_device_get_property_value,
		FuncId_udev_device_has_tag,
		FuncId_udev_device_get_sysattr_value,
		FuncId_udev_device_set_sysattr_value,
		FuncId_udev_enumerate_ref,
		FuncId_udev_enumerate_unref,
		FuncId_udev_enumerate_get_udev,
		FuncId_udev_enumerate_add_match_subsystem,
		FuncId_udev_enumerate_add_nomatch_subsystem,
		FuncId_udev_enumerate_add_match_sysattr,
		FuncId_udev_enumerate_add_nomatch_sysattr,
		FuncId_udev_enumerate_add_match_property,
		FuncId_udev_enumerate_add_match_sysname,
		FuncId_udev_enumerate_add_match_tag,
		FuncId_udev_enumerate_add_match_parent,
		FuncId_udev_enumerate_add_match_is_initialized,
		FuncId_udev_enumerate_scan_devices,
		FuncId_udev_enumerate_scan_subsystems,
		FuncId_udev_enumerate_add_syspath,
		FuncId_udev_enumerate_get_list_entry,
		FuncId_udev_list_entry_get_next,
		FuncId_udev_list_entry_get_by_name,
		FuncId_udev_list_entry_get_name,
		FuncId_udev_list_entry_get_value,
		FuncId_udev_monitor_ref,
		FuncId_udev_monitor_unref,
		FuncId_udev_monitor_get_udev,
		FuncId_udev_monitor_get_fd,
		FuncId_udev_monitor_filter_update,
		FuncId_udev_monitor_filter_remove,
		FuncId_udev_monitor_filter_add_match_subsystem_devtype,
		FuncId_udev_monitor_filter_add_match_tag,
		FuncId_udev_monitor_set_receive_buffer_size,
		FuncId_udev_monitor_enable_receiving,
		FuncId_udev_monitor_receive_device,
		FuncId__Count,
	};

protected:
	void* m_funcTable[FuncId__Count];
	static const char* m_funcNameTable[FuncId__Count];

public:
	UdevLib()
	{
		memset(m_funcTable, 0, sizeof(m_funcTable));
		open("libudev.so.1") || open("libudev.so.0");
	}

	// context API

	static
	struct udev*
	udev_ref(struct udev* context)
	{
		void* p = getFunc(FuncId_udev_ref);
		return p ? ((udev_ref_func*)p)(context) : NULL;
	}

	static
	struct udev*
	udev_unref(struct udev* context)
	{
		void* p = getFunc(FuncId_udev_unref);
		return p ? ((udev_unref_func*)p)(context) : NULL;
	}

	static
	struct udev*
	udev_new()
	{
		void* p = getFunc(FuncId_udev_new);
		if (!p)
			return NULL;

		struct udev* context = ((udev_new_func*)p)();
		return completeUdev(context, "udev_new");
	}

	static
	struct udev_device*
	udev_device_new_from_syspath(
		struct udev* context,
		const char* syspath
		)
	{
		void* p = getFunc(FuncId_udev_device_new_from_syspath);
		if (!p)
			return NULL;

		struct udev_device* device = ((udev_device_new_from_syspath_func*)p)(context, syspath);
		return completeUdev(device, "udev_device_new_from_syspath");
	}

	static
	struct udev_device*
	udev_device_new_from_devnum(
		struct udev* context,
		char type,
		dev_t devnum
		)
	{
		void* p = getFunc(FuncId_udev_device_new_from_devnum);
		if (!p)
			return NULL;

		struct udev_device* device = ((udev_device_new_from_devnum_func*)p)(context, type, devnum);
		return completeUdev(device, "udev_device_new_from_devnum");
	}

	static
	struct udev_device*
	udev_device_new_from_subsystem_sysname(
		struct udev* context,
		const char* subsystem,
		const char* sysname
		)
	{
		void* p = getFunc(FuncId_udev_device_new_from_subsystem_sysname);
		if (!p)
			return NULL;

		struct udev_device* device = ((udev_device_new_from_subsystem_sysname_func*)p)(context, subsystem, sysname);
		return completeUdev(device, "udev_device_new_from_subsystem_sysname");
	}

#if (_AXL_SYS_LNX_LIBUDEV_NEW_API)
	static
	struct udev_device*
	udev_device_new_from_device_id(
		struct udev* context,
		const char* id
		)
	{
		void* p = getFunc(FuncId_udev_device_new_from_device_id);
		if (!p)
			return NULL;

		struct udev_device* device = ((udev_device_new_from_device_id_func*)p)(context, id);
		return completeUdev(device, "udev_device_new_from_device_id");
	}
#endif

	static
	struct udev_device*
	udev_device_new_from_environment(struct udev* context)
	{
		void* p = getFunc(FuncId_udev_device_new_from_environment);
		if (!p)
			return NULL;

		struct udev_device* device = ((udev_device_new_from_environment_func*)p)(context);
		return completeUdev(device, "udev_device_new_from_environment");
	}

	static
	struct udev_enumerate*
	udev_enumerate_new(struct udev* context)
	{
		void* p = getFunc(FuncId_udev_enumerate_new);
		if (!p)
			return NULL;

		struct udev_enumerate* enumerate = ((udev_enumerate_new_func*)p)(context);
		return completeUdev(enumerate, "udev_device_new_from_environment");
	}

	static
	struct udev_monitor*
	udev_monitor_new_from_netlink(
		struct udev* context,
		const char* name
		)
	{
		void* p = getFunc(FuncId_udev_monitor_new_from_netlink);
		if (!p)
			return NULL;

		struct udev_monitor* monitor = ((udev_monitor_new_from_netlink_func*)p)(context, name);
		return completeUdev(monitor, "udev_monitor_new_from_netlink");
	}

	// device API

	static
	struct udev_device*
	udev_device_ref(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_ref);
		return p ? ((udev_device_ref_func*)p)(device) : NULL;
	}

	static
	struct udev_device*
	udev_device_unref(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_unref);
		return p ? ((udev_device_unref_func*)p)(device) : NULL;
	}

	static
	struct udev*
	udev_device_get_udev(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_udev);
		return p ? ((udev_device_get_udev_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_syspath(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_syspath);
		return p ? ((udev_device_get_syspath_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_sysname(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_sysname);
		return p ? ((udev_device_get_sysname_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_sysnum(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_sysnum);
		return p ? ((udev_device_get_sysnum_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_devpath(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_devpath);
		return p ? ((udev_device_get_devpath_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_devnode(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_devnode);
		return p ? ((udev_device_get_devnode_func*)p)(device) : NULL;
	}

	static
	dev_t
	udev_device_get_devnum(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_devnum);
		return p ? ((udev_device_get_devnum_func*)p)(device) : 0;
	}

	static
	const char*
	udev_device_get_devtype(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_devtype);
		return p ? ((udev_device_get_devtype_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_subsystem(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_subsystem);
		return p ? ((udev_device_get_subsystem_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_driver(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_driver);
		return p ? ((udev_device_get_driver_func*)p)(device) : NULL;
	}

	static
	struct udev_device*
	udev_device_get_parent(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_parent);
		return p ? ((udev_device_get_parent_func*)p)(device) : NULL;
	}

	static
	struct udev_device*
	udev_device_get_parent_with_subsystem_devtype(
		struct udev_device* device,
		const char* subsystem,
		const char* devtype
		)
	{
		void* p = getFunc(FuncId_udev_device_get_parent_with_subsystem_devtype);
		return p ? ((udev_device_get_parent_with_subsystem_devtype_func*)p)(device, subsystem, devtype) : NULL;
	}

	static
	bool
	udev_device_get_is_initialized(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_is_initialized);
		return p ? ((udev_device_get_is_initialized_func*)p)(device) != 0 : false;
	}

	static
	const char*
	udev_device_get_action(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_action);
		return p ? ((udev_device_get_action_func*)p)(device) : NULL;
	}

	static
	struct udev_list_entry*
	udev_device_get_devlinks_list_entry(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_devlinks_list_entry);
		return p ? ((udev_device_get_devlinks_list_entry_func*)p)(device) : NULL;
	}

	static
	struct udev_list_entry*
	udev_device_get_properties_list_entry(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_properties_list_entry);
		return p ? ((udev_device_get_properties_list_entry_func*)p)(device) : NULL;
	}

	static
	struct udev_list_entry*
	udev_device_get_tags_list_entry(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_tags_list_entry);
		return p ? ((udev_device_get_tags_list_entry_func*)p)(device) : NULL;
	}

	static
	struct udev_list_entry*
	udev_device_get_sysattr_list_entry(struct udev_device* device)
	{
		void* p = getFunc(FuncId_udev_device_get_sysattr_list_entry);
		return p ? ((udev_device_get_sysattr_list_entry_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_property_value(
		struct udev_device* device,
		const char* key
		)
	{
		void* p = getFunc(FuncId_udev_device_get_property_value);
		return p ? ((udev_device_get_property_value_func*)p)(device, key) : NULL;
	}

	static
	bool
	udev_device_has_tag(
		struct udev_device* device,
		const char* tag
		)
	{
		void* p = getFunc(FuncId_udev_device_has_tag);
		return p ? ((udev_device_has_tag_func*)p)(device, tag) != 0 : false;
	}

#if (_AXL_SYS_LNX_LIBUDEV_NEW_API)
	static
	const char*
	udev_device_get_sysattr_value(
		struct udev_device* device,
		const char* sysattr
		)
	{
		void* p = getFunc(FuncId_udev_device_get_sysattr_value);
		return p ? ((udev_device_get_sysattr_value_func*)p)(device, sysattr) : NULL;
	}

	static
	int
	udev_device_set_sysattr_value(
		struct udev_device* device,
		const char* sysattr,
		const char* value
		)
	{
		void* p = getFunc(FuncId_udev_device_set_sysattr_value);
		return p ? ((udev_device_set_sysattr_value_func*)p)(device, sysattr, value) : NULL;
	}
#endif

	// enumerator API

	static
	struct udev_enumerate*
	udev_enumerate_ref(struct udev_enumerate* enumerate)
	{
		void* p = getFunc(FuncId_udev_enumerate_ref);
		return p ? ((udev_enumerate_ref_func*)p)(enumerate) : NULL;
	}

	static
	struct udev_enumerate*
	udev_enumerate_unref(struct udev_enumerate* enumerate)
	{
		void* p = getFunc(FuncId_udev_enumerate_unref);
		return p ? ((udev_enumerate_unref_func*)p)(enumerate) : NULL;
	}

	static
	struct udev*
	udev_enumerate_get_udev(struct udev_enumerate* enumerate)
	{
		void* p = getFunc(FuncId_udev_enumerate_get_udev);
		return p ? ((udev_enumerate_get_udev_func*)p)(enumerate) : NULL;
	}

	static
	bool
	udev_enumerate_add_match_subsystem_b(
		struct udev_enumerate* enumerate,
		const char* subsystem
		)
	{
		void* p = getFunc(FuncId_udev_enumerate_add_match_subsystem);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_match_subsystem_func*)p)(enumerate, subsystem);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_nomatch_subsystem_b(
		struct udev_enumerate* enumerate,
		const char* subsystem
		)
	{
		void* p = getFunc(FuncId_udev_enumerate_add_nomatch_subsystem);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_nomatch_subsystem_func*)p)(enumerate, subsystem);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_sysattr_b(
		struct udev_enumerate* enumerate,
		const char* sysattr,
		const char* value
		)
	{
		void* p = getFunc(FuncId_udev_enumerate_add_match_sysattr);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_match_sysattr_func*)p)(enumerate, sysattr, value);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_nomatch_sysattr_b(
		struct udev_enumerate* enumerate,
		const char* sysattr,
		const char* value
		)
	{
		void* p = getFunc(FuncId_udev_enumerate_add_nomatch_sysattr);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_nomatch_sysattr_func*)p)(enumerate, sysattr, value);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_property_b(
		struct udev_enumerate* enumerate,
		const char* property,
		const char* value
		)
	{
		void* p = getFunc(FuncId_udev_enumerate_add_match_property);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_match_property_func*)p)(enumerate, property, value);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_sysname_b(
		struct udev_enumerate* enumerate,
		const char* sysname
		)
	{
		void* p = getFunc(FuncId_udev_enumerate_add_match_sysname);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_match_sysname_func*)p)(enumerate, sysname);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_tag_b(
		struct udev_enumerate* enumerate,
		const char* tag
		)
	{
		void* p = getFunc(FuncId_udev_enumerate_add_match_tag);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_match_tag_func*)p)(enumerate, tag);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_parent_b(
		struct udev_enumerate* enumerate,
		struct udev_device* parent
		)
	{
		void* p = getFunc(FuncId_udev_enumerate_add_match_parent);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_match_parent_func*)p)(enumerate, parent);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_is_initialized_b(struct udev_enumerate* enumerate)
	{
		void* p = getFunc(FuncId_udev_enumerate_add_match_is_initialized);
		return p ? ((udev_enumerate_add_match_is_initialized_func*)p)(enumerate) != 0 : false;
	}

	static
	bool
	udev_enumerate_scan_devices_b(struct udev_enumerate* enumerate)
	{
		void* p = getFunc(FuncId_udev_enumerate_scan_devices);
		if (!p)
			return false;

		int result = ((udev_enumerate_scan_devices_func*)p)(enumerate);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_scan_subsystems_b(struct udev_enumerate* enumerate)
	{
		void* p = getFunc(FuncId_udev_enumerate_scan_subsystems);
		if (!p)
			return false;

		int result = ((udev_enumerate_scan_subsystems_func*)p)(enumerate);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_syspath_b(
		struct udev_enumerate* enumerate,
		const char* syspath
		)
	{
		void* p = getFunc(FuncId_udev_enumerate_add_syspath);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_syspath_func*)p)(enumerate, syspath);
		return completeUdevErrno(result);
	}

	static
	struct udev_list_entry*
	udev_enumerate_get_list_entry(struct udev_enumerate* enumerate)
	{
		void* p = getFunc(FuncId_udev_enumerate_get_list_entry);
		return p ? ((udev_enumerate_get_list_entry_func*)p)(enumerate) : NULL;
	}

	// list entry API

	static
	struct udev_list_entry*
	udev_list_entry_get_next(struct udev_list_entry* entry)
	{
		void* p = getFunc(FuncId_udev_list_entry_get_next);
		return p ? ((udev_list_entry_get_next_func*)p)(entry) : NULL;
	}

	static
	struct udev_list_entry*
	udev_list_entry_get_by_name(
		struct udev_list_entry* entry,
		const char* name
		)
	{
		void* p = getFunc(FuncId_udev_list_entry_get_by_name);
		return p ? ((udev_list_entry_get_by_name_func*)p)(entry, name) : NULL;
	}

	static
	const char*
	udev_list_entry_get_name(struct udev_list_entry* entry)
	{
		void* p = getFunc(FuncId_udev_list_entry_get_name);
		return p ? ((udev_list_entry_get_name_func*)p)(entry) : NULL;
	}

	static
	const char*
	udev_list_entry_get_value(struct udev_list_entry* entry)
	{
		void* p = getFunc(FuncId_udev_list_entry_get_value);
		return p ? ((udev_list_entry_get_value_func*)p)(entry) : NULL;
	}

	// monitor API

	static
	struct udev_monitor*
	udev_monitor_ref(struct udev_monitor* monitor)
	{
		void* p = getFunc(FuncId_udev_monitor_ref);
		return p ? ((udev_monitor_ref_func*)p)(monitor) : NULL;
	}

	static
	struct udev_monitor*
	udev_monitor_unref(struct udev_monitor* monitor)
	{
		void* p = getFunc(FuncId_udev_monitor_unref);
		return p ? ((udev_monitor_unref_func*)p)(monitor) : NULL;
	}

	static
	struct udev*
	udev_monitor_get_udev(struct udev_monitor* monitor)
	{
		void* p = getFunc(FuncId_udev_monitor_get_udev);
		return p ? ((udev_monitor_get_udev_func*)p)(monitor) : NULL;
	}

	static
	int
	udev_monitor_get_fd(struct udev_monitor* monitor)
	{
		void* p = getFunc(FuncId_udev_monitor_get_fd);
		return p ? ((udev_monitor_get_fd_func*)p)(monitor) : -1;
	}

	static
	bool
	udev_monitor_filter_update_b(struct udev_monitor* monitor)
	{
		void* p = getFunc(FuncId_udev_monitor_filter_update);
		if (!p)
			return false;

		int result = ((udev_monitor_filter_update_func*)p)(monitor);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_filter_remove_b(struct udev_monitor* monitor)
	{
		void* p = getFunc(FuncId_udev_monitor_filter_remove);
		if (!p)
			return false;

		int result = ((udev_monitor_filter_remove_func*)p)(monitor);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_filter_add_match_subsystem_devtype_b(
		struct udev_monitor* monitor,
		const char* subsystem,
		const char* devtype
		)
	{
		void* p = getFunc(FuncId_udev_monitor_filter_add_match_subsystem_devtype);
		if (!p)
			return false;

		int result = ((udev_monitor_filter_add_match_subsystem_devtype_func*)p)(monitor, subsystem, devtype);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_filter_add_match_tag_b(
		struct udev_monitor* monitor,
		const char* tag
		)
	{
		void* p = getFunc(FuncId_udev_monitor_filter_add_match_tag);
		if (!p)
			return false;

		int result = ((udev_monitor_filter_add_match_tag_func*)p)(monitor, tag);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_set_receive_buffer_size_b(
		struct udev_monitor* monitor,
		int size
		)
	{
		void* p = getFunc(FuncId_udev_monitor_set_receive_buffer_size);
		if (!p)
			return false;

		int result = ((udev_monitor_set_receive_buffer_size_func*)p)(monitor, size);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_enable_receiving_b(struct udev_monitor* monitor)
	{
		void* p = getFunc(FuncId_udev_monitor_enable_receiving);
		if (!p)
			return false;

		int result = ((udev_monitor_enable_receiving_func*)p)(monitor);
		return completeUdevErrno(result);
	}

	static
	struct udev_device*
	udev_monitor_receive_device(struct udev_monitor* monitor)
	{
		void* p = getFunc(FuncId_udev_monitor_receive_device);
		if (!p)
			return NULL;

		struct udev_device* device = ((udev_monitor_receive_device_func*)p)(monitor);
		return completeUdev(device, "udev_monitor_receive_device ");
	}

protected:
	static
	void*
	getFunc(FuncId id)
	{
		return sl::getSingleton<UdevLib>()->getFuncImpl(id);
	}

	void*
	getFuncImpl(FuncId id)
	{
		ASSERT((size_t)id < FuncId__Count);

		return m_funcTable[id] ?
			m_funcTable[id] :
			m_funcTable[id] = getFunction(m_funcNameTable[id]);
	}
};

//..............................................................................

typedef UdevContextBase<Udev>    UdevContext_s;
typedef UdevDeviceBase<Udev>     UdevDevice_s;
typedef UdevEnumeratorBase<Udev> UdevEnumerator_s;
typedef UdevListEntryBase<Udev>  UdevListEntry_s;
typedef UdevMonitorBase<Udev>    UdevMonitor_s;

typedef UdevContextBase<UdevLib>    DynamicUdevContext;
typedef UdevDeviceBase<UdevLib>     DynamicUdevDevice;
typedef UdevEnumeratorBase<UdevLib> DynamicUdevEnumerator;
typedef UdevListEntryBase<UdevLib>  DynamicUdevListEntry;
typedef UdevMonitorBase<UdevLib>    DynamicUdevMonitor;

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
