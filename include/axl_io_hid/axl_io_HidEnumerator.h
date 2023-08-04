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

#define _AXL_IO_HIDENUMERATOR_H

#include "axl_io_HidDevice.h"

namespace axl {
namespace io {

//..............................................................................

class FreeHidDeviceInfoList {
public:
	void
	operator () (hid_device_info* device) {
		::hid_free_enumeration(device);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HidDeviceInfoIterator {
protected:
	const hid_device_info* m_p;

public:
	HidDeviceInfoIterator(const hid_device_info* p = NULL) {
		m_p = p;
	}

	operator const void* () const {
		return m_p;
	}

	const hid_device_info*
	operator * () const {
		return m_p;
	}

	const hid_device_info*
	operator -> () const {
		ASSERT(this->m_p);
		return m_p;
	}

	HidDeviceInfoIterator&
	operator ++ () {
		next();
		return *this;
	}

	HidDeviceInfoIterator
	operator ++ (int) {
		HidDeviceInfoIterator old = *this;
		next();
		return old;
	}

	void
	next() {
		if (m_p)
			m_p = m_p->next;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HidDeviceInfoList: public sl::Handle<hid_device_info*, FreeHidDeviceInfoList> {
public:
	bool
	enumerate(
		uint16_t vid = 0,
		uint16_t pid = 0
	) {
		close();
		m_h = hid_enumerate(vid, pid);
		return hidComplete(m_h != NULL);
	}

	HidDeviceInfoIterator
	getHead() const {
		return m_h;
	}
};

//..............................................................................

} // namespace io
} // namespace axl
