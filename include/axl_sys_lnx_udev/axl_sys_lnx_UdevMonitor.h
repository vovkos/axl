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

#define _AXL_SYS_LNX_UDEVMONITOR_H

#include "axl_sys_lnx_UdevDevice.h"

namespace axl {
namespace sys {
namespace lnx {

using namespace libudev;

//..............................................................................

class UnrefUdevMonitor {
public:
	void
	operator () (udev_monitor* h) {
		udev_monitor_unref(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UdevMonitor: public sl::Handle<udev_monitor*, UnrefUdevMonitor> {
public:
	UdevMonitor() {}

	UdevMonitor(udev_monitor* monitor) {
		if ((m_h = monitor))
			ref();
	}

	void
	ref() {
		udev_monitor_ref(m_h);
	}

	void
	unref() {
		udev_monitor_unref(m_h);
	}

	bool
	createFromNetlink(
		udev* ctx,
		const sl::StringRef& name = "udev" // "udev" or "kernel" (for raw kernel events)
	) {
		attach(udev_monitor_new_from_netlink(ctx, name.sz()));
		return completeUdev(m_h, "udev_monitor_new_from_netlink");
	}

	udev*
	getUdev() const {
		return udev_monitor_get_udev(m_h);
	}

	bool
	enableReceiving() {
		int result = udev_monitor_enable_receiving(m_h);
		return completeUdevErrno(result);
	}

	bool
	setReceiveBufferSize(size_t size) {
		int result = udev_monitor_set_receive_buffer_size(m_h, (int)size);
		return completeUdevErrno(result);
	}

	int
	getFd() const {
		return udev_monitor_get_fd(m_h);
	}

	UdevDevice
	receiveDevice();

	bool
	receiveDevice(UdevDevice* device) {
		device->attach(udev_monitor_receive_device(m_h));
		return device->isOpen(); // false is not an error
	}

	bool
	addMatchSubsystemDevType(
		const sl::StringRef& subsystem,
		const sl::StringRef& devType = sl::StringRef() // empty matches any
	) {
		int result = udev_monitor_filter_add_match_subsystem_devtype(m_h, subsystem.sz(), devType.szn());
		return completeUdevErrno(result);
	}

	bool
	addMatchTag(const sl::StringRef& tag) {
		int result = udev_monitor_filter_add_match_tag(m_h, tag.sz());
		return completeUdevErrno(result);
	}

	bool
	updateFilter() {
		int result = udev_monitor_filter_update(m_h);
		return completeUdevErrno(result);
	}

	bool
	removeFilter() {
		int result = udev_monitor_filter_remove(m_h);
		return completeUdevErrno(result);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
UdevDevice
UdevMonitor::receiveDevice() {
	UdevDevice device;
	receiveDevice(&device);
	return device;
}

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
