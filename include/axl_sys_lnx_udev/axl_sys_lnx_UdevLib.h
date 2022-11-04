
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

#define _AXL_SYS_LNX_UDEVLIB_H

#include "axl_sys_lnx_UdevPch.h"
/*
namespace axl {
namespace sys {
namespace lnx {
namespace udev {

// don't rely on the system header libudev.h and static linking to libudev.so
// instead, declare everything we need here and link to libudev.so dynamically
// to increase portablity among different linux distros

//..............................................................................

//
// udev opaque structs

struct udev;
struct udev_list_entry;
struct udev_device;
struct udev_monitor;
struct udev_enumerate;
struct udev_queue;
struct udev_hwdb;

//
// udev library context
//

typedef
udev*
udev_ref_t(udev* udev);

typedef
udev*
udev_unref_t(udev* udev);

typedef
udev*
udev_new_t(void);

typedef
void*
udev_get_userdata_t(udev* udev);

typedef
void
udev_set_userdata_t(
	udev* udev,
	void* userdata
);

//
// udev_list
//

typedef
udev_list_entry*
udev_list_entry_get_next_t(udev_list_entry* list_entry);

typedef
udev_list_entry*
udev_list_entry_get_by_name_t(
	udev_list_entry* list_entry,
	const char* name
);

typedef
const char*
udev_list_entry_get_name_t(udev_list_entry* list_entry);

typedef
const char*
udev_list_entry_get_value_t(udev_list_entry* list_entry);

//
// udev_device
//

typedef
udev_device*
udev_device_ref_t(udev_device* udev_device);

typedef
udev_device*
udev_device_unref_t(udev_device* udev_device);

typedef
udev*
udev_device_get_udev_t(udev_device* udev_device);

typedef
udev_device*
udev_device_new_from_syspath_t(
	udev* udev,
	const char* syspath
);

typedef
udev_device*
udev_device_new_from_devnum_t(
	udev* udev,
	char type,
	dev_t devnum
);

typedef
udev_device*
udev_device_new_from_subsystem_sysname_t(
	udev* udev,
	const char* subsystem,
	const char* sysname
);

typedef
udev_device*
udev_device_new_from_device_id_t(
	udev* udev,
	const char* id
);

typedef
udev_device*
udev_device_new_from_environment_t(udev* udev);

typedef
udev_device*
udev_device_get_parent_t(udev_device* udev_device);

typedef
udev_device*
udev_device_get_parent_with_subsystem_devtype_t(
	udev_device* udev_device,
	const char* subsystem,
	const char* devtype
);

typedef
const char*
udev_device_get_devpath_t(udev_device* udev_device);

typedef
const char*
udev_device_get_subsystem_t(udev_device* udev_device);

typedef
const char*
udev_device_get_devtype_t(udev_device* udev_device);

typedef
const char*
udev_device_get_syspath_t(udev_device* udev_device);

typedef
const char*
udev_device_get_sysname_t(udev_device* udev_device);

typedef
const char*
udev_device_get_sysnum_t(udev_device* udev_device);

typedef
const char*
udev_device_get_devnode_t(udev_device* udev_device);

typedef
int
udev_device_get_is_initialized_t(udev_device* udev_device);

typedef
udev_list_entry*
udev_device_get_devlinks_list_entry_t(udev_device* udev_device);

typedef
udev_list_entry*
udev_device_get_properties_list_entry_t(udev_device* udev_device);

typedef
udev_list_entry*
udev_device_get_tags_list_entry_t(udev_device* udev_device);

typedef
udev_list_entry*
udev_device_get_current_tags_list_entry_t(udev_device* udev_device);

typedef
udev_list_entry*
udev_device_get_sysattr_list_entry_t(udev_device* udev_device);

typedef
const char*
udev_device_get_property_value_t(
	udev_device* udev_device,
	const char* key
);

typedef
const char*
udev_device_get_driver_t(udev_device* udev_device);

typedef
dev_t
udev_device_get_devnum_t(udev_device* udev_device);

typedef
const char*
udev_device_get_action_t(udev_device* udev_device);

typedef
unsigned long long int
udev_device_get_seqnum_t(udev_device* udev_device);

typedef
unsigned long long int
udev_device_get_usec_since_initialized_t(udev_device* udev_device);

typedef
const char*
udev_device_get_sysattr_value_t(
	udev_device* udev_device,
	const char* sysattr
);

typedef
int
udev_device_set_sysattr_value_t(
	udev_device* udev_device,
	const char* sysattr,
	const char* value
);

typedef
int
udev_device_has_tag_t(
	udev_device* udev_device,
	const char* tag
);

typedef
int
udev_device_has_current_tag_t(
	udev_device* udev_device,
	const char* tag
);

//
// udev_monitor
//

typedef
udev_monitor*
udev_monitor_ref_t(udev_monitor* udev_monitor);

typedef
udev_monitor*
udev_monitor_unref_t(udev_monitor* udev_monitor);

typedef
udev*
udev_monitor_get_udev_t(udev_monitor* udev_monitor);

typedef
udev_monitor*
udev_monitor_new_from_netlink_t(
	udev* udev,
	const char* name
);

typedef
int
udev_monitor_enable_receiving_t(udev_monitor* udev_monitor);

typedef
int
udev_monitor_set_receive_buffer_size_t(
	udev_monitor* udev_monitor,
	int size
);

typedef
int
udev_monitor_get_fd_t(udev_monitor* udev_monitor);

typedef
udev_device*
udev_monitor_receive_device_t(udev_monitor* udev_monitor);

typedef
int
udev_monitor_filter_add_match_subsystem_devtype_t(
	udev_monitor* udev_monitor,
	const char* subsystem,
	const char* devtype
);

typedef
int
udev_monitor_filter_add_match_tag_t(
	udev_monitor* udev_monitor,
	const char* tag
);

typedef
int
udev_monitor_filter_update_t(udev_monitor* udev_monitor);

typedef
int
udev_monitor_filter_remove_t(udev_monitor* udev_monitor);

//
// udev_enumerate
//

typedef
udev_enumerate*
udev_enumerate_ref_t(udev_enumerate* udev_enumerate);

typedef
udev_enumerate*
udev_enumerate_unref_t(udev_enumerate* udev_enumerate);

typedef
udev*
udev_enumerate_get_udev_t(udev_enumerate* udev_enumerate);

typedef
udev_enumerate*
udev_enumerate_new_t(udev* udev);

typedef
int
udev_enumerate_add_match_subsystem_t(
	udev_enumerate* udev_enumerate,
	const char* subsystem
);

typedef
int
udev_enumerate_add_nomatch_subsystem_t(
	udev_enumerate* udev_enumerate,
	const char* subsystem
);

typedef
int
udev_enumerate_add_match_sysattr_t(
	udev_enumerate* udev_enumerate,
	const char* sysattr,
	const char* value
);

typedef
int
udev_enumerate_add_nomatch_sysattr_t(
	udev_enumerate* udev_enumerate,
	const char* sysattr,
	const char* value
);

typedef
int
udev_enumerate_add_match_property_t(
	udev_enumerate* udev_enumerate,
	const char* property,
	const char* value
);

typedef
int
udev_enumerate_add_match_sysname_t(
	udev_enumerate* udev_enumerate,
	const char* sysname
);

typedef
int
udev_enumerate_add_match_tag_t(
	udev_enumerate* udev_enumerate,
	const char* tag
);

typedef
int
udev_enumerate_add_match_parent_t(
	udev_enumerate* udev_enumerate,
	udev_device* parent
);

typedef
int
udev_enumerate_add_match_is_initialized_t(udev_enumerate* udev_enumerate);

typedef
int udev_enumerate_add_syspath_t(
	udev_enumerate* udev_enumerate,
	const char* syspath
);

typedef
int
udev_enumerate_scan_devices_t(udev_enumerate* udev_enumerate);

typedef
int
udev_enumerate_scan_subsystems_t(udev_enumerate* udev_enumerate);

typedef
udev_list_entry*
udev_enumerate_get_list_entry_t(udev_enumerate* udev_enumerate);

//
// udev_queue
//

typedef
udev_queue*
udev_queue_ref_t(udev_queue* udev_queue);

typedef
udev_queue*
udev_queue_unref_t(udev_queue* udev_queue);

typedef
udev*
udev_queue_get_udev_t(udev_queue* udev_queue);

typedef
udev_queue*
udev_queue_new_t(udev* udev);

typedef
int
udev_queue_get_udev_is_active_t(udev_queue* udev_queue);

typedef
int
udev_queue_get_queue_is_empty_t(udev_queue* udev_queue);

typedef
int
udev_queue_get_fd_t(udev_queue* udev_queue);

typedef
typedef
int
udev_queue_flush_t(udev_queue* udev_queue);

//
// udev_hwdb
//

typedef
udev_hwdb*
udev_hwdb_new_t(udev* udev);

typedef
udev_hwdb*
udev_hwdb_ref_t(udev_hwdb* hwdb);

typedef
udev_hwdb*
udev_hwdb_unref_t(udev_hwdb* hwdb);

typedef
udev_list_entry*
udev_hwdb_get_properties_list_entry_t(
	udev_hwdb* hwdb,
	const char* modalias,
	unsigned flags
);

//..............................................................................

} // namespace udev

using namespace udev; // only in this file

// unfortunately, udev does not set errno properly

template <typename T>
T
completeUdev(
	T result,
	const char* functionName
) {
	if (!result)
		err::setFormatStringError("'%s' failed", functionName);

	return result;
}

inline
bool
completeUdevErrno(int result) {
	if (result != 0) {
		err::setError(-result);
		return false;
	}

	return true;
}

//..............................................................................

class UdevLib: public DynamicLib {
public:
	UdevLib() {
		open("libudev.so.1") || open("libudev.so.0");
	}

	// context API

	static
	udev*
	udev_ref(udev* context) {
		void* p = getFunc(FuncId_udev_ref);
		return p ? ((udev_ref_func*)p)(context) : NULL;
	}

	static
	udev*
	udev_unref(udev* context) {
		void* p = getFunc(FuncId_udev_unref);
		return p ? ((udev_unref_func*)p)(context) : NULL;
	}

	static
	udev*
	udev_new() {
		void* p = getFunc(FuncId_udev_new);
		if (!p)
			return NULL;

		udev* context = ((udev_new_func*)p)();
		return completeUdev(context, "udev_new");
	}

	static
	udev_device*
	udev_device_new_from_syspath(
		udev* context,
		const char* syspath
	) {
		void* p = getFunc(FuncId_udev_device_new_from_syspath);
		if (!p)
			return NULL;

		udev_device* device = ((udev_device_new_from_syspath_func*)p)(context, syspath);
		return completeUdev(device, "udev_device_new_from_syspath");
	}

	static
	udev_device*
	udev_device_new_from_devnum(
		udev* context,
		char type,
		dev_t devnum
	) {
		void* p = getFunc(FuncId_udev_device_new_from_devnum);
		if (!p)
			return NULL;

		udev_device* device = ((udev_device_new_from_devnum_func*)p)(context, type, devnum);
		return completeUdev(device, "udev_device_new_from_devnum");
	}

	static
	udev_device*
	udev_device_new_from_subsystem_sysname(
		udev* context,
		const char* subsystem,
		const char* sysname
	) {
		void* p = getFunc(FuncId_udev_device_new_from_subsystem_sysname);
		if (!p)
			return NULL;

		udev_device* device = ((udev_device_new_from_subsystem_sysname_func*)p)(context, subsystem, sysname);
		return completeUdev(device, "udev_device_new_from_subsystem_sysname");
	}

	static
	udev_device*
	udev_device_new_from_device_id(
		udev* context,
		const char* id
	) {
		void* p = getFunc(FuncId_udev_device_new_from_device_id);
		if (!p)
			return NULL;

		udev_device* device = ((udev_device_new_from_device_id_func*)p)(context, id);
		return completeUdev(device, "udev_device_new_from_device_id");
	}

	static
	udev_device*
	udev_device_new_from_environment(udev* context) {
		void* p = getFunc(FuncId_udev_device_new_from_environment);
		if (!p)
			return NULL;

		udev_device* device = ((udev_device_new_from_environment_func*)p)(context);
		return completeUdev(device, "udev_device_new_from_environment");
	}

	static
	udev_enumerate*
	udev_enumerate_new(udev* context) {
		void* p = getFunc(FuncId_udev_enumerate_new);
		if (!p)
			return NULL;

		udev_enumerate* enumerate = ((udev_enumerate_new_func*)p)(context);
		return completeUdev(enumerate, "udev_device_new_from_environment");
	}

	static
	udev_monitor*
	udev_monitor_new_from_netlink(
		udev* context,
		const char* name
	) {
		void* p = getFunc(FuncId_udev_monitor_new_from_netlink);
		if (!p)
			return NULL;

		udev_monitor* monitor = ((udev_monitor_new_from_netlink_func*)p)(context, name);
		return completeUdev(monitor, "udev_monitor_new_from_netlink");
	}

	// device API

	static
	udev_device*
	udev_device_ref(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_ref);
		return p ? ((udev_device_ref_func*)p)(device) : NULL;
	}

	static
	udev_device*
	udev_device_unref(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_unref);
		return p ? ((udev_device_unref_func*)p)(device) : NULL;
	}

	static
	udev*
	udev_device_get_udev(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_udev);
		return p ? ((udev_device_get_udev_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_syspath(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_syspath);
		return p ? ((udev_device_get_syspath_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_sysname(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_sysname);
		return p ? ((udev_device_get_sysname_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_sysnum(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_sysnum);
		return p ? ((udev_device_get_sysnum_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_devpath(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_devpath);
		return p ? ((udev_device_get_devpath_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_devnode(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_devnode);
		return p ? ((udev_device_get_devnode_func*)p)(device) : NULL;
	}

	static
	dev_t
	udev_device_get_devnum(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_devnum);
		return p ? ((udev_device_get_devnum_func*)p)(device) : 0;
	}

	static
	const char*
	udev_device_get_devtype(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_devtype);
		return p ? ((udev_device_get_devtype_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_subsystem(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_subsystem);
		return p ? ((udev_device_get_subsystem_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_driver(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_driver);
		return p ? ((udev_device_get_driver_func*)p)(device) : NULL;
	}

	static
	udev_device*
	udev_device_get_parent(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_parent);
		return p ? ((udev_device_get_parent_func*)p)(device) : NULL;
	}

	static
	udev_device*
	udev_device_get_parent_with_subsystem_devtype(
		udev_device* device,
		const char* subsystem,
		const char* devtype
	) {
		void* p = getFunc(FuncId_udev_device_get_parent_with_subsystem_devtype);
		return p ? ((udev_device_get_parent_with_subsystem_devtype_func*)p)(device, subsystem, devtype) : NULL;
	}

	static
	bool
	udev_device_get_is_initialized(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_is_initialized);
		return p ? ((udev_device_get_is_initialized_func*)p)(device) != 0 : false;
	}

	static
	const char*
	udev_device_get_action(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_action);
		return p ? ((udev_device_get_action_func*)p)(device) : NULL;
	}

	static
	udev_list_entry*
	udev_device_get_devlinks_list_entry(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_devlinks_list_entry);
		return p ? ((udev_device_get_devlinks_list_entry_func*)p)(device) : NULL;
	}

	static
	udev_list_entry*
	udev_device_get_properties_list_entry(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_properties_list_entry);
		return p ? ((udev_device_get_properties_list_entry_func*)p)(device) : NULL;
	}

	static
	udev_list_entry*
	udev_device_get_tags_list_entry(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_tags_list_entry);
		return p ? ((udev_device_get_tags_list_entry_func*)p)(device) : NULL;
	}

	static
	udev_list_entry*
	udev_device_get_sysattr_list_entry(udev_device* device) {
		void* p = getFunc(FuncId_udev_device_get_sysattr_list_entry);
		return p ? ((udev_device_get_sysattr_list_entry_func*)p)(device) : NULL;
	}

	static
	const char*
	udev_device_get_property_value(
		udev_device* device,
		const char* key
	) {
		void* p = getFunc(FuncId_udev_device_get_property_value);
		return p ? ((udev_device_get_property_value_func*)p)(device, key) : NULL;
	}

	static
	bool
	udev_device_has_tag(
		udev_device* device,
		const char* tag
	) {
		void* p = getFunc(FuncId_udev_device_has_tag);
		return p ? ((udev_device_has_tag_func*)p)(device, tag) != 0 : false;
	}

	static
	const char*
	udev_device_get_sysattr_value(
		udev_device* device,
		const char* sysattr
	) {
		void* p = getFunc(FuncId_udev_device_get_sysattr_value);
		return p ? ((udev_device_get_sysattr_value_func*)p)(device, sysattr) : NULL;
	}

	static
	int
	udev_device_set_sysattr_value(
		udev_device* device,
		const char* sysattr,
		const char* value
	) {
		void* p = getFunc(FuncId_udev_device_set_sysattr_value);
		return p ? ((udev_device_set_sysattr_value_func*)p)(device, sysattr, value) : NULL;
	}

	// enumerator API

	static
	udev_enumerate*
	udev_enumerate_ref(udev_enumerate* enumerate) {
		void* p = getFunc(FuncId_udev_enumerate_ref);
		return p ? ((udev_enumerate_ref_func*)p)(enumerate) : NULL;
	}

	static
	udev_enumerate*
	udev_enumerate_unref(udev_enumerate* enumerate) {
		void* p = getFunc(FuncId_udev_enumerate_unref);
		return p ? ((udev_enumerate_unref_func*)p)(enumerate) : NULL;
	}

	static
	udev*
	udev_enumerate_get_udev(udev_enumerate* enumerate) {
		void* p = getFunc(FuncId_udev_enumerate_get_udev);
		return p ? ((udev_enumerate_get_udev_func*)p)(enumerate) : NULL;
	}

	static
	bool
	udev_enumerate_add_match_subsystem_b(
		udev_enumerate* enumerate,
		const char* subsystem
	) {
		void* p = getFunc(FuncId_udev_enumerate_add_match_subsystem);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_match_subsystem_func*)p)(enumerate, subsystem);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_nomatch_subsystem_b(
		udev_enumerate* enumerate,
		const char* subsystem
	) {
		void* p = getFunc(FuncId_udev_enumerate_add_nomatch_subsystem);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_nomatch_subsystem_func*)p)(enumerate, subsystem);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_sysattr_b(
		udev_enumerate* enumerate,
		const char* sysattr,
		const char* value
	) {
		void* p = getFunc(FuncId_udev_enumerate_add_match_sysattr);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_match_sysattr_func*)p)(enumerate, sysattr, value);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_nomatch_sysattr_b(
		udev_enumerate* enumerate,
		const char* sysattr,
		const char* value
	) {
		void* p = getFunc(FuncId_udev_enumerate_add_nomatch_sysattr);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_nomatch_sysattr_func*)p)(enumerate, sysattr, value);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_property_b(
		udev_enumerate* enumerate,
		const char* property,
		const char* value
	) {
		void* p = getFunc(FuncId_udev_enumerate_add_match_property);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_match_property_func*)p)(enumerate, property, value);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_sysname_b(
		udev_enumerate* enumerate,
		const char* sysname
	) {
		void* p = getFunc(FuncId_udev_enumerate_add_match_sysname);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_match_sysname_func*)p)(enumerate, sysname);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_tag_b(
		udev_enumerate* enumerate,
		const char* tag
	) {
		void* p = getFunc(FuncId_udev_enumerate_add_match_tag);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_match_tag_func*)p)(enumerate, tag);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_parent_b(
		udev_enumerate* enumerate,
		udev_device* parent
	) {
		void* p = getFunc(FuncId_udev_enumerate_add_match_parent);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_match_parent_func*)p)(enumerate, parent);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_match_is_initialized_b(udev_enumerate* enumerate) {
		void* p = getFunc(FuncId_udev_enumerate_add_match_is_initialized);
		return p ? ((udev_enumerate_add_match_is_initialized_func*)p)(enumerate) != 0 : false;
	}

	static
	bool
	udev_enumerate_scan_devices_b(udev_enumerate* enumerate) {
		void* p = getFunc(FuncId_udev_enumerate_scan_devices);
		if (!p)
			return false;

		int result = ((udev_enumerate_scan_devices_func*)p)(enumerate);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_scan_subsystems_b(udev_enumerate* enumerate) {
		void* p = getFunc(FuncId_udev_enumerate_scan_subsystems);
		if (!p)
			return false;

		int result = ((udev_enumerate_scan_subsystems_func*)p)(enumerate);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_enumerate_add_syspath_b(
		udev_enumerate* enumerate,
		const char* syspath
	) {
		void* p = getFunc(FuncId_udev_enumerate_add_syspath);
		if (!p)
			return false;

		int result = ((udev_enumerate_add_syspath_func*)p)(enumerate, syspath);
		return completeUdevErrno(result);
	}

	static
	udev_list_entry*
	udev_enumerate_get_list_entry(udev_enumerate* enumerate) {
		void* p = getFunc(FuncId_udev_enumerate_get_list_entry);
		return p ? ((udev_enumerate_get_list_entry_func*)p)(enumerate) : NULL;
	}

	// list entry API

	static
	udev_list_entry*
	udev_list_entry_get_next(udev_list_entry* entry) {
		void* p = getFunc(FuncId_udev_list_entry_get_next);
		return p ? ((udev_list_entry_get_next_func*)p)(entry) : NULL;
	}

	static
	udev_list_entry*
	udev_list_entry_get_by_name(
		udev_list_entry* entry,
		const char* name
	) {
		void* p = getFunc(FuncId_udev_list_entry_get_by_name);
		return p ? ((udev_list_entry_get_by_name_func*)p)(entry, name) : NULL;
	}

	static
	const char*
	udev_list_entry_get_name(udev_list_entry* entry) {
		void* p = getFunc(FuncId_udev_list_entry_get_name);
		return p ? ((udev_list_entry_get_name_func*)p)(entry) : NULL;
	}

	static
	const char*
	udev_list_entry_get_value(udev_list_entry* entry) {
		void* p = getFunc(FuncId_udev_list_entry_get_value);
		return p ? ((udev_list_entry_get_value_func*)p)(entry) : NULL;
	}

	// monitor API

	static
	udev_monitor*
	udev_monitor_ref(udev_monitor* monitor) {
		void* p = getFunc(FuncId_udev_monitor_ref);
		return p ? ((udev_monitor_ref_func*)p)(monitor) : NULL;
	}

	static
	udev_monitor*
	udev_monitor_unref(udev_monitor* monitor) {
		void* p = getFunc(FuncId_udev_monitor_unref);
		return p ? ((udev_monitor_unref_func*)p)(monitor) : NULL;
	}

	static
	udev*
	udev_monitor_get_udev(udev_monitor* monitor) {
		void* p = getFunc(FuncId_udev_monitor_get_udev);
		return p ? ((udev_monitor_get_udev_func*)p)(monitor) : NULL;
	}

	static
	int
	udev_monitor_get_fd(udev_monitor* monitor) {
		void* p = getFunc(FuncId_udev_monitor_get_fd);
		return p ? ((udev_monitor_get_fd_func*)p)(monitor) : -1;
	}

	static
	bool
	udev_monitor_filter_update_b(udev_monitor* monitor) {
		void* p = getFunc(FuncId_udev_monitor_filter_update);
		if (!p)
			return false;

		int result = ((udev_monitor_filter_update_func*)p)(monitor);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_filter_remove_b(udev_monitor* monitor) {
		void* p = getFunc(FuncId_udev_monitor_filter_remove);
		if (!p)
			return false;

		int result = ((udev_monitor_filter_remove_func*)p)(monitor);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_filter_add_match_subsystem_devtype_b(
		udev_monitor* monitor,
		const char* subsystem,
		const char* devtype
	) {
		void* p = getFunc(FuncId_udev_monitor_filter_add_match_subsystem_devtype);
		if (!p)
			return false;

		int result = ((udev_monitor_filter_add_match_subsystem_devtype_func*)p)(monitor, subsystem, devtype);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_filter_add_match_tag_b(
		udev_monitor* monitor,
		const char* tag
	) {
		void* p = getFunc(FuncId_udev_monitor_filter_add_match_tag);
		if (!p)
			return false;

		int result = ((udev_monitor_filter_add_match_tag_func*)p)(monitor, tag);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_set_receive_buffer_size_b(
		udev_monitor* monitor,
		int size
	) {
		void* p = getFunc(FuncId_udev_monitor_set_receive_buffer_size);
		if (!p)
			return false;

		int result = ((udev_monitor_set_receive_buffer_size_func*)p)(monitor, size);
		return completeUdevErrno(result);
	}

	static
	bool
	udev_monitor_enable_receiving_b(udev_monitor* monitor) {
		void* p = getFunc(FuncId_udev_monitor_enable_receiving);
		if (!p)
			return false;

		int result = ((udev_monitor_enable_receiving_func*)p)(monitor);
		return completeUdevErrno(result);
	}

	static
	udev_device*
	udev_monitor_receive_device(udev_monitor* monitor) {
		void* p = getFunc(FuncId_udev_monitor_receive_device);
		if (!p)
			return NULL;

		udev_device* device = ((udev_monitor_receive_device_func*)p)(monitor);
		return completeUdev(device, "udev_monitor_receive_device ");
	}

protected:
	static
	void*
	getFunc(FuncId id) {
		return sl::getSingleton<UdevLib>()->getFuncImpl(id);
	}

	void*
	getFuncImpl(FuncId id) {
		ASSERT((size_t)id < FuncId__Count);

		return m_funcTable[id] ?
			m_funcTable[id] :
			m_funcTable[id] = getFunction(m_funcNameTable[id]);
	}
};

//..............................................................................

// handle closing

template <typename T>
class UnrefUdevContext {
public:
void
operator () (udev* h) {
	T::udev_unref(h);
}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class UdevContextBase: public sl::Handle<udev*, UnrefUdevContext<T> > {
public:
UdevContextBase() {
	create();
}

UdevContextBase(udev* h) {
	this->m_h = h;
}

bool
create() {
	this->close();
	return this->m_h = T::udev_new();
}

void
addRef() {
	T::udev_ref(this->m_h);
}

udev_enumerate*
createEnumerator() {
	return T::udev_enumerate_new(this->m_h);
}

udev_monitor*
createMonitor(const sl::StringRef& name) {
	return T::udev_monitor_new_from_netlink(this->m_h, name.sz());
}
};

//..............................................................................

typedef UdevContextBase<Udev>    UdevContext_static;
typedef UdevDeviceBase<Udev>     UdevDevice_static;
typedef UdevEnumeratorBase<Udev> UdevEnumerator_static;
typedef UdevListEntryBase<Udev>  UdevListEntry_static;
typedef UdevMonitorBase<Udev>    UdevMonitor_static;
typedef UdevHwdbBase<Udev>       UdevHwdb_static;

typedef UdevContextBase<UdevLib>    UdevContext_dynamic;
typedef UdevDeviceBase<UdevLib>     UdevDevice_dynamic;
typedef UdevEnumeratorBase<UdevLib> UdevEnumerator_dynamic;
typedef UdevListEntryBase<UdevLib>  UdevListEntry_dynamic;
typedef UdevMonitorBase<UdevLib>    UdevMonitor_dynamic;
typedef UdevHwdbBase<UdevLib>       UdevHwdb_dynamic;

#if (_AXL_SYS_LNX_LIBUDEV_STATIC)
typedef UdevContext_static     UdevContext;
typedef UdevDevice_static      UdevDevice;
typedef UdevEnumerator_static  UdevEnumerator;
typedef UdevListEntry_static   UdevListEntry;
typedef UdevMonitor_static     UdevMonitor;
typedef UdevHwdb_static        UdevHwdb;
#else
typedef UdevContext_dynamic    UdevContext;
typedef UdevDevice_dynamic     UdevDevice;
typedef UdevEnumerator_dynamic UdevEnumerator;
typedef UdevListEntry_dynamic  UdevListEntry;
typedef UdevMonitor_dynamic    UdevMonitor;
typedef UdevHwdb_dynamic       UdevHwdb;
#endif

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
*/