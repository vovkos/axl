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
#include "axl_io_UsbMonEnumerator.h"

namespace axl {
namespace io {

//..............................................................................

const char*
getUsbMonSpeedString(UsbMonDeviceSpeed speed) {
	static const char* stringTable[] = {
		"Unknown",                  // UsbMonDeviceSpeed_Unknown
		"Low Speed (1.5 Mbit/s)",   // UsbMonDeviceSpeed_Low,
		"Full Speed (12 Mbit/s)",   // UsbMonDeviceSpeed_Full,
		"Hi-Speed (480 Mbit/s)",    // UsbMonDeviceSpeed_High,
		"SuperSpeed (5000 Mbit/s)", // UsbMonDeviceSpeed_Super,
	};

	return (size_t)speed < countof(stringTable) ?
		stringTable[speed] :
		stringTable[UsbMonDeviceSpeed_Unknown];
}

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

sl::StringRef
getUsbMonTransferFlagsString(uint_t flags) {
	return (flags & UsbMonTransferFlag_Completed) ?
	   sl::StringRef("Completed") :
	   sl::StringRef();
}

const char*
getUsbMonControlRecipientString(UsbMonControlRecipient recipient) {
	static const char* stringTable[] = {
		"Device",     // 0
		"Interface",  // 1
		"Endpoint",   // 2
		"Other",      // 3
	};

	return (size_t)recipient < countof(stringTable) ?
		stringTable[recipient] :
		"Unknown";
}

const char*
getUsbMonControlTypeString(UsbMonControlType type) {
	static const char* stringTable[] = {
		"Standard", // 0
		"Class",    // 1
		"Vendor",   // 2
	};

	return (size_t)type < countof(stringTable) ?
		stringTable[type] :
		"Unknown";
}

const char*
getUsbMonControlDirectionString(UsbMonControlDirection direction) {
	static const char* stringTable[] = {
		"Out", // 0
		"In",  // 1
	};

	return (size_t)direction < countof(stringTable) ?
		stringTable[direction] :
		"Unknown";
}

const char*
getUsbMonControlStdRequestString(UsbMonControlStdRequest request) {
	static const char* stringTable[] = {
		"GetStatus",         // 0
		"ClearFeature",      // 1
		"Reserved",          // 2
		"SetFeature",        // 3
		"Reserved2",         // 4
		"SetAddress",        // 5
		"GetDescriptor",     // 6
		"SetDescriptor",     // 7
		"GetConfiguration",  // 8
		"SetConfiguration",  // 9
		"GetInterface",      // 10
		"SetInterface",      // 11
		"SynchFrame",        // 12
	};

	return request < countof(stringTable) ? stringTable[request] : "Unknown";
}

//..............................................................................

} // namespace io
} // namespace axl
