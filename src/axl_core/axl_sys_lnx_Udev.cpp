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

namespace axl {
namespace sys {
namespace lnx {

//..............................................................................

const char* UdevLib::m_funcNameTable[FuncId__Count] =
{
	"udev_ref",
	"udev_unref",
	"udev_new",
	"udev_device_new_from_syspath",
	"udev_device_new_from_devnum",
	"udev_device_new_from_subsystem_sysname",
	"udev_device_new_from_device_id",
	"udev_device_new_from_environment",
	"udev_enumerate_new",
	"udev_monitor_new_from_netlink",
	"udev_device_ref",
	"udev_device_unref",
	"udev_device_get_udev",
	"udev_device_get_syspath",
	"udev_device_get_sysname",
	"udev_device_get_sysnum",
	"udev_device_get_devpath",
	"udev_device_get_devnode",
	"udev_device_get_devnum",
	"udev_device_get_devtype",
	"udev_device_get_subsystem",
	"udev_device_get_driver",
	"udev_device_get_parent",
	"udev_device_get_parent_with_subsystem_devtype",
	"udev_device_get_is_initialized",
	"udev_device_get_action",
	"udev_device_get_devlinks_list_entry",
	"udev_device_get_properties_list_entry",
	"udev_device_get_tags_list_entry",
	"udev_device_get_sysattr_list_entry",
	"udev_device_get_property_value",
	"udev_device_has_tag",
	"udev_device_get_sysattr_value",
	"udev_device_set_sysattr_value",
	"udev_enumerate_ref",
	"udev_enumerate_unref",
	"udev_enumerate_get_udev",
	"udev_enumerate_add_match_subsystem",
	"udev_enumerate_add_nomatch_subsystem",
	"udev_enumerate_add_match_sysattr",
	"udev_enumerate_add_nomatch_sysattr",
	"udev_enumerate_add_match_property",
	"udev_enumerate_add_match_sysname",
	"udev_enumerate_add_match_tag",
	"udev_enumerate_add_match_parent",
	"udev_enumerate_add_match_is_initialized",
	"udev_enumerate_scan_devices",
	"udev_enumerate_scan_subsystems",
	"udev_enumerate_add_syspath",
	"udev_enumerate_get_list_entry",
	"udev_list_entry_get_next",
	"udev_list_entry_get_by_name",
	"udev_list_entry_get_name",
	"udev_list_entry_get_value",
	"udev_monitor_ref",
	"udev_monitor_unref",
	"udev_monitor_get_udev",
	"udev_monitor_get_fd",
	"udev_monitor_filter_update",
	"udev_monitor_filter_remove",
	"udev_monitor_filter_add_match_subsystem_devtype",
	"udev_monitor_filter_add_match_tag",
	"udev_monitor_set_receive_buffer_size",
	"udev_monitor_enable_receiving",
	"udev_monitor_receive_device",
};

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
