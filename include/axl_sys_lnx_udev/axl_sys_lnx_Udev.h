
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
#include "axl_err_Errno.h"

namespace axl {
namespace sys {
namespace lnx {

// don't rely on the system header libudev.h and static linking to libudev.so
// instead, declare everything we need here and link to libudev.so dynamically
// to increase portablity among different linux distros

namespace libudev {

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//
// udev library context
//

typedef
udev*
udev_ref_t(udev* ctx);

typedef
udev*
udev_unref_t(udev* ctx);

typedef
udev*
udev_new_t();

typedef
void*
udev_get_userdata_t(udev* ctx);

typedef
void
udev_set_userdata_t(
	udev* ctx,
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
udev_device_ref_t(udev_device* device);

typedef
udev_device*
udev_device_unref_t(udev_device* device);

typedef
udev*
udev_device_get_udev_t(udev_device* device);

typedef
udev_device*
udev_device_new_from_syspath_t(
	udev* ctx,
	const char* syspath
);

typedef
udev_device*
udev_device_new_from_devnum_t(
	udev* ctx,
	char type,
	dev_t devnum
);

typedef
udev_device*
udev_device_new_from_subsystem_sysname_t(
	udev* ctx,
	const char* subsystem,
	const char* sysname
);

typedef
udev_device*
udev_device_new_from_device_id_t(
	udev* ctx,
	const char* id
);

typedef
udev_device*
udev_device_new_from_environment_t(udev* ctx);

typedef
udev_device*
udev_device_get_parent_t(udev_device* device);

typedef
udev_device*
udev_device_get_parent_with_subsystem_devtype_t(
	udev_device* device,
	const char* subsystem,
	const char* devtype
);

typedef
const char*
udev_device_get_devpath_t(udev_device* device);

typedef
const char*
udev_device_get_subsystem_t(udev_device* device);

typedef
const char*
udev_device_get_devtype_t(udev_device* device);

typedef
const char*
udev_device_get_syspath_t(udev_device* device);

typedef
const char*
udev_device_get_sysname_t(udev_device* device);

typedef
const char*
udev_device_get_sysnum_t(udev_device* device);

typedef
const char*
udev_device_get_devnode_t(udev_device* device);

typedef
int
udev_device_get_is_initialized_t(udev_device* device);

typedef
udev_list_entry*
udev_device_get_devlinks_list_entry_t(udev_device* device);

typedef
udev_list_entry*
udev_device_get_properties_list_entry_t(udev_device* device);

typedef
udev_list_entry*
udev_device_get_tags_list_entry_t(udev_device* device);

typedef
udev_list_entry*
udev_device_get_current_tags_list_entry_t(udev_device* device);

typedef
udev_list_entry*
udev_device_get_sysattr_list_entry_t(udev_device* device);

typedef
const char*
udev_device_get_property_value_t(
	udev_device* device,
	const char* key
);

typedef
const char*
udev_device_get_driver_t(udev_device* device);

typedef
dev_t
udev_device_get_devnum_t(udev_device* device);

typedef
const char*
udev_device_get_action_t(udev_device* device);

typedef
unsigned long long int
udev_device_get_seqnum_t(udev_device* device);

typedef
unsigned long long int
udev_device_get_usec_since_initialized_t(udev_device* device);

typedef
const char*
udev_device_get_sysattr_value_t(
	udev_device* device,
	const char* sysattr
);

typedef
int
udev_device_set_sysattr_value_t(
	udev_device* device,
	const char* sysattr,
	const char* value
);

typedef
int
udev_device_has_tag_t(
	udev_device* device,
	const char* tag
);

typedef
int
udev_device_has_current_tag_t(
	udev_device* device,
	const char* tag
);

//
// udev_monitor
//

typedef
udev_monitor*
udev_monitor_ref_t(udev_monitor* monitor);

typedef
udev_monitor*
udev_monitor_unref_t(udev_monitor* monitor);

typedef
udev*
udev_monitor_get_udev_t(udev_monitor* monitor);

typedef
udev_monitor*
udev_monitor_new_from_netlink_t(
	udev* ctx,
	const char* name
);

typedef
int
udev_monitor_enable_receiving_t(udev_monitor* monitor);

typedef
int
udev_monitor_set_receive_buffer_size_t(
	udev_monitor* monitor,
	int size
);

typedef
int
udev_monitor_get_fd_t(udev_monitor* monitor);

typedef
udev_device*
udev_monitor_receive_device_t(udev_monitor* monitor);

typedef
int
udev_monitor_filter_add_match_subsystem_devtype_t(
	udev_monitor* monitor,
	const char* subsystem,
	const char* devtype
);

typedef
int
udev_monitor_filter_add_match_tag_t(
	udev_monitor* monitor,
	const char* tag
);

typedef
int
udev_monitor_filter_update_t(udev_monitor* monitor);

typedef
int
udev_monitor_filter_remove_t(udev_monitor* monitor);

//
// udev_enumerate
//

typedef
udev_enumerate*
udev_enumerate_ref_t(udev_enumerate* enumerate);

typedef
udev_enumerate*
udev_enumerate_unref_t(udev_enumerate* enumerate);

typedef
udev*
udev_enumerate_get_udev_t(udev_enumerate* enumerate);

typedef
udev_enumerate*
udev_enumerate_new_t(udev* ctx);

typedef
int
udev_enumerate_add_match_subsystem_t(
	udev_enumerate* enumerate,
	const char* subsystem
);

typedef
int
udev_enumerate_add_nomatch_subsystem_t(
	udev_enumerate* enumerate,
	const char* subsystem
);

typedef
int
udev_enumerate_add_match_sysattr_t(
	udev_enumerate* enumerate,
	const char* sysattr,
	const char* value
);

typedef
int
udev_enumerate_add_nomatch_sysattr_t(
	udev_enumerate* enumerate,
	const char* sysattr,
	const char* value
);

typedef
int
udev_enumerate_add_match_property_t(
	udev_enumerate* enumerate,
	const char* property,
	const char* value
);

typedef
int
udev_enumerate_add_match_sysname_t(
	udev_enumerate* enumerate,
	const char* sysname
);

typedef
int
udev_enumerate_add_match_tag_t(
	udev_enumerate* enumerate,
	const char* tag
);

typedef
int
udev_enumerate_add_match_parent_t(
	udev_enumerate* enumerate,
	udev_device* parent
);

typedef
int
udev_enumerate_add_match_is_initialized_t(udev_enumerate* enumerate);

typedef
int
udev_enumerate_add_syspath_t(
	udev_enumerate* enumerate,
	const char* syspath
);

typedef
int
udev_enumerate_scan_devices_t(udev_enumerate* enumerate);

typedef
int
udev_enumerate_scan_subsystems_t(udev_enumerate* enumerate);

typedef
udev_list_entry*
udev_enumerate_get_list_entry_t(udev_enumerate* enumerate);

//
// udev_queue
//

typedef
udev_queue*
udev_queue_ref_t(udev_queue* queue);

typedef
udev_queue*
udev_queue_unref_t(udev_queue* queue);

typedef
udev*
udev_queue_get_udev_t(udev_queue* queue);

typedef
udev_queue*
udev_queue_new_t(udev* ctx);

typedef
int
udev_queue_get_udev_is_active_t(udev_queue* queue);

typedef
int
udev_queue_get_queue_is_empty_t(udev_queue* queue);

typedef
int
udev_queue_get_fd_t(udev_queue* queue);

typedef
int
udev_queue_flush_t(udev_queue* queue);

//
// udev_hwdb
//

typedef
udev_hwdb*
udev_hwdb_ref_t(udev_hwdb* hwdb);

typedef
udev_hwdb*
udev_hwdb_unref_t(udev_hwdb* hwdb);

typedef
udev_hwdb*
udev_hwdb_new_t(udev* ctx);

typedef
udev_list_entry*
udev_hwdb_get_properties_list_entry_t(
	udev_hwdb* hwdb,
	const char* modalias,
	unsigned flags
);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

extern udev_ref_t* udev_ref;
extern udev_unref_t* udev_unref;
extern udev_new_t* udev_new;
extern udev_get_userdata_t* udev_get_userdata;
extern udev_set_userdata_t* udev_set_userdata;

extern udev_list_entry_get_next_t* udev_list_entry_get_next;
extern udev_list_entry_get_by_name_t* udev_list_entry_get_by_name;
extern udev_list_entry_get_name_t* udev_list_entry_get_name;
extern udev_list_entry_get_value_t* udev_list_entry_get_value;

extern udev_device_ref_t* udev_device_ref;
extern udev_device_unref_t* udev_device_unref;
extern udev_device_get_udev_t* udev_device_get_udev;
extern udev_device_new_from_syspath_t* udev_device_new_from_syspath;
extern udev_device_new_from_devnum_t* udev_device_new_from_devnum;
extern udev_device_new_from_subsystem_sysname_t* udev_device_new_from_subsystem_sysname;
extern udev_device_new_from_device_id_t* udev_device_new_from_device_id;
extern udev_device_new_from_environment_t* udev_device_new_from_environment;
extern udev_device_get_parent_t* udev_device_get_parent;
extern udev_device_get_parent_with_subsystem_devtype_t* udev_device_get_parent_with_subsystem_devtype;
extern udev_device_get_devpath_t* udev_device_get_devpath;
extern udev_device_get_subsystem_t* udev_device_get_subsystem;
extern udev_device_get_devtype_t* udev_device_get_devtype;
extern udev_device_get_syspath_t* udev_device_get_syspath;
extern udev_device_get_sysname_t* udev_device_get_sysname;
extern udev_device_get_sysnum_t* udev_device_get_sysnum;
extern udev_device_get_devnode_t* udev_device_get_devnode;
extern udev_device_get_is_initialized_t* udev_device_get_is_initialized;
extern udev_device_get_devlinks_list_entry_t* udev_device_get_devlinks_list_entry;
extern udev_device_get_properties_list_entry_t* udev_device_get_properties_list_entry;
extern udev_device_get_tags_list_entry_t* udev_device_get_tags_list_entry;
extern udev_device_get_current_tags_list_entry_t* udev_device_get_current_tags_list_entry;
extern udev_device_get_sysattr_list_entry_t* udev_device_get_sysattr_list_entry;
extern udev_device_get_property_value_t* udev_device_get_property_value;
extern udev_device_get_driver_t* udev_device_get_driver;
extern udev_device_get_devnum_t* udev_device_get_devnum;
extern udev_device_get_action_t* udev_device_get_action;
extern udev_device_get_seqnum_t* udev_device_get_seqnum;
extern udev_device_get_usec_since_initialized_t* udev_device_get_usec_since_initialized;
extern udev_device_get_sysattr_value_t* udev_device_get_sysattr_value;
extern udev_device_set_sysattr_value_t* udev_device_set_sysattr_value;
extern udev_device_has_tag_t* udev_device_has_tag;
extern udev_device_has_current_tag_t* udev_device_has_current_tag;

extern udev_monitor_ref_t* udev_monitor_ref;
extern udev_monitor_unref_t* udev_monitor_unref;
extern udev_monitor_get_udev_t* udev_monitor_get_udev;
extern udev_monitor_new_from_netlink_t* udev_monitor_new_from_netlink;
extern udev_monitor_enable_receiving_t* udev_monitor_enable_receiving;
extern udev_monitor_set_receive_buffer_size_t* udev_monitor_set_receive_buffer_size;
extern udev_monitor_get_fd_t* udev_monitor_get_fd;
extern udev_monitor_receive_device_t* udev_monitor_receive_device;
extern udev_monitor_filter_add_match_subsystem_devtype_t* udev_monitor_filter_add_match_subsystem_devtype;
extern udev_monitor_filter_add_match_tag_t* udev_monitor_filter_add_match_tag;
extern udev_monitor_filter_update_t* udev_monitor_filter_update;
extern udev_monitor_filter_remove_t* udev_monitor_filter_remove;

extern udev_enumerate_ref_t* udev_enumerate_ref;
extern udev_enumerate_unref_t* udev_enumerate_unref;
extern udev_enumerate_get_udev_t* udev_enumerate_get_udev;
extern udev_enumerate_new_t* udev_enumerate_new;
extern udev_enumerate_add_match_subsystem_t* udev_enumerate_add_match_subsystem;
extern udev_enumerate_add_nomatch_subsystem_t* udev_enumerate_add_nomatch_subsystem;
extern udev_enumerate_add_match_sysattr_t* udev_enumerate_add_match_sysattr;
extern udev_enumerate_add_nomatch_sysattr_t* udev_enumerate_add_nomatch_sysattr;
extern udev_enumerate_add_match_property_t* udev_enumerate_add_match_property;
extern udev_enumerate_add_match_sysname_t* udev_enumerate_add_match_sysname;
extern udev_enumerate_add_match_tag_t* udev_enumerate_add_match_tag;
extern udev_enumerate_add_match_parent_t* udev_enumerate_add_match_parent;
extern udev_enumerate_add_match_is_initialized_t* udev_enumerate_add_match_is_initialized;
extern udev_enumerate_add_syspath_t* udev_enumerate_add_syspath;
extern udev_enumerate_scan_devices_t* udev_enumerate_scan_devices;
extern udev_enumerate_scan_subsystems_t* udev_enumerate_scan_subsystems;
extern udev_enumerate_get_list_entry_t* udev_enumerate_get_list_entry;

extern udev_queue_ref_t* udev_queue_ref;
extern udev_queue_unref_t* udev_queue_unref;
extern udev_queue_get_udev_t* udev_queue_get_udev;
extern udev_queue_new_t* udev_queue_new;
extern udev_queue_get_udev_is_active_t* udev_queue_get_udev_is_active;
extern udev_queue_get_queue_is_empty_t* udev_queue_get_queue_is_empty;
extern udev_queue_get_fd_t* udev_queue_get_fd;
extern udev_queue_flush_t* udev_queue_flush;

extern udev_hwdb_ref_t* udev_hwdb_ref;
extern udev_hwdb_unref_t* udev_hwdb_unref;
extern udev_hwdb_new_t* udev_hwdb_new;
extern udev_hwdb_get_properties_list_entry_t* udev_hwdb_get_properties_list_entry;

//..............................................................................

} // namespace libudev

// unfortunately, libudev does not set errno on failures

inline
bool
failWithUdevError(const char* function) {
	err::setFormatStringError("libudev failed: %s", function);
	return false;
}

inline
bool
completeUdev(
	bool result,
	const char* function
) {
	return result ? true : failWithUdevError(function);
}

inline
bool
completeUdevErrno(int result) {
	if (result < 0) {
		err::setError(err::Errno(-result));
		return false;
	}

	return true;
}

//..............................................................................

using namespace libudev;

class UnrefUdev {
public:
	void
	operator () (udev* h) {
		udev_unref(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Udev: public sl::Handle<udev*, UnrefUdev> {
public:
	Udev() {}

	Udev(udev* ctx) {
		if (m_h = ctx)
			ref();
	}

	void
	ref() {
		udev_ref(m_h);
	}

	void
	unref() {
		udev_unref(m_h);
	}

	bool
	create() {
		attach(udev_new());
		return completeUdev(m_h, "udev_new");
	}
};

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
