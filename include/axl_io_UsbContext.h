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

#define _AXL_IO_USBCONTEXT_H

#include "axl_io_UsbError.h"
#include "axl_sl_Singleton.h"

namespace axl {
namespace io {

//..............................................................................

class CloseUsbContext
{
public:
	void
	operator () (libusb_context* h)
	{
		libusb_exit (h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UsbContext: public sl::Handle <
	libusb_context*, 
	CloseUsbContext, 
	sl::MinusOne <libusb_context*> 
	>
{
public:
	bool
	create ();

	bool
	createDefault ();

	void
	setDebugLevel (int level)
	{
		libusb_set_debug (m_h, level);
	}

	bool
	handleEvents ()
	{
		int result = libusb_handle_events (m_h);
		return result == 0 ? true : err::fail (UsbError ((int) result));
	}

	bool
	handleEvents (uint_t timeout);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
UsbContext*
getUsbDefaultContext ()
{
	static int32_t flag = 0;
	return sl::getSingleton <UsbContext> (&flag);
}

inline
bool
hasUsbCapability (uint32_t capability)
{
	return libusb_has_capability (capability) != 0;
}

//..............................................................................

} // namespace io
} // namespace axl
