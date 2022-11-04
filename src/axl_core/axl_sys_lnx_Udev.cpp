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
#include "axl_sys_lnx_Udev.h"
#include "axl_sys_psx_DynamicLib.h"
#include "axl_sl_Singleton.h"

namespace axl {
namespace sys {
namespace lnx {
namespace libudev {

//..............................................................................

class UdevLib: public psx::DynamicLib {
public:
	UdevLib() {
		open("libudev.so") ||
		open("libudev.so.1") ||
		open("libudev.so.0");
	}
};

inline
void*
loadFunc(const char* name) {
	UdevLib* lib = sl::getSingleton<UdevLib>();
	return ::dlsym(*lib, name);
}

//..............................................................................

//
// udev library context
//

udev*
fail_udev_ref(udev* ctx) {
	return NULL;
}

udev*
fail_udev_unref(udev* ctx) {
	return NULL;
}

udev*
fail_udev_new() {
	return NULL;
}

void*
fail_udev_get_userdata(udev* ctx) {
	return NULL;
}

void
fail_udev_set_userdata(
	udev* ctx,
	void* userdata
) {
}

//
// udev_list
//

udev_list_entry*
fail_udev_list_entry_get_next(udev_list_entry* list_entry) {
	return NULL;
}

udev_list_entry*
fail_udev_list_entry_get_by_name(
	udev_list_entry* list_entry,
	const char* name
) {
	return NULL;
}

const char*
fail_udev_list_entry_get_name(udev_list_entry* list_entry) {
	return NULL;
}

const char*
fail_udev_list_entry_get_value(udev_list_entry* list_entry) {
	return NULL;
}

//
// udev_device
//

udev_device*
fail_udev_device_ref(udev_device* device) {
	return NULL;
}

udev_device*
fail_udev_device_unref(udev_device* device) {
	return NULL;
}

udev*
fail_udev_device_get_udev(udev_device* device) {
	return NULL;
}

udev_device*
fail_udev_device_new_from_syspath(
	udev* ctx,
	const char* syspath
) {
	return NULL;
}

udev_device*
fail_udev_device_new_from_devnum(
	udev* ctx,
	char type,
	dev_t devnum
) {
	return NULL;
}

udev_device*
fail_udev_device_new_from_subsystem_sysname(
	udev* ctx,
	const char* subsystem,
	const char* sysname
) {
	return NULL;
}

udev_device*
fail_udev_device_new_from_device_id(
	udev* ctx,
	const char* id
) {
	return NULL;
}

udev_device*
fail_udev_device_new_from_environment(udev* ctx) {
	return NULL;
}

udev_device*
fail_udev_device_get_parent(udev_device* device) {
	return NULL;
}

udev_device*
fail_udev_device_get_parent_with_subsystem_devtype(
	udev_device* device,
	const char* subsystem,
	const char* devtype
) {
	return NULL;
}

const char*
fail_udev_device_get_devpath(udev_device* device) {
	return NULL;
}

const char*
fail_udev_device_get_subsystem(udev_device* device) {
	return NULL;
}

const char*
fail_udev_device_get_devtype(udev_device* device) {
	return NULL;
}

const char*
fail_udev_device_get_syspath(udev_device* device) {
	return NULL;
}

const char*
fail_udev_device_get_sysname(udev_device* device) {
	return NULL;
}

const char*
fail_udev_device_get_sysnum(udev_device* device) {
	return NULL;
}

const char*
fail_udev_device_get_devnode(udev_device* device) {
	return NULL;
}

int
fail_udev_device_get_is_initialized(udev_device* device) {
	return -ENOENT;
}

udev_list_entry*
fail_udev_device_get_devlinks_list_entry(udev_device* device) {
	return NULL;
}

udev_list_entry*
fail_udev_device_get_properties_list_entry(udev_device* device) {
	return NULL;
}

udev_list_entry*
fail_udev_device_get_tags_list_entry(udev_device* device) {
	return NULL;
}

udev_list_entry*
fail_udev_device_get_current_tags_list_entry(udev_device* device) {
	return NULL;
}

udev_list_entry*
fail_udev_device_get_sysattr_list_entry(udev_device* device) {
	return NULL;
}

const char*
fail_udev_device_get_property_value(
	udev_device* device,
	const char* key
) {
	return NULL;
}

const char*
fail_udev_device_get_driver(udev_device* device) {
	return NULL;
}

dev_t
fail_udev_device_get_devnum(udev_device* device) {
	return 0;
}

const char*
fail_udev_device_get_action(udev_device* device) {
	return NULL;
}

unsigned long long int
fail_udev_device_get_seqnum(udev_device* device) {
	return -ENOENT;
}

unsigned long long int
fail_udev_device_get_usec_since_initialized(udev_device* device) {
	return -ENOENT;
}

const char*
fail_udev_device_get_sysattr_value(
	udev_device* device,
	const char* sysattr
) {
	return NULL;
}

int
fail_udev_device_set_sysattr_value(
	udev_device* device,
	const char* sysattr,
	const char* value
) {
	return -ENOENT;
}

int
fail_udev_device_has_tag(
	udev_device* device,
	const char* tag
) {
	return -ENOENT;
}

int
fail_udev_device_has_current_tag(
	udev_device* device,
	const char* tag
) {
	return -ENOENT;
}

//
// udev_monitor
//

udev_monitor*
fail_udev_monitor_ref(udev_monitor* monitor) {
	return NULL;
}

udev_monitor*
fail_udev_monitor_unref(udev_monitor* monitor) {
	return NULL;
}

udev*
fail_udev_monitor_get_udev(udev_monitor* monitor) {
	return NULL;
}

udev_monitor*
fail_udev_monitor_new_from_netlink(
	udev* ctx,
	const char* name
) {
	return NULL;
}

int
fail_udev_monitor_enable_receiving(udev_monitor* monitor) {
	return -ENOENT;
}

int
fail_udev_monitor_set_receive_buffer_size(
	udev_monitor* monitor,
	int size
) {
	return -ENOENT;
}

int
fail_udev_monitor_get_fd(udev_monitor* monitor) {
	return -ENOENT;
}

udev_device*
fail_udev_monitor_receive_device(udev_monitor* monitor) {
	return NULL;
}

int
fail_udev_monitor_filter_add_match_subsystem_devtype(
	udev_monitor* monitor,
	const char* subsystem,
	const char* devtype
) {
	return -ENOENT;
}

int
fail_udev_monitor_filter_add_match_tag(
	udev_monitor* monitor,
	const char* tag
) {
	return -ENOENT;
}

int
fail_udev_monitor_filter_update(udev_monitor* monitor) {
	return -ENOENT;
}

int
fail_udev_monitor_filter_remove(udev_monitor* monitor) {
	return -ENOENT;
}

//
// udev_enumerate
//

udev_enumerate*
fail_udev_enumerate_ref(udev_enumerate* enumerate) {
	return NULL;
}

udev_enumerate*
fail_udev_enumerate_unref(udev_enumerate* enumerate) {
	return NULL;
}

udev*
fail_udev_enumerate_get_udev(udev_enumerate* enumerate) {
	return NULL;
}

udev_enumerate*
fail_udev_enumerate_new(udev* ctx) {
	return NULL;
}

int
fail_udev_enumerate_add_match_subsystem(
	udev_enumerate* enumerate,
	const char* subsystem
) {
	return -ENOENT;
}

int
fail_udev_enumerate_add_nomatch_subsystem(
	udev_enumerate* enumerate,
	const char* subsystem
) {
	return -ENOENT;
}

int
fail_udev_enumerate_add_match_sysattr(
	udev_enumerate* enumerate,
	const char* sysattr,
	const char* value
) {
	return -ENOENT;
}

int
fail_udev_enumerate_add_nomatch_sysattr(
	udev_enumerate* enumerate,
	const char* sysattr,
	const char* value
) {
	return -ENOENT;
}

int
fail_udev_enumerate_add_match_property(
	udev_enumerate* enumerate,
	const char* property,
	const char* value
) {
	return -ENOENT;
}

int
fail_udev_enumerate_add_match_sysname(
	udev_enumerate* enumerate,
	const char* sysname
) {
	return -ENOENT;
}

int
fail_udev_enumerate_add_match_tag(
	udev_enumerate* enumerate,
	const char* tag
) {
	return -ENOENT;
}

int
fail_udev_enumerate_add_match_parent(
	udev_enumerate* enumerate,
	udev_device* parent
) {
	return -ENOENT;
}

int
fail_udev_enumerate_add_match_is_initialized(udev_enumerate* enumerate) {
	return -ENOENT;
}

int
fail_udev_enumerate_add_syspath(
	udev_enumerate* enumerate,
	const char* syspath
) {
	return -ENOENT;
}

int
fail_udev_enumerate_scan_devices(udev_enumerate* enumerate) {
	return -ENOENT;
}

int
fail_udev_enumerate_scan_subsystems(udev_enumerate* enumerate) {
	return -ENOENT;
}

udev_list_entry*
fail_udev_enumerate_get_list_entry(udev_enumerate* enumerate) {
	return NULL;
}

//
// udev_queue
//

udev_queue*
fail_udev_queue_ref(udev_queue* queue) {
	return NULL;
}

udev_queue*
fail_udev_queue_unref(udev_queue* queue) {
	return NULL;
}

udev*
fail_udev_queue_get_udev(udev_queue* queue) {
	return NULL;
}

udev_queue*
fail_udev_queue_new(udev* ctx) {
	return NULL;
}

int
fail_udev_queue_get_udev_is_active(udev_queue* queue) {
	return -ENOENT;
}

int
fail_udev_queue_get_queue_is_empty(udev_queue* queue) {
	return -ENOENT;
}

int
fail_udev_queue_get_fd(udev_queue* queue) {
	return -ENOENT;
}

int
fail_udev_queue_flush(udev_queue* queue) {
	return -ENOENT;
}

//
// udev_hwdb
//

udev_hwdb*
fail_udev_hwdb_new(udev* ctx) {
	return NULL;
}

udev_hwdb*
fail_udev_hwdb_ref(udev_hwdb* hwdb) {
	return NULL;
}

udev_hwdb*
fail_udev_hwdb_unref(udev_hwdb* hwdb) {
	return NULL;
}

udev_list_entry*
fail_udev_hwdb_get_properties_list_entry(
	udev_hwdb* hwdb,
	const char* modalias,
	unsigned flags
) {
	return NULL;
}

//..............................................................................

//
// udev library context
//

udev*
load_udev_ref(udev* ctx) {
	void* p = loadFunc("udev_ref");
	udev_ref = p ? (udev_ref_t*)p : fail_udev_ref;
	return udev_ref(ctx);
}

udev*
load_udev_unref(udev* ctx) {
	void* p = loadFunc("udev_unref");
	udev_unref = p ? (udev_unref_t*)p : fail_udev_unref;
	return udev_unref(ctx);
}

udev*
load_udev_new() {
	void* p = loadFunc("udev_new");
	udev_new = p ? (udev_new_t*)p : fail_udev_new;
	return udev_new();
}

void*
load_udev_get_userdata(udev* ctx) {
	void* p = loadFunc("udev_get_userdata");
	udev_get_userdata = p ? (udev_get_userdata_t*)p : fail_udev_get_userdata;
	return udev_get_userdata(ctx);
}

void
load_udev_set_userdata(
	udev* ctx,
	void* userdata
) {
	void* p = loadFunc("udev_set_userdata");
	udev_set_userdata = p ? (udev_set_userdata_t*)p : fail_udev_set_userdata;
	return udev_set_userdata(ctx, userdata);
}

//
// udev_list
//

udev_list_entry*
load_udev_list_entry_get_next(udev_list_entry* list_entry) {
	void* p = loadFunc("udev_list_entry_get_next");
	udev_list_entry_get_next = p ? (udev_list_entry_get_next_t*)p : fail_udev_list_entry_get_next;
	return udev_list_entry_get_next(list_entry);
}

udev_list_entry*
load_udev_list_entry_get_by_name(
	udev_list_entry* list_entry,
	const char* name
) {
	void* p = loadFunc("udev_list_entry_get_by_name");
	udev_list_entry_get_by_name = p ? (udev_list_entry_get_by_name_t*)p : fail_udev_list_entry_get_by_name;
	return udev_list_entry_get_by_name(list_entry, name);
}

const char*
load_udev_list_entry_get_name(udev_list_entry* list_entry) {
	void* p = loadFunc("udev_list_entry_get_name");
	udev_list_entry_get_name = p ? (udev_list_entry_get_name_t*)p : fail_udev_list_entry_get_name;
	return udev_list_entry_get_name(list_entry);
}

const char*
load_udev_list_entry_get_value(udev_list_entry* list_entry) {
	void* p = loadFunc("udev_list_entry_get_value");
	udev_list_entry_get_value = p ? (udev_list_entry_get_value_t*)p : fail_udev_list_entry_get_value;
	return udev_list_entry_get_value(list_entry);
}

//
// udev_device
//

udev_device*
load_udev_device_ref(udev_device* device) {
	void* p = loadFunc("udev_device_ref");
	udev_device_ref = p ? (udev_device_ref_t*)p : fail_udev_device_ref;
	return udev_device_ref(device);
}

udev_device*
load_udev_device_unref(udev_device* device) {
	void* p = loadFunc("udev_device_unref");
	udev_device_unref = p ? (udev_device_unref_t*)p : fail_udev_device_unref;
	return udev_device_unref(device);
}

udev*
load_udev_device_get_udev(udev_device* device) {
	void* p = loadFunc("udev_device_get_udev");
	udev_device_get_udev = p ? (udev_device_get_udev_t*)p : fail_udev_device_get_udev;
	return udev_device_get_udev(device);
}

udev_device*
load_udev_device_new_from_syspath(
	udev* ctx,
	const char* syspath
) {
	void* p = loadFunc("udev_device_new_from_syspath");
	udev_device_new_from_syspath = p ? (udev_device_new_from_syspath_t*)p : fail_udev_device_new_from_syspath;
	return udev_device_new_from_syspath(ctx, syspath);
}

udev_device*
load_udev_device_new_from_devnum(
	udev* ctx,
	char type,
	dev_t devnum
) {
	void* p = loadFunc("udev_device_new_from_devnum");
	udev_device_new_from_devnum = p ? (udev_device_new_from_devnum_t*)p : fail_udev_device_new_from_devnum;
	return udev_device_new_from_devnum(ctx, type, devnum);
}

udev_device*
load_udev_device_new_from_subsystem_sysname(
	udev* ctx,
	const char* subsystem,
	const char* sysname
) {
	void* p = loadFunc("udev_device_new_from_subsystem_sysname");
	udev_device_new_from_subsystem_sysname = p ? (udev_device_new_from_subsystem_sysname_t*)p : fail_udev_device_new_from_subsystem_sysname;
	return udev_device_new_from_subsystem_sysname(ctx, subsystem, sysname);
}

udev_device*
load_udev_device_new_from_device_id(
	udev* ctx,
	const char* id
) {
	void* p = loadFunc("udev_device_new_from_device_id");
	udev_device_new_from_device_id = p ? (udev_device_new_from_device_id_t*)p : fail_udev_device_new_from_device_id;
	return udev_device_new_from_device_id(ctx, id);
}

udev_device*
load_udev_device_new_from_environment(udev* ctx) {
	void* p = loadFunc("udev_device_new_from_environment");
	udev_device_new_from_environment = p ? (udev_device_new_from_environment_t*)p : fail_udev_device_new_from_environment;
	return udev_device_new_from_environment(ctx);
}

udev_device*
load_udev_device_get_parent(udev_device* device) {
	void* p = loadFunc("udev_device_get_parent");
	udev_device_get_parent = p ? (udev_device_get_parent_t*)p : fail_udev_device_get_parent;
	return udev_device_get_parent(device);
}

udev_device*
load_udev_device_get_parent_with_subsystem_devtype(
	udev_device* device,
	const char* subsystem,
	const char* devtype
) {
	void* p = loadFunc("udev_device_get_parent_with_subsystem_devtype");
	udev_device_get_parent_with_subsystem_devtype = p ? (udev_device_get_parent_with_subsystem_devtype_t*)p : fail_udev_device_get_parent_with_subsystem_devtype;
	return udev_device_get_parent_with_subsystem_devtype(device, subsystem, devtype);
}

const char*
load_udev_device_get_devpath(udev_device* device) {
	void* p = loadFunc("udev_device_get_devpath");
	udev_device_get_devpath = p ? (udev_device_get_devpath_t*)p : fail_udev_device_get_devpath;
	return udev_device_get_devpath(device);
}

const char*
load_udev_device_get_subsystem(udev_device* device) {
	void* p = loadFunc("udev_device_get_subsystem");
	udev_device_get_subsystem = p ? (udev_device_get_subsystem_t*)p : fail_udev_device_get_subsystem;
	return udev_device_get_subsystem(device);
}

const char*
load_udev_device_get_devtype(udev_device* device) {
	void* p = loadFunc("udev_device_get_devtype");
	udev_device_get_devtype = p ? (udev_device_get_devtype_t*)p : fail_udev_device_get_devtype;
	return udev_device_get_devtype(device);
}

const char*
load_udev_device_get_syspath(udev_device* device) {
	void* p = loadFunc("udev_device_get_syspath");
	udev_device_get_syspath = p ? (udev_device_get_syspath_t*)p : fail_udev_device_get_syspath;
	return udev_device_get_syspath(device);
}

const char*
load_udev_device_get_sysname(udev_device* device) {
	void* p = loadFunc("udev_device_get_sysname");
	udev_device_get_sysname = p ? (udev_device_get_sysname_t*)p : fail_udev_device_get_sysname;
	return udev_device_get_sysname(device);
}

const char*
load_udev_device_get_sysnum(udev_device* device) {
	void* p = loadFunc("udev_device_get_sysnum");
	udev_device_get_sysnum = p ? (udev_device_get_sysnum_t*)p : fail_udev_device_get_sysnum;
	return udev_device_get_sysnum(device);
}

const char*
load_udev_device_get_devnode(udev_device* device) {
	void* p = loadFunc("udev_device_get_devnode");
	udev_device_get_devnode = p ? (udev_device_get_devnode_t*)p : fail_udev_device_get_devnode;
	return udev_device_get_devnode(device);
}

int
load_udev_device_get_is_initialized(udev_device* device) {
	void* p = loadFunc("udev_device_get_is_initialized");
	udev_device_get_is_initialized = p ? (udev_device_get_is_initialized_t*)p : fail_udev_device_get_is_initialized;
	return udev_device_get_is_initialized(device);
}

udev_list_entry*
load_udev_device_get_devlinks_list_entry(udev_device* device) {
	void* p = loadFunc("udev_device_get_devlinks_list_entry");
	udev_device_get_devlinks_list_entry = p ? (udev_device_get_devlinks_list_entry_t*)p : fail_udev_device_get_devlinks_list_entry;
	return udev_device_get_devlinks_list_entry(device);
}

udev_list_entry*
load_udev_device_get_properties_list_entry(udev_device* device) {
	void* p = loadFunc("udev_device_get_properties_list_entry");
	udev_device_get_properties_list_entry = p ? (udev_device_get_properties_list_entry_t*)p : fail_udev_device_get_properties_list_entry;
	return udev_device_get_properties_list_entry(device);
}

udev_list_entry*
load_udev_device_get_tags_list_entry(udev_device* device) {
	void* p = loadFunc("udev_device_get_tags_list_entry");
	udev_device_get_tags_list_entry = p ? (udev_device_get_tags_list_entry_t*)p : fail_udev_device_get_tags_list_entry;
	return udev_device_get_tags_list_entry(device);
}

udev_list_entry*
load_udev_device_get_current_tags_list_entry(udev_device* device) {
	void* p = loadFunc("udev_device_get_current_tags_list_entry");
	udev_device_get_current_tags_list_entry = p ? (udev_device_get_current_tags_list_entry_t*)p : fail_udev_device_get_current_tags_list_entry;
	return udev_device_get_current_tags_list_entry(device);
}

udev_list_entry*
load_udev_device_get_sysattr_list_entry(udev_device* device) {
	void* p = loadFunc("udev_device_get_sysattr_list_entry");
	udev_device_get_sysattr_list_entry = p ? (udev_device_get_sysattr_list_entry_t*)p : fail_udev_device_get_sysattr_list_entry;
	return udev_device_get_sysattr_list_entry(device);
}

const char*
load_udev_device_get_property_value(
	udev_device* device,
	const char* key
) {
	void* p = loadFunc("udev_device_get_property_value");
	udev_device_get_property_value = p ? (udev_device_get_property_value_t*)p : fail_udev_device_get_property_value;
	return udev_device_get_property_value(device, key);
}

const char*
load_udev_device_get_driver(udev_device* device) {
	void* p = loadFunc("udev_device_get_driver");
	udev_device_get_driver = p ? (udev_device_get_driver_t*)p : fail_udev_device_get_driver;
	return udev_device_get_driver(device);
}

dev_t
load_udev_device_get_devnum(udev_device* device) {
	void* p = loadFunc("udev_device_get_devnum");
	udev_device_get_devnum = p ? (udev_device_get_devnum_t*)p : fail_udev_device_get_devnum;
	return udev_device_get_devnum(device);
}

const char*
load_udev_device_get_action(udev_device* device) {
	void* p = loadFunc("udev_device_get_action");
	udev_device_get_action = p ? (udev_device_get_action_t*)p : fail_udev_device_get_action;
	return udev_device_get_action(device);
}

unsigned long long int
load_udev_device_get_seqnum(udev_device* device) {
	void* p = loadFunc("udev_device_get_seqnum");
	udev_device_get_seqnum = p ? (udev_device_get_seqnum_t*)p : fail_udev_device_get_seqnum;
	return udev_device_get_seqnum(device);
}

unsigned long long int
load_udev_device_get_usec_since_initialized(udev_device* device) {
	void* p = loadFunc("udev_device_get_usec_since_initialized");
	udev_device_get_usec_since_initialized = p ? (udev_device_get_usec_since_initialized_t*)p : fail_udev_device_get_usec_since_initialized;
	return udev_device_get_usec_since_initialized(device);
}

const char*
load_udev_device_get_sysattr_value(
	udev_device* device,
	const char* sysattr
) {
	void* p = loadFunc("udev_device_get_sysattr_value");
	udev_device_get_sysattr_value = p ? (udev_device_get_sysattr_value_t*)p : fail_udev_device_get_sysattr_value;
	return udev_device_get_sysattr_value(device, sysattr);
}

int
load_udev_device_set_sysattr_value(
	udev_device* device,
	const char* sysattr,
	const char* value
) {
	void* p = loadFunc("udev_device_set_sysattr_value");
	udev_device_set_sysattr_value = p ? (udev_device_set_sysattr_value_t*)p : fail_udev_device_set_sysattr_value;
	return udev_device_set_sysattr_value(device, sysattr, value);
}

int
load_udev_device_has_tag(
	udev_device* device,
	const char* tag
) {
	void* p = loadFunc("udev_device_has_tag");
	udev_device_has_tag = p ? (udev_device_has_tag_t*)p : fail_udev_device_has_tag;
	return udev_device_has_tag(device, tag);
}

int
load_udev_device_has_current_tag(
	udev_device* device,
	const char* tag
) {
	void* p = loadFunc("udev_device_has_current_tag");
	udev_device_has_current_tag = p ? (udev_device_has_current_tag_t*)p : fail_udev_device_has_current_tag;
	return udev_device_has_current_tag(device, tag);
}

//
// udev_monitor
//

udev_monitor*
load_udev_monitor_ref(udev_monitor* monitor) {
	void* p = loadFunc("udev_monitor_ref");
	udev_monitor_ref = p ? (udev_monitor_ref_t*)p : fail_udev_monitor_ref;
	return udev_monitor_ref(monitor);
}

udev_monitor*
load_udev_monitor_unref(udev_monitor* monitor) {
	void* p = loadFunc("udev_monitor_unref");
	udev_monitor_unref = p ? (udev_monitor_unref_t*)p : fail_udev_monitor_unref;
	return udev_monitor_unref(monitor);
}

udev*
load_udev_monitor_get_udev(udev_monitor* monitor) {
	void* p = loadFunc("udev_monitor_get_udev");
	udev_monitor_get_udev = p ? (udev_monitor_get_udev_t*)p : fail_udev_monitor_get_udev;
	return udev_monitor_get_udev(monitor);
}

udev_monitor*
load_udev_monitor_new_from_netlink(
	udev* ctx,
	const char* name
) {
	void* p = loadFunc("udev_monitor_new_from_netlink");
	udev_monitor_new_from_netlink = p ? (udev_monitor_new_from_netlink_t*)p : fail_udev_monitor_new_from_netlink;
	return udev_monitor_new_from_netlink(ctx, name);
}

int
load_udev_monitor_enable_receiving(udev_monitor* monitor) {
	void* p = loadFunc("udev_monitor_enable_receiving");
	udev_monitor_enable_receiving = p ? (udev_monitor_enable_receiving_t*)p : fail_udev_monitor_enable_receiving;
	return udev_monitor_enable_receiving(monitor);
}

int
load_udev_monitor_set_receive_buffer_size(
	udev_monitor* monitor,
	int size
) {
	void* p = loadFunc("udev_monitor_set_receive_buffer_size");
	udev_monitor_set_receive_buffer_size = p ? (udev_monitor_set_receive_buffer_size_t*)p : fail_udev_monitor_set_receive_buffer_size;
	return udev_monitor_set_receive_buffer_size(monitor, size);
}

int
load_udev_monitor_get_fd(udev_monitor* monitor) {
	void* p = loadFunc("udev_monitor_get_fd");
	udev_monitor_get_fd = p ? (udev_monitor_get_fd_t*)p : fail_udev_monitor_get_fd;
	return udev_monitor_get_fd(monitor);
}

udev_device*
load_udev_monitor_receive_device(udev_monitor* monitor) {
	void* p = loadFunc("udev_monitor_receive_device");
	udev_monitor_receive_device = p ? (udev_monitor_receive_device_t*)p : fail_udev_monitor_receive_device;
	return udev_monitor_receive_device(monitor);
}

int
load_udev_monitor_filter_add_match_subsystem_devtype(
	udev_monitor* monitor,
	const char* subsystem,
	const char* devtype
) {
	void* p = loadFunc("udev_monitor_filter_add_match_subsystem_devtype");
	udev_monitor_filter_add_match_subsystem_devtype = p ? (udev_monitor_filter_add_match_subsystem_devtype_t*)p : fail_udev_monitor_filter_add_match_subsystem_devtype;
	return udev_monitor_filter_add_match_subsystem_devtype(monitor, subsystem, devtype);
}

int
load_udev_monitor_filter_add_match_tag(
	udev_monitor* monitor,
	const char* tag
) {
	void* p = loadFunc("udev_monitor_filter_add_match_tag");
	udev_monitor_filter_add_match_tag = p ? (udev_monitor_filter_add_match_tag_t*)p : fail_udev_monitor_filter_add_match_tag;
	return udev_monitor_filter_add_match_tag(monitor, tag);
}

int
load_udev_monitor_filter_update(udev_monitor* monitor) {
	void* p = loadFunc("udev_monitor_filter_update");
	udev_monitor_filter_update = p ? (udev_monitor_filter_update_t*)p : fail_udev_monitor_filter_update;
	return udev_monitor_filter_update(monitor);
}

int
load_udev_monitor_filter_remove(udev_monitor* monitor) {
	void* p = loadFunc("udev_monitor_filter_remove");
	udev_monitor_filter_remove = p ? (udev_monitor_filter_remove_t*)p : fail_udev_monitor_filter_remove;
	return udev_monitor_filter_remove(monitor);
}

//
// udev_enumerate
//

udev_enumerate*
load_udev_enumerate_ref(udev_enumerate* enumerate) {
	void* p = loadFunc("udev_enumerate_ref");
	udev_enumerate_ref = p ? (udev_enumerate_ref_t*)p : fail_udev_enumerate_ref;
	return udev_enumerate_ref(enumerate);
}

udev_enumerate*
load_udev_enumerate_unref(udev_enumerate* enumerate) {
	void* p = loadFunc("udev_enumerate_unref");
	udev_enumerate_unref = p ? (udev_enumerate_unref_t*)p : fail_udev_enumerate_unref;
	return udev_enumerate_unref(enumerate);
}

udev*
load_udev_enumerate_get_udev(udev_enumerate* enumerate) {
	void* p = loadFunc("udev_enumerate_get_udev");
	udev_enumerate_get_udev = p ? (udev_enumerate_get_udev_t*)p : fail_udev_enumerate_get_udev;
	return udev_enumerate_get_udev(enumerate);
}

udev_enumerate*
load_udev_enumerate_new(udev* ctx) {
	void* p = loadFunc("udev_enumerate_new");
	udev_enumerate_new = p ? (udev_enumerate_new_t*)p : fail_udev_enumerate_new;
	return udev_enumerate_new(ctx);
}

int
load_udev_enumerate_add_match_subsystem(
	udev_enumerate* enumerate,
	const char* subsystem
) {
	void* p = loadFunc("udev_enumerate_add_match_subsystem");
	udev_enumerate_add_match_subsystem = p ? (udev_enumerate_add_match_subsystem_t*)p : fail_udev_enumerate_add_match_subsystem;
	return udev_enumerate_add_match_subsystem(enumerate, subsystem);
}

int
load_udev_enumerate_add_nomatch_subsystem(
	udev_enumerate* enumerate,
	const char* subsystem
) {
	void* p = loadFunc("udev_enumerate_add_nomatch_subsystem");
	udev_enumerate_add_nomatch_subsystem = p ? (udev_enumerate_add_nomatch_subsystem_t*)p : fail_udev_enumerate_add_nomatch_subsystem;
	return udev_enumerate_add_nomatch_subsystem(enumerate, subsystem);
}

int
load_udev_enumerate_add_match_sysattr(
	udev_enumerate* enumerate,
	const char* sysattr,
	const char* value
) {
	void* p = loadFunc("udev_enumerate_add_match_sysattr");
	udev_enumerate_add_match_sysattr = p ? (udev_enumerate_add_match_sysattr_t*)p : fail_udev_enumerate_add_match_sysattr;
	return udev_enumerate_add_match_sysattr(enumerate, sysattr, value);
}

int
load_udev_enumerate_add_nomatch_sysattr(
	udev_enumerate* enumerate,
	const char* sysattr,
	const char* value
) {
	void* p = loadFunc("udev_enumerate_add_nomatch_sysattr");
	udev_enumerate_add_nomatch_sysattr = p ? (udev_enumerate_add_nomatch_sysattr_t*)p : fail_udev_enumerate_add_nomatch_sysattr;
	return udev_enumerate_add_nomatch_sysattr(enumerate, sysattr, value);
}

int
load_udev_enumerate_add_match_property(
	udev_enumerate* enumerate,
	const char* property,
	const char* value
) {
	void* p = loadFunc("udev_enumerate_add_match_property");
	udev_enumerate_add_match_property = p ? (udev_enumerate_add_match_property_t*)p : fail_udev_enumerate_add_match_property;
	return udev_enumerate_add_match_property(enumerate, property, value);
}

int
load_udev_enumerate_add_match_sysname(
	udev_enumerate* enumerate,
	const char* sysname
) {
	void* p = loadFunc("udev_enumerate_add_match_sysname");
	udev_enumerate_add_match_sysname = p ? (udev_enumerate_add_match_sysname_t*)p : fail_udev_enumerate_add_match_sysname;
	return udev_enumerate_add_match_sysname(enumerate, sysname);
}

int
load_udev_enumerate_add_match_tag(
	udev_enumerate* enumerate,
	const char* tag
) {
	void* p = loadFunc("udev_enumerate_add_match_tag");
	udev_enumerate_add_match_tag = p ? (udev_enumerate_add_match_tag_t*)p : fail_udev_enumerate_add_match_tag;
	return udev_enumerate_add_match_tag(enumerate, tag);
}

int
load_udev_enumerate_add_match_parent(
	udev_enumerate* enumerate,
	udev_device* parent
) {
	void* p = loadFunc("udev_enumerate_add_match_parent");
	udev_enumerate_add_match_parent = p ? (udev_enumerate_add_match_parent_t*)p : fail_udev_enumerate_add_match_parent;
	return udev_enumerate_add_match_parent(enumerate, parent);
}

int
load_udev_enumerate_add_match_is_initialized(udev_enumerate* enumerate) {
	void* p = loadFunc("udev_enumerate_add_match_is_initialized");
	udev_enumerate_add_match_is_initialized = p ? (udev_enumerate_add_match_is_initialized_t*)p : fail_udev_enumerate_add_match_is_initialized;
	return udev_enumerate_add_match_is_initialized(enumerate);
}

int
load_udev_enumerate_add_syspath(
	udev_enumerate* enumerate,
	const char* syspath
) {
	void* p = loadFunc("udev_enumerate_add_syspath");
	udev_enumerate_add_syspath = p ? (udev_enumerate_add_syspath_t*)p : fail_udev_enumerate_add_syspath;
	return udev_enumerate_add_syspath(enumerate, syspath);
}

int
load_udev_enumerate_scan_devices(udev_enumerate* enumerate) {
	void* p = loadFunc("udev_enumerate_scan_devices");
	udev_enumerate_scan_devices = p ? (udev_enumerate_scan_devices_t*)p : fail_udev_enumerate_scan_devices;
	return udev_enumerate_scan_devices(enumerate);
}

int
load_udev_enumerate_scan_subsystems(udev_enumerate* enumerate) {
	void* p = loadFunc("udev_enumerate_scan_subsystems");
	udev_enumerate_scan_subsystems = p ? (udev_enumerate_scan_subsystems_t*)p : fail_udev_enumerate_scan_subsystems;
	return udev_enumerate_scan_subsystems(enumerate);
}

udev_list_entry*
load_udev_enumerate_get_list_entry(udev_enumerate* enumerate) {
	void* p = loadFunc("udev_enumerate_get_list_entry");
	udev_enumerate_get_list_entry = p ? (udev_enumerate_get_list_entry_t*)p : fail_udev_enumerate_get_list_entry;
	return udev_enumerate_get_list_entry(enumerate);
}

//
// udev_queue
//

udev_queue*
load_udev_queue_ref(udev_queue* queue) {
	void* p = loadFunc("udev_queue_ref");
	udev_queue_ref = p ? (udev_queue_ref_t*)p : fail_udev_queue_ref;
	return udev_queue_ref(queue);
}

udev_queue*
load_udev_queue_unref(udev_queue* queue) {
	void* p = loadFunc("udev_queue_unref");
	udev_queue_unref = p ? (udev_queue_unref_t*)p : fail_udev_queue_unref;
	return udev_queue_unref(queue);
}

udev*
load_udev_queue_get_udev(udev_queue* queue) {
	void* p = loadFunc("udev_queue_get_udev");
	udev_queue_get_udev = p ? (udev_queue_get_udev_t*)p : fail_udev_queue_get_udev;
	return udev_queue_get_udev(queue);
}

udev_queue*
load_udev_queue_new(udev* ctx) {
	void* p = loadFunc("udev_queue_new");
	udev_queue_new = p ? (udev_queue_new_t*)p : fail_udev_queue_new;
	return udev_queue_new(ctx);
}

int
load_udev_queue_get_udev_is_active(udev_queue* queue) {
	void* p = loadFunc("udev_queue_get_udev_is_active");
	udev_queue_get_udev_is_active = p ? (udev_queue_get_udev_is_active_t*)p : fail_udev_queue_get_udev_is_active;
	return udev_queue_get_udev_is_active(queue);
}

int
load_udev_queue_get_queue_is_empty(udev_queue* queue) {
	void* p = loadFunc("udev_queue_get_queue_is_empty");
	udev_queue_get_queue_is_empty = p ? (udev_queue_get_queue_is_empty_t*)p : fail_udev_queue_get_queue_is_empty;
	return udev_queue_get_queue_is_empty(queue);
}

int
load_udev_queue_get_fd(udev_queue* queue) {
	void* p = loadFunc("udev_queue_get_fd");
	udev_queue_get_fd = p ? (udev_queue_get_fd_t*)p : fail_udev_queue_get_fd;
	return udev_queue_get_fd(queue);
}

int
load_udev_queue_flush(udev_queue* queue) {
	void* p = loadFunc("udev_queue_flush");
	udev_queue_flush = p ? (udev_queue_flush_t*)p : fail_udev_queue_flush;
	return udev_queue_flush(queue);
}

//
// udev_hwdb
//

udev_hwdb*
load_udev_hwdb_new(udev* ctx) {
	void* p = loadFunc("udev_hwdb_new");
	udev_hwdb_new = p ? (udev_hwdb_new_t*)p : fail_udev_hwdb_new;
	return udev_hwdb_new(ctx);
}

udev_hwdb*
load_udev_hwdb_ref(udev_hwdb* hwdb) {
	void* p = loadFunc("udev_hwdb_ref");
	udev_hwdb_ref = p ? (udev_hwdb_ref_t*)p : fail_udev_hwdb_ref;
	return udev_hwdb_ref(hwdb);
}

udev_hwdb*
load_udev_hwdb_unref(udev_hwdb* hwdb) {
	void* p = loadFunc("udev_hwdb_unref");
	udev_hwdb_unref = p ? (udev_hwdb_unref_t*)p : fail_udev_hwdb_unref;
	return udev_hwdb_unref(hwdb);
}

udev_list_entry*
load_udev_hwdb_get_properties_list_entry(
	udev_hwdb* hwdb,
	const char* modalias,
	unsigned flags
) {
	void* p = loadFunc("udev_hwdb_get_properties_list_entry");
	udev_hwdb_get_properties_list_entry = p ? (udev_hwdb_get_properties_list_entry_t*)p : fail_udev_hwdb_get_properties_list_entry;
	return udev_hwdb_get_properties_list_entry(hwdb, modalias, flags);
}

//..............................................................................

udev_ref_t* udev_ref = load_udev_ref;
udev_unref_t* udev_unref = load_udev_unref;
udev_new_t* udev_new = load_udev_new;
udev_get_userdata_t* udev_get_userdata = load_udev_get_userdata;
udev_set_userdata_t* udev_set_userdata = load_udev_set_userdata;
udev_list_entry_get_next_t* udev_list_entry_get_next = load_udev_list_entry_get_next;
udev_list_entry_get_by_name_t* udev_list_entry_get_by_name = load_udev_list_entry_get_by_name;
udev_list_entry_get_name_t* udev_list_entry_get_name = load_udev_list_entry_get_name;
udev_list_entry_get_value_t* udev_list_entry_get_value = load_udev_list_entry_get_value;
udev_device_ref_t* udev_device_ref = load_udev_device_ref;
udev_device_unref_t* udev_device_unref = load_udev_device_unref;
udev_device_get_udev_t* udev_device_get_udev = load_udev_device_get_udev;
udev_device_new_from_syspath_t* udev_device_new_from_syspath = load_udev_device_new_from_syspath;
udev_device_new_from_devnum_t* udev_device_new_from_devnum = load_udev_device_new_from_devnum;
udev_device_new_from_subsystem_sysname_t* udev_device_new_from_subsystem_sysname = load_udev_device_new_from_subsystem_sysname;
udev_device_new_from_device_id_t* udev_device_new_from_device_id = load_udev_device_new_from_device_id;
udev_device_new_from_environment_t* udev_device_new_from_environment = load_udev_device_new_from_environment;
udev_device_get_parent_t* udev_device_get_parent = load_udev_device_get_parent;
udev_device_get_parent_with_subsystem_devtype_t* udev_device_get_parent_with_subsystem_devtype = load_udev_device_get_parent_with_subsystem_devtype;
udev_device_get_devpath_t* udev_device_get_devpath = load_udev_device_get_devpath;
udev_device_get_subsystem_t* udev_device_get_subsystem = load_udev_device_get_subsystem;
udev_device_get_devtype_t* udev_device_get_devtype = load_udev_device_get_devtype;
udev_device_get_syspath_t* udev_device_get_syspath = load_udev_device_get_syspath;
udev_device_get_sysname_t* udev_device_get_sysname = load_udev_device_get_sysname;
udev_device_get_sysnum_t* udev_device_get_sysnum = load_udev_device_get_sysnum;
udev_device_get_devnode_t* udev_device_get_devnode = load_udev_device_get_devnode;
udev_device_get_is_initialized_t* udev_device_get_is_initialized = load_udev_device_get_is_initialized;
udev_device_get_devlinks_list_entry_t* udev_device_get_devlinks_list_entry = load_udev_device_get_devlinks_list_entry;
udev_device_get_properties_list_entry_t* udev_device_get_properties_list_entry = load_udev_device_get_properties_list_entry;
udev_device_get_tags_list_entry_t* udev_device_get_tags_list_entry = load_udev_device_get_tags_list_entry;
udev_device_get_current_tags_list_entry_t* udev_device_get_current_tags_list_entry = load_udev_device_get_current_tags_list_entry;
udev_device_get_sysattr_list_entry_t* udev_device_get_sysattr_list_entry = load_udev_device_get_sysattr_list_entry;
udev_device_get_property_value_t* udev_device_get_property_value = load_udev_device_get_property_value;
udev_device_get_driver_t* udev_device_get_driver = load_udev_device_get_driver;
udev_device_get_devnum_t* udev_device_get_devnum = load_udev_device_get_devnum;
udev_device_get_action_t* udev_device_get_action = load_udev_device_get_action;
udev_device_get_seqnum_t* udev_device_get_seqnum = load_udev_device_get_seqnum;
udev_device_get_usec_since_initialized_t* udev_device_get_usec_since_initialized = load_udev_device_get_usec_since_initialized;
udev_device_get_sysattr_value_t* udev_device_get_sysattr_value = load_udev_device_get_sysattr_value;
udev_device_set_sysattr_value_t* udev_device_set_sysattr_value = load_udev_device_set_sysattr_value;
udev_device_has_tag_t* udev_device_has_tag = load_udev_device_has_tag;
udev_device_has_current_tag_t* udev_device_has_current_tag = load_udev_device_has_current_tag;
udev_monitor_ref_t* udev_monitor_ref = load_udev_monitor_ref;
udev_monitor_unref_t* udev_monitor_unref = load_udev_monitor_unref;
udev_monitor_get_udev_t* udev_monitor_get_udev = load_udev_monitor_get_udev;
udev_monitor_new_from_netlink_t* udev_monitor_new_from_netlink = load_udev_monitor_new_from_netlink;
udev_monitor_enable_receiving_t* udev_monitor_enable_receiving = load_udev_monitor_enable_receiving;
udev_monitor_set_receive_buffer_size_t* udev_monitor_set_receive_buffer_size = load_udev_monitor_set_receive_buffer_size;
udev_monitor_get_fd_t* udev_monitor_get_fd = load_udev_monitor_get_fd;
udev_monitor_receive_device_t* udev_monitor_receive_device = load_udev_monitor_receive_device;
udev_monitor_filter_add_match_subsystem_devtype_t* udev_monitor_filter_add_match_subsystem_devtype = load_udev_monitor_filter_add_match_subsystem_devtype;
udev_monitor_filter_add_match_tag_t* udev_monitor_filter_add_match_tag = load_udev_monitor_filter_add_match_tag;
udev_monitor_filter_update_t* udev_monitor_filter_update = load_udev_monitor_filter_update;
udev_monitor_filter_remove_t* udev_monitor_filter_remove = load_udev_monitor_filter_remove;
udev_enumerate_ref_t* udev_enumerate_ref = load_udev_enumerate_ref;
udev_enumerate_unref_t* udev_enumerate_unref = load_udev_enumerate_unref;
udev_enumerate_get_udev_t* udev_enumerate_get_udev = load_udev_enumerate_get_udev;
udev_enumerate_new_t* udev_enumerate_new = load_udev_enumerate_new;
udev_enumerate_add_match_subsystem_t* udev_enumerate_add_match_subsystem = load_udev_enumerate_add_match_subsystem;
udev_enumerate_add_nomatch_subsystem_t* udev_enumerate_add_nomatch_subsystem = load_udev_enumerate_add_nomatch_subsystem;
udev_enumerate_add_match_sysattr_t* udev_enumerate_add_match_sysattr = load_udev_enumerate_add_match_sysattr;
udev_enumerate_add_nomatch_sysattr_t* udev_enumerate_add_nomatch_sysattr = load_udev_enumerate_add_nomatch_sysattr;
udev_enumerate_add_match_property_t* udev_enumerate_add_match_property = load_udev_enumerate_add_match_property;
udev_enumerate_add_match_sysname_t* udev_enumerate_add_match_sysname = load_udev_enumerate_add_match_sysname;
udev_enumerate_add_match_tag_t* udev_enumerate_add_match_tag = load_udev_enumerate_add_match_tag;
udev_enumerate_add_match_parent_t* udev_enumerate_add_match_parent = load_udev_enumerate_add_match_parent;
udev_enumerate_add_match_is_initialized_t* udev_enumerate_add_match_is_initialized = load_udev_enumerate_add_match_is_initialized;
udev_enumerate_add_syspath_t* udev_enumerate_add_syspath = load_udev_enumerate_add_syspath;
udev_enumerate_scan_devices_t* udev_enumerate_scan_devices = load_udev_enumerate_scan_devices;
udev_enumerate_scan_subsystems_t* udev_enumerate_scan_subsystems = load_udev_enumerate_scan_subsystems;
udev_enumerate_get_list_entry_t* udev_enumerate_get_list_entry = load_udev_enumerate_get_list_entry;
udev_queue_ref_t* udev_queue_ref = load_udev_queue_ref;
udev_queue_unref_t* udev_queue_unref = load_udev_queue_unref;
udev_queue_get_udev_t* udev_queue_get_udev = load_udev_queue_get_udev;
udev_queue_new_t* udev_queue_new = load_udev_queue_new;
udev_queue_get_udev_is_active_t* udev_queue_get_udev_is_active = load_udev_queue_get_udev_is_active;
udev_queue_get_queue_is_empty_t* udev_queue_get_queue_is_empty = load_udev_queue_get_queue_is_empty;
udev_queue_get_fd_t* udev_queue_get_fd = load_udev_queue_get_fd;
udev_queue_flush_t* udev_queue_flush = load_udev_queue_flush;
udev_hwdb_new_t* udev_hwdb_new = load_udev_hwdb_new;
udev_hwdb_ref_t* udev_hwdb_ref = load_udev_hwdb_ref;
udev_hwdb_unref_t* udev_hwdb_unref = load_udev_hwdb_unref;
udev_hwdb_get_properties_list_entry_t* udev_hwdb_get_properties_list_entry = load_udev_hwdb_get_properties_list_entry;

//..............................................................................

} // namespace libudev
} // namespace lnx
} // namespace sys
} // namespace axl
