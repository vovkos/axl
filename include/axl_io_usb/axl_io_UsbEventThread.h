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

#define _AXL_IO_USBEVENTTHREAD_H

#include "axl_io_UsbPch.h"
#include "axl_sys_Thread.h"
#include "axl_sl_Singleton.h"

namespace axl {
namespace io {

//..............................................................................

class UsbEventThread: public sys::ThreadImpl<UsbEventThread> {
public:
	enum {
		TimerGranularity = 500,
	};

protected:
	libusb_context* m_context;
	volatile bool m_isTerminating;

public:
	UsbEventThread() {
		m_context = NULL;
		m_isTerminating = false;
	}

	~UsbEventThread() {
		stop();
	}

	bool
	start(libusb_context* context = NULL);

	void
	stop() {
		m_isTerminating = true;
		waitAndClose();
	}

	void
	threadFunc();
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
UsbEventThread*
getUsbDefaultContextEventThread() {
	static int32_t flag = 0;
	return sl::getSingleton<UsbEventThread> (&flag);
}

//..............................................................................

} // namespace io
} // namespace axl
