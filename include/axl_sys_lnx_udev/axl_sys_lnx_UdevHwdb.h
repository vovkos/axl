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

#define _AXL_SYS_LNX_UDEVHWDB_H

#include "axl_sys_lnx_Udev.h"

namespace axl {
namespace sys {
namespace lnx {

using namespace libudev;

//..............................................................................

class UnrefUdevHwdb {
public:
	void
	operator () (udev_hwdb* h) {
		udev_hwdb_unref(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UdevHwdb: public sl::Handle<udev_hwdb*, UnrefUdevHwdb> {
public:
	UdevHwdb() {}

	UdevHwdb(udev_hwdb* hwdb) {
		if (m_h = hwdb)
			ref();
	}

	void
	ref() {
		udev_hwdb_ref(m_h);
	}

	void
	unref() {
		udev_hwdb_unref(m_h);
	}

	bool
	create(udev* ctx) {
		attach(udev_hwdb_new(ctx));
		return completeUdev(m_h, "udev_hwdb_new");
	}

	udev_list_entry*
	getProperties(
		const sl::StringRef& modalias,
		unsigned flags = 0
	) const {
		return udev_hwdb_get_properties_list_entry(m_h, modalias.sz(), flags);
	}
};

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
