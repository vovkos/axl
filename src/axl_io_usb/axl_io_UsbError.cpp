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

#include "pch.h"
#include "axl_io_UsbError.h"

namespace axl {
namespace io {

//..............................................................................

sl::StringRef
UsbErrorProvider::getErrorDescription(int code)
{
	static const char* errorTable[LIBUSB_ERROR_COUNT] =
	{
		"Success",
		"Input/Output Error",
		"Invalid parameter",
		"Access denied (insufficient permissions)",
		"No such device (it may have been disconnected)",
		"Entity not found",
		"Resource busy",
		"Operation timed out",
		"Overflow",
		"Pipe error",
		"System call interrupted (perhaps due to signal)",
		"Insufficient memory",
		"Operation not supported or unimplemented on this platform",
		"Other error",
	};

	static const char* transferStatusTable[LIBUSB_TRANSFER_OVERFLOW + 2] =
	{
		"Transfer completed without error",
		"Transfer failed",
		"Transfer timed out",
		"Transfer was cancelled",
		"Endpoint stalled",
		"Device was disconnected",
		"Device sent more data than requested",
		"Other transfer error",
	};

	if (code > 0)
		return (size_t)code < countof(transferStatusTable) ?
			transferStatusTable[code] :
			transferStatusTable[countof(transferStatusTable) - 1];

	size_t index = -code;

	return index < countof(errorTable) ?
		errorTable[index] :
		errorTable[countof(errorTable) - 1];
}

//..............................................................................

} // namespace io
} // namespace axl
