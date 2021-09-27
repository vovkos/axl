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

#include "axl_sl_Handle.h"

namespace axl {
namespace sys {
namespace lnx {

//..............................................................................

template <typename T>
class UnrefUdevMonitor {
public:
	void
	operator () (udev_monitor* h) {
		T::udev_monitor_unref(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class UdevMonitorBase: public sl::Handle<udev_monitor*, UnrefUdevMonitor<T> > {
public:
	UdevMonitorBase() {}

	UdevMonitorBase(udev_monitor* h) {
		this->m_h = h;
	}

	void
	addRef() {
		T::udev_monitor_ref(this->m_h);
	}

	udev*
	getUdev() {
		return T::udev_monitor_get_udev(this->m_h);
	}

	int
	getFd() {
		return T::udev_monitor_get_fd(this->m_h);
	}

	bool
	updateFilter() {
		return T::udev_monitor_filter_update_b(this->m_h);
	}

	bool
	removeFilter() {
		return T::udev_monitor_filter_remove_b(this->m_h);
	}

	bool
	addMatchSubsystemDevTypeFilter(
		const sl::StringRef& subsystem,
		const sl::StringRef& devType
	) {
		return T::udev_monitor_filter_add_match_subsystem_devtype_b(this->m_h, subsystem.sz(), devType.sz());
	}

	bool
	addMatchTagFilter(const char *tag) {
		return T::udev_monitor_filter_add_match_tag_b(this->m_h, tag);
	}

	bool
	setReceiveBufferSize(size_t size) {
		return T::udev_monitor_set_receive_buffer_size_b(this->m_h, size);
	}

	bool
	enableReceiving() {
		return T::udev_monitor_enable_receiving_b(this->m_h);
	}

	udev_device*
	receiveDevice() {
		return T::udev_monitor_receive_device(this->m_h);
	}
};

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
