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

#define _AXL_IO_USBMONTRANSFER_H

#include "axl_g_Pch.h"

namespace axl {
namespace io {

//..............................................................................

enum UsbMonTransferType {
	UsbMonTransferType_Isochronous, // 0
	UsbMonTransferType_Interrupt,   // 1
	UsbMonTransferType_Control,     // 2
	UsbMonTransferType_Bulk,        // 3
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getUsbMonTransferTypeString(UsbMonTransferType transferType);

//..............................................................................

enum UsbMonTransferFlag {
	UsbMonTransferFlag_Completed = 0x01,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct UsbMonTransferHdr {
	uint64_t m_timestamp;
	uint32_t m_status;
	uint32_t m_dataSize;
	uint8_t m_transferType;
	uint8_t m_bus;
	uint8_t m_address;
	uint8_t m_endpoint;
	uint32_t m_flags;

	union {
		uint8_t m_controlSetup[8];
		struct {
			uint32_t m_startFrame;
			uint32_t m_packetCount;
			uint32_t m_errorCount;
			uint32_t m_padding;
		} m_iso;
	};

	uint64_t m_padding; // align on 16 bytes
};

//..............................................................................

} // namespace io
} // namespace axl
