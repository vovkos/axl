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
#include "axl_io_UsbMonTransfer.h"

namespace axl {
namespace io {

//..............................................................................

const char*
getUsbMonTransferTypeString(UsbMonTransferType transferType) {
	static const char* stringTable[] = {
		"Isochronous", // 0
		"Interrupt",   // 1
		"Control",     // 2
		"Bulk",        // 3
	};

	return (size_t)transferType < countof(stringTable) ?
		stringTable[transferType] :
		"Unknown";
}

//..............................................................................

} // namespace io
} // namespace axl
