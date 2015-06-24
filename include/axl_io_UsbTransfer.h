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

class FreeUsbTransfer
{
public:
	void
	operator () (libusb_transfer* h)
	{
		libusb_free_transfer (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UsbTransfer: public rtl::Handle <libusb_transfer*, FreeUsbTransfer>
{
};

//.............................................................................

} // namespace io {
} // namespace axl {
