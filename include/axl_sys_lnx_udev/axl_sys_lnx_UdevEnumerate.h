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

#define _AXL_SYS_LNX_UDEVENUMERATE_H

#include "axl_sys_lnx_Udev.h"

namespace axl {
namespace sys {
namespace lnx {

using namespace libudev;

//..............................................................................

class UnrefUdevEnumerate {
public:
	void
	operator () (udev_enumerate* h) {
		udev_enumerate_unref(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UdevEnumerate: public sl::Handle<udev_enumerate*, UnrefUdevEnumerate> {
public:
	UdevEnumerate() {}

	UdevEnumerate(udev_enumerate* enumerate) {
		if (m_h = enumerate)
			ref();
	}

	void
	ref() {
		udev_enumerate_ref(m_h);
	}

	void
	unref() {
		udev_enumerate_ref(m_h);
	}

	bool
	create(udev* ctx) {
		attach(udev_enumerate_new(ctx));
		return completeUdev(m_h, "udev_enumerate_new");
	}

	udev*
	getUdev() const {
		return udev_enumerate_get_udev(m_h);
	}

	bool
	addMatchSubsystem(const sl::StringRef& subsystem) {
		int result = udev_enumerate_add_match_subsystem(m_h, subsystem.sz());
		return completeUdevErrno(result);
	}

	bool
	addNoMatchSubsystem(const sl::StringRef& subsystem) {
		int result = udev_enumerate_add_nomatch_subsystem(m_h, subsystem.sz());
		return completeUdevErrno(result);
	}

	bool
	addMatchSysAttr(
		const sl::StringRef& sysAttr,
		const sl::StringRef& value
	) {
		int result = udev_enumerate_add_match_sysattr(m_h, sysAttr.sz(), value.sz());
		return completeUdevErrno(result);
	}

	bool
	addNoMatchSysAttr(
		const sl::StringRef& sysAttr,
		const sl::StringRef& value
	) {
		int result = udev_enumerate_add_match_sysattr(m_h, sysAttr.sz(), value.sz());
		return completeUdevErrno(result);
	}

	bool
	addMatchProperty(
		const sl::StringRef& prop,
		const sl::StringRef& value
	) {
		int result = udev_enumerate_add_match_property(m_h, prop.sz(), value.sz());
		return completeUdevErrno(result);
	}

	bool
	addMatchSysName(const sl::StringRef& sysName) {
		int result = udev_enumerate_add_match_sysname(m_h, sysName.sz());
		return completeUdevErrno(result);
	}

	bool
	addMatchTag(const sl::StringRef& tag) {
		int result = udev_enumerate_add_match_tag(m_h, tag.sz());
		return completeUdevErrno(result);
	}

	bool
	addMatchParentDevice(udev_device* parentDevice) {
		int result = udev_enumerate_add_match_parent(m_h, parentDevice);
		return completeUdevErrno(result);
	}

	bool
	addMatchIsInitialized() {
		int result = udev_enumerate_add_match_is_initialized(m_h);
		return completeUdevErrno(result);
	}

	bool
	scanDevices() {
		int result = udev_enumerate_scan_devices(m_h);
		return completeUdevErrno(result);
	}

	bool
	scanSubsystems() {
		int result = udev_enumerate_scan_subsystems(m_h);
		return completeUdevErrno(result);
	}

	bool
	addSysPath(const sl::StringRef& sysPath) {
		int result = udev_enumerate_add_syspath(m_h, sysPath.sz());
		return completeUdevErrno(result);
	}

	udev_list_entry*
	getListEntry() const {
		return udev_enumerate_get_list_entry(m_h);
	}
};

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
