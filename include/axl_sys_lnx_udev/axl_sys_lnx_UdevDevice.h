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

#define _AXL_SYS_LNX_UDEVDEVICE_H

#include "axl_sys_lnx_Udev.h"

namespace axl {
namespace sys {
namespace lnx {

using namespace libudev;

//..............................................................................

class UnrefUdevDevice {
public:
	void
	operator () (udev_device* h) {
		udev_device_unref(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UdevDevice: public sl::Handle<udev_device*, UnrefUdevDevice> {
public:
	UdevDevice() {}

	UdevDevice(udev_device* device) {
		if (m_h = device)
			ref();
	}

	void
	ref() {
		udev_device_ref(m_h);
	}

	void
	unref() {
		udev_device_unref(m_h);
	}

	bool
	createFromSysPath(
		udev* ctx,
		const sl::StringRef& path
	) {
		attach(udev_device_new_from_syspath(ctx, path.sz()));
		return completeUdev(m_h, "udev_device_new_from_syspath");
	}

	bool
	createFromDevNum(
		udev* ctx,
		char type,
		dev_t devNum
	) {
		attach(udev_device_new_from_devnum(ctx, type, devNum));
		return completeUdev(m_h, "udev_device_new_from_devnum");
	}

	bool
	createFromSubsytemSysName(
		udev* ctx,
		const sl::StringRef& subsystem,
		const sl::StringRef& sysName
	) {
		attach(udev_device_new_from_subsystem_sysname(ctx, subsystem.sz(), sysName.sz()));
		return completeUdev(m_h, "udev_device_new_from_subsystem_sysname");
	}

	bool
	createDeviceFromDeviceId(
		udev* ctx,
		const sl::StringRef& deviceId
	) {
		attach(udev_device_new_from_device_id(ctx, (char*)deviceId.sz()));
		return completeUdev(m_h, "udev_device_new_from_device_id");
	}

	bool
	createFromEnvironment(udev* ctx) {
		attach(udev_device_new_from_environment(ctx));
		return completeUdev(m_h, "udev_device_new_from_environment");
	}

	udev*
	getUdev() const {
		return udev_device_get_udev(m_h);
	}

	sl::StringRef
	getSysPath() const {
		return udev_device_get_syspath(m_h);
	}

	sl::StringRef
	getSysName() const {
		return udev_device_get_sysname(m_h);
	}

	sl::StringRef
	getSysNum() const {
		return udev_device_get_sysnum(m_h);
	}

	sl::StringRef
	getDevPath() const {
		return udev_device_get_devpath(m_h);
	}

	sl::StringRef
	getDevNode() const {
		return udev_device_get_devnode(m_h);
	}

	dev_t
	getDevNum() const {
		return udev_device_get_devnum(m_h);
	}

	sl::StringRef
	getDevType() const {
		return udev_device_get_devtype(m_h);
	}

	sl::StringRef
	getSubsystem() const {
		return udev_device_get_subsystem(m_h);
	}

	sl::StringRef
	getDriver() const {
		return udev_device_get_driver(m_h);
	}

	udev_device*
	getParent() const {
		return udev_device_get_parent(m_h);
	}

	udev_device*
	getParentWithSubsystemDevType(const sl::StringRef& subSystem) const {
		return udev_device_get_parent_with_subsystem_devtype(m_h, subSystem.szn(), NULL);
	}

	udev_device*
	getParentWithSubsystemDevType(
		const sl::StringRef& subSystem,
		const sl::StringRef& devType
	) const {
		return udev_device_get_parent_with_subsystem_devtype(m_h, subSystem.szn(), devType.szn());
	}

	bool
	isInitialized() const {
		return udev_device_get_is_initialized(m_h) > 0;
	}

	sl::StringRef
	getAction() const {
		return udev_device_get_action(m_h);
	}

	udev_list_entry*
	getDevLinkList() const {
		return udev_device_get_devlinks_list_entry(m_h);
	}

	udev_list_entry*
	getPropertyList() const {
		return udev_device_get_properties_list_entry(m_h);
	}

	udev_list_entry*
	getTagList() const {
		return udev_device_get_tags_list_entry(m_h);
	}

	udev_list_entry*
	getSysAttrList() const {
		return udev_device_get_sysattr_list_entry(m_h);
	}

	sl::StringRef
	getPropertyValue(const sl::StringRef& key) const {
		return udev_device_get_property_value(m_h, key.sz());
	}

	bool
	hasTag(const sl::StringRef& tag) const {
		return udev_device_has_tag(m_h, tag.sz()) > 0;
	}

	sl::StringRef
	getSysAttrValue(const sl::StringRef& sysAttr) const {
		return udev_device_get_sysattr_value(m_h, sysAttr.sz());
	}

	bool
	setSysAttrValue(
		const sl::StringRef& sysAttr,
		const sl::StringRef& value
	) {
		bool result = udev_device_set_sysattr_value(m_h, sysAttr.sz(), (char*)value.sz());
		return completeUdev(result, "udev_device_set_sysattr_value");
	}
};

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
