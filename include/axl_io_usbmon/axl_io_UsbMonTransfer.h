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

#include "axl_sl_String.h"

namespace axl {
namespace io {
namespace win {

const char*
getUrbFunctionString(uint_t function);

const char*
getUsbdStatusString(uint_t status);

} // namespace win

//..............................................................................

enum UsbMonTransferFlag {
	UsbMonTransferFlag_Completed = 0x01,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::StringRef
getUsbMonTransferFlagsString(uint_t flags);

//..............................................................................

enum UsbMonControlRecipient {
	UsbMonControlRecipient_Device = 0,
	UsbMonControlRecipient_Interface,
	UsbMonControlRecipient_Endpoint,
	UsbMonControlRecipient_Other,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getUsbMonControlRecipientString(UsbMonControlRecipient recipient);

//..............................................................................

enum UsbMonControlType {
	UsbMonControlType_Standard = 0,
	UsbMonControlType_Class,
	UsbMonControlType_Vendor,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getUsbMonControlTypeString(UsbMonControlType type);

//..............................................................................

enum UsbMonControlDirection {
	UsbMonControlDirection_Out = 0,
	UsbMonControlDirection_In,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getUsbMonControlDirectionString(UsbMonControlDirection direction);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum UsbMonControlStdRequest {
	UsbMonControlStdRequest_GetStatus,         // 0
	UsbMonControlStdRequest_ClearFeature,      // 1
	UsbMonControlStdRequest_Reserved,          // 2
	UsbMonControlStdRequest_SetFeature,        // 3
	UsbMonControlStdRequest_Reserved2,         // 4
	UsbMonControlStdRequest_SetAddress,        // 5
	UsbMonControlStdRequest_GetDescriptor,     // 6
	UsbMonControlStdRequest_SetDescriptor,     // 7
	UsbMonControlStdRequest_GetConfiguration,  // 8
	UsbMonControlStdRequest_SetConfiguration,  // 9
	UsbMonControlStdRequest_GetInterface,      // 10
	UsbMonControlStdRequest_SetInterface,      // 11
	UsbMonControlStdRequest_SynchFrame,        // 12
};

const char*
getUsbMonControlStdRequestString(UsbMonControlStdRequest request);

//..............................................................................

struct UsbMonControlRequestType {
	union {
		uint8_t m_value;
		struct {
			uint8_t m_recipient : 2; // UsbMonControlRecipient
			uint8_t m_reserved  : 3;
			uint8_t m_type      : 2; // UsbMonControlType
			uint8_t m_direction : 1; // UsbMonControlDirection
		};
	};
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct UsbMonControlSetup {
	UsbMonControlRequestType m_requestType;
	uint8_t m_request;
	uint16_t m_value;
	uint16_t m_index;
	uint16_t m_length;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct UsbMonIsochronousHdr {
	uint32_t m_startFrame;
	uint32_t m_packetCount;
	uint32_t m_errorCount;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct UsbMonTransferHdr {
	uint64_t m_id;
	uint64_t m_timestamp;
	uint32_t m_status;       // USBD_STATUS on Windows, errno on Linux
	uint16_t m_urbFunction;  // URB_FUNCTION_XXX on Windows
	uint16_t m_flags;        // UsbMonTransferFlag
	uint8_t m_transferType;  // libusb_transfer_type or -1
	uint8_t m_bus;
	uint8_t m_address;
	uint8_t m_endpoint;
	uint32_t m_originalSize; // original payload size
	uint32_t m_captureSize;  // payload captured by the driver

	union {
		UsbMonControlSetup m_controlSetup;
		UsbMonIsochronousHdr m_isochronousHdr;
	};
}; // 48 bytes

//..............................................................................

} // namespace io
} // namespace axl
