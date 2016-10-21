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

#define _AXL_IO_USBTRANSFER_H

#include "axl_io_UsbError.h"

namespace axl {
namespace io {

//..............................................................................

class FreeUsbTransfer
{
public:
	void
	operator () (libusb_transfer* h)
	{
		libusb_free_transfer (h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class UsbTransfer: public sl::Handle <libusb_transfer*, FreeUsbTransfer>
{
};

//..............................................................................

} // namespace io
} // namespace axl
