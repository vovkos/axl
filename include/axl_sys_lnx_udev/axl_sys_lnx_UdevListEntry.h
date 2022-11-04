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

#define _AXL_SYS_LNX_UDEVLISTENTRY_H

#include "axl_sys_lnx_Udev.h"

namespace axl {
namespace sys {
namespace lnx {

using namespace libudev;

//..............................................................................

class UdevListEntry {
protected:
	udev_list_entry* m_p;

public:
	UdevListEntry(udev_list_entry* p = NULL) {
		m_p = p;
	}

	operator udev_list_entry* () const {
		return m_p;
	}

	UdevListEntry&
	operator ++ () {
		return next();
	}

	UdevListEntry
	operator ++ (int) { // post increment
		UdevListEntry old = *this;
		next();
		return old;
	}

	UdevListEntry&
	next() {
		if (m_p)
			m_p = udev_list_entry_get_next(m_p);

		return *this;
	}

	UdevListEntry
	getNext() const {
		return UdevListEntry(*this).next();
	}

	UdevListEntry&
	gotoName(const sl::StringRef& name) {
		if (m_p)
			m_p = udev_list_entry_get_by_name(m_p, name.sz());

		return *this;
	}

	UdevListEntry
	findName(const sl::StringRef& name) const {
		return UdevListEntry(*this).gotoName(name);
	}

	sl::StringRef
	getName() const {
		return m_p ? udev_list_entry_get_name(m_p) : sl::StringRef();
	}

	sl::StringRef
	getValue() const {
		return m_p ? udev_list_entry_get_value(m_p) : sl::StringRef();
	}
};

//..............................................................................

} // namespace lnx
} // namespace sys
} // namespace axl
