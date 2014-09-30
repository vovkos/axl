// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_USBTRANSFER_H

#include "axl_io_UsbError.h"
#include "axl_rtl_Handle.h"

namespace axl {
namespace io {

//.............................................................................

class FreeUsbransfer
{
public:
	void
	operator () (libusb_transfer* h)
	{
		libusb_free_transfer (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Usbransfer: public rtl::Handle <libusb_transfer*, FreeUsbransfer>
{
};

//.............................................................................

} // namespace io {
} // namespace axl {
