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

#include "axl_sl_Handle.h"

namespace axl {
namespace sys {
namespace lnx {

//..............................................................................

template <typename T>
class UnrefUdevDevice {
public:
	void
	operator () (udev_device* h) {
		T::udev_device_unref(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class UdevDeviceBase: public sl::Handle<udev_device*, UnrefUdevDevice<T> > {
public:
	UdevDeviceBase() {}

	UdevDeviceBase(udev_device* h) {
		this->m_h = h;
	}

	void
	addRef() {
		T::udev_device_ref(this->m_h);
	}

	udev*
	getUdev() {
		return T::udev_device_get_udev(this->m_h);
	}

	sl::StringRef
	getSysPath() {
		return T::udev_device_get_syspath(this->m_h);
	}

	sl::StringRef
	getSysName() {
		return T::udev_device_get_sysname(this->m_h);
	}

	sl::StringRef
	getSysNum() {
		return T::udev_device_get_sysnum(this->m_h);
	}

	sl::StringRef
	getDevPath() {
		return T::udev_device_get_devpath(this->m_h);
	}

	sl::StringRef
	getDevNode() {
		return T::udev_device_get_devnode(this->m_h);
	}

	dev_t
	getDevNum() {
		return T::udev_device_get_devnum(this->m_h);
	}

	sl::StringRef
	getDevType() {
		return T::udev_device_get_devtype(this->m_h);
	}

	sl::StringRef
	getSubsystem() {
		return T::udev_device_get_subsystem(this->m_h);
	}

	sl::StringRef
	getDriver() {
		return T::udev_device_get_driver(this->m_h);
	}

	udev_device*
	getParent() {
		return T::udev_device_get_parent(this->m_h);
	}

	udev_device*
	getParentWithSubsystemDevType(
		const sl::StringRef& subSystem,
		const sl::StringRef& devType
	) {
		return T::udev_device_get_parent_with_subsystem_devtype(this->m_h, subSystem.sz(), devType.sz());
	}

	bool
	isInitialized() {
		return T::udev_device_get_is_initialized(this->m_h) != 0;
	}

	sl::StringRef
	getAction() {
		return T::udev_device_get_action(this->m_h);
	}


	udev_list_entry*
	getDevLinkList() {
		return T::udev_device_get_devlinks_list_entry(this->m_h);
	}

	udev_list_entry*
	getPropertyList() {
		return T::udev_device_get_properties_list_entry(this->m_h);
	}

	udev_list_entry*
	getTagList() {
		return T::udev_device_get_tags_list_entry(this->m_h);
	}

	udev_list_entry*
	getSysAttrList() {
		return T::udev_device_get_sysattr_list_entry(this->m_h);
	}

	sl::StringRef
	getPropertyValue(const sl::StringRef& key) {
		return T::udev_device_get_property_value(this->m_h, key.sz());
	}

	bool
	hasTag(const sl::StringRef& tag) {
		return T::udev_device_has_tag(this->m_h, tag.sz());
	}

#if (_AXL_SYS_LNX_LIBUDEV_NEW_API)
	sl::StringRef
	getSysAttrValue(const sl::StringRef& sysAttr) {
		return T::udev_device_get_sysattr_value(this->m_h, sysAttr.sz());
	}

	int
	setSysAttrValue(
		const sl::StringRef& sysAttr,
		const sl::StringRef& value
	) {
		return T::udev_device_set_sysattr_value(this->m_h, sysAttr.sz(), (char*)value.sz());
	}
#endif
};

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
