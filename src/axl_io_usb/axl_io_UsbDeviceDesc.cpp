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
#include "axl_io_UsbDeviceDesc.h"

#if (_AXL_OS_WIN)
#	include "axl_io_win_UsbHub.h"
#endif

namespace axl {
namespace io {

//..............................................................................

const char*
getUsbClassString(uint_t usbClass) {
	// for updates, visit https://usb.org/defined-class-codes

	static const char* stringTable[] = {
		"Multi-function",      // 0x00
		"Audio",               // 0x01
		"Communication",       // 0x02
		"HID",                 // 0x03
		"Unknown",             // 0x04
		"Physical",            // 0x05
		"Image",               // 0x06
		"Printer",             // 0x07
		"Mass Storage",        // 0x08
		"Hub",                 // 0x09
		"CDC-Data",            // 0x0a
		"Smart Card",          // 0x0b
		"Unknown",             // 0x0c
		"Content Security",    // 0x0d
		"Video",               // 0x0e
		"Personal Healthcare", // 0x0f
		"Audio/Video",         // 0x10
		"Billboard",           // 0x11
		"USB Type-C Bridge",   // 0x12
	};

	if ((size_t)usbClass < countof(stringTable))
		return stringTable[usbClass];

	switch (usbClass) {
	case 0x3c:
		return "I3C";

	case 0xdc:
		return "Diagnostic";

	case 0xe0:
		return "Wireless Controller";

	case 0xef:
		return "Miscellaneous";

	case 0xfe:
		return "Application-specific";

	case 0xff:
		return "Vendor-specific";

	default:
		return "Unknown";
	}
}

const char*
getUsbSpeedString(libusb_speed speed) {
	static const char* stringTable[] = {
		"Unknown speed",             // LIBUSB_SPEED_UNKNOWN    = 0,
		"Low-Speed (1.5 Mbps)",      // LIBUSB_SPEED_LOW        = 1,
		"Full-Speed (12 Mbps)",      // LIBUSB_SPEED_FULL       = 2,
		"Hi-Speed (480 Mbps)",       // LIBUSB_SPEED_HIGH       = 3,
		"SuperSpeed (5 Gbps)",       // LIBUSB_SPEED_SUPER      = 4,
		"SuperSpeed Plus (10 Gbps)", // LIBUSB_SPEED_SUPER_PLUS = 5,
	};

	return (size_t)speed < countof(stringTable) ?
		stringTable[speed] :
		stringTable[0];
}

//..............................................................................

static
ushort_t
chooseUsbStringDescriptorLanguage(
	const sl::String_utf16& string,
	uint_t preferredLangId = 0x09 // English (pass 0 to pick the very first)
) {
	size_t length = string.getLength();
	if (!length)
		return 0;

	ushort_t firstLangId = string[0];
	if (!preferredLangId || (firstLangId & 0xff) == preferredLangId)
		return firstLangId;

	for (size_t i = 1; i < length; i++) {
		ushort_t langId = string[i];
		if ((langId & 0xff) == preferredLangId)
			return langId;
	}

	return firstLangId;
}

#if (_AXL_OS_WIN)
bool
UsbDeviceDesc::fetch(
	win::UsbHubDb* hubDb,
	sys::win::DeviceInfo* deviceInfo,
	uint_t port,
	sl::String_w* string,
	uint_t flags
) {
	bool result;

	if (flags & UsbDeviceDescFlag_Description) {
		result = deviceInfo->getDeviceRegistryProperty(SPDRP_FRIENDLYNAME, string);
		if (!result)
			result = deviceInfo->getDeviceRegistryProperty(SPDRP_DEVICEDESC, string);

		if (result)
			m_description = *string;
	}

	if (flags & UsbDeviceDescFlag_Manufacturer) {
		result = deviceInfo->getDeviceRegistryProperty(SPDRP_MFG, string);
		if (result)
			m_manufacturer = *string;
	}

	if (flags & UsbDeviceDescFlag_Driver) {
		result = deviceInfo->getDeviceDriverPath(string);
		if (result)
			m_driver = *string;
	}

	if (flags & UsbDeviceDescFlag_Descriptors) {
		win::UsbHub* hub = hubDb->getUsbHub(deviceInfo);
		if (!hub)
			return false; // can't fetch descriptors without a hub

		result = hub->getStringDescriptor(string, port, 0, 0);
		ushort_t languageId = result ?
			chooseUsbStringDescriptorLanguage(
				sl::String_utf16((utf16_t*)string->cp(), string->getLength())
			) : 0;

		if (m_manufacturerDescriptorId) {
			result = hub->getStringDescriptor(
				string,
				port,
				m_manufacturerDescriptorId,
				languageId
			);

			if (result)
				m_manufacturerDescriptor = *string;
		}

		if (m_productDescriptorId) {
			result = hub->getStringDescriptor(
				string,
				port,
				m_productDescriptorId,
				languageId
			);

			if (result)
				m_productDescriptor = *string;
		}

		if (m_serialNumberDescriptorId) {
			result = hub->getStringDescriptor(
				string,
				port,
				m_serialNumberDescriptorId,
				languageId
			);

			if (result)
				m_serialNumberDescriptor = *string;
		}
	}

	return true;
}
#endif

//..............................................................................

} // namespace io
} // namespace axl
