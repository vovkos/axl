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
#include "axl_io_UsbDeviceStrings.h"
#include "axl_io_UsbDevice.h"

#if (_AXL_OS_WIN)
#	include "axl_io_win_UsbHub.h"
#elif (_AXL_OS_LINUX)
#	include "axl_io_psx_File.h"
#	include "axl_sys_lnx_UdevHwdb.h"
#	include "axl_sys_lnx_UdevListEntry.h"
#elif (_AXL_OS_DARWIN)
#	include "axl_iok_RegistryEntry.h"
#	include "axl_cf_Number.h"
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
chooseUsbStringDescriptorLangId(
	const sl::StringRef_utf16& string,
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

void
UsbDeviceStrings::queryStrings(
	win::UsbHubDb* hubDb,
	sys::win::DeviceInfo* deviceInfo,
	uint_t port,
	uint_t manufacturerDescriptorId,
	uint_t productDescriptorId,
	uint_t serialNumberDescriptorId,
	sl::String_w* string,
	uint_t mask // UsbDeviceStringId-s
) {
	queryStrings(
		(mask & (UsbDeviceStringId_HubInterfacePath | UsbDeviceStringId_Descriptors)) ?
			hubDb->getUsbHub(deviceInfo) :
			NULL,
		deviceInfo,
		port,
		manufacturerDescriptorId,
		productDescriptorId,
		serialNumberDescriptorId,
		string,
		mask
	);
}

void
UsbDeviceStrings::queryStrings(
	const win::UsbHub* hub,
	sys::win::DeviceInfo* deviceInfo,
	uint_t port,
	uint_t manufacturerDescriptorId,
	uint_t productDescriptorId,
	uint_t serialNumberDescriptorId,
	sl::String_w* string,
	uint_t mask
) {
	bool result;

	if (mask & UsbDeviceStringId_Description) {
		result = deviceInfo->getDeviceRegistryProperty(SPDRP_FRIENDLYNAME, string);
		if (!result)
			result = deviceInfo->getDeviceRegistryProperty(SPDRP_DEVICEDESC, string);

		if (result)
			m_description = *string;
	}

	if (mask & UsbDeviceStringId_Manufacturer) {
		result = deviceInfo->getDeviceRegistryProperty(SPDRP_MFG, string);
		if (result)
			m_manufacturer = *string;
	}

	if (mask & UsbDeviceStringId_Driver) {
		result = deviceInfo->getDeviceDriverPath(string);
		if (result)
			m_driver = *string;
	}

	if (!hub)
		return; // the further strings require a hub

	if (mask & UsbDeviceStringId_HubInterfacePath)
		m_hubInterfacePath = hub->getPath();

	if (mask & UsbDeviceStringId_Descriptors) {
		result = hub->getStringDescriptor(string, port, 0, 0);
		ushort_t langId = result ?
			chooseUsbStringDescriptorLangId(
				sl::StringRef_utf16((utf16_t*)string->cp(), string->getLength())
			) : 0;

		if ((mask & UsbDeviceStringId_ManufacturerDescriptor) && manufacturerDescriptorId) {
			result = hub->getStringDescriptor(
				string,
				port,
				manufacturerDescriptorId,
				langId
			);

			if (result)
				m_manufacturerDescriptor = *string;
		}

		if ((mask & UsbDeviceStringId_ProductDescriptor) && productDescriptorId) {
			result = hub->getStringDescriptor(
				string,
				port,
				productDescriptorId,
				langId
			);

			if (result)
				m_productDescriptor = *string;
		}

		if ((mask & UsbDeviceStringId_SerialNumberDescriptor) && serialNumberDescriptorId) {
			result = hub->getStringDescriptor(
				string,
				port,
				serialNumberDescriptorId,
				langId
			);

			if (result)
				m_serialNumberDescriptor = *string;
		}

		if (mask & UsbDeviceStringId_ConfigDescriptor)
			hub->getConfigurationDescriptor(&m_configDescriptor, port);
	}
}

#elif (_AXL_OS_LINUX)

sl::String
getHwdbString(
	const sys::lnx::UdevHwdb& hwdb,
	const sl::StringRef& modalias,
	const sl::StringRef& property
) {
	sys::lnx::UdevListEntry it = hwdb.getProperties(modalias);
	for (; it; it++) {
		sl::StringRef name = it.getName();
		if (name == property)
			return it.getValue();
	}

	return sl::String();
}

enum {
	MaxUsbDepth            = 8,
	MaxSysFsPropertyLength = 256,
};

bool
getUsbDeviceSysFsPrefix(
	sl::String* prefix,
	const io::UsbDevice* device
) {
	prefix->clear();

	uint8_t path[MaxUsbDepth] = { 0 };
#if (_AXL_IO_USBDEVICE_GETPORTNUMBERS)
	size_t depth = device->getPortNumbers(path, countof(path));
#elif (_AXL_IO_USBDEVICE_GETPORTPATH)
	size_t depth = device->getPortPath(path, countof(path));
#endif

	if (depth == -1)
		return false;

	prefix->format("/sys/bus/usb/devices/%d-%d", device->getBusNumber(), path[0]);
	for (size_t i = 1; i < depth; i++)
		prefix->appendFormat(".%d", path[i]);

	prefix->append('/');
	return true;
}

bool
readSysFsProperty(
	sl::String* value,
	const sl::StringRef& path
) {
	value->clear();

	io::psx::File file;
	bool result = file.open(path, O_RDONLY);
	if (!result) {
		value->clear();
		return false;
	}

	char* p = value->createBuffer(MaxSysFsPropertyLength);
	size_t size = file.read(p, MaxSysFsPropertyLength);
	if (size == -1) {
		value->clear();
		return false;
	}

	value->overrideLength(size);
	value->trimRight();
	return true;
}

void
UsbDeviceStrings::queryStrings(
	const sys::lnx::UdevHwdb& hwdb,
	io::UsbDevice* device,
	const libusb_device_descriptor& deviceDescriptor,
	sl::String* sysFsPrefix,
	sl::String* string,
	sl::String_utf16* string_utf16,
	uint_t mask
) {
	getUsbDeviceSysFsPrefix(sysFsPrefix, device);

	if (mask & UsbDeviceStringId_Description) {
		if (hwdb.isOpen()) {
			string->format("usb:v%04Xp%04X*", deviceDescriptor.idVendor, deviceDescriptor.idProduct);
			m_description = getHwdbString(hwdb, *string, "ID_MODEL_FROM_DATABASE");
		}

		if (m_description.isEmpty()) {
			string->forceCopy(*sysFsPrefix);
			string->append("product");
			readSysFsProperty(&m_description, *string);
			m_productDescriptor = m_description;
		}
	}

	if (mask & UsbDeviceStringId_Manufacturer) {
		if (hwdb.isOpen()) {
			string->format("usb:v%04X*", deviceDescriptor.idVendor);
			m_manufacturer = getHwdbString(hwdb, *string, "ID_VENDOR_FROM_DATABASE");
		}

		if (m_manufacturer.isEmpty()) {
			string->forceCopy(*sysFsPrefix);
			string->append("manufacturer");
			readSysFsProperty(&m_manufacturer, *string);
			m_manufacturerDescriptor = m_manufacturer;
		}
	}

	if ((mask & UsbDeviceStringId_ManufacturerDescriptor) && m_manufacturerDescriptor.isEmpty()) {
		string->forceCopy(*sysFsPrefix);
		string->append("manufacturer");
		readSysFsProperty(&m_manufacturerDescriptor, *string);
	}

	if ((mask & UsbDeviceStringId_ProductDescriptor) && m_productDescriptor.isEmpty()) {
		string->forceCopy(*sysFsPrefix);
		string->append("product");
		readSysFsProperty(&m_productDescriptor, *string);
	}

	if (mask & UsbDeviceStringId_SerialNumberDescriptor) {
		string->forceCopy(*sysFsPrefix);
		string->append("serial");
		readSysFsProperty(&m_serialNumberDescriptor, *string);
	}
}

#elif (_AXL_OS_DARWIN)

void
UsbDeviceStrings::queryStrings(
	const cf::MutableDictionary& usbDeviceDict,
	cf::MutableDictionary* propertyMatchDict,
	io::UsbDevice* device,
	const libusb_device_descriptor& deviceDescriptor,
	sl::String* string,
	sl::String_utf16* string_utf16,
	uint_t mask
) {
	iok::RegistryEntry registryEntry;

#if (_AXL_IO_USBDEVICE_GETSESSIONDATA)
	int64_t sessionId = device->getSessionData();
	propertyMatchDict->setValue(CFSTR("sessionID"), cf::Number(kCFNumberSInt64Type, &sessionId));

	cf::Type prop;
	iok::Iterator it = iok::findMatchingServices(usbDeviceDict);
	registryEntry = it.next().p();
	if (registryEntry) {
		if (mask & (UsbDeviceStringId_Description | UsbDeviceStringId_ProductDescriptor)) {
			prop = registryEntry.getProperty(kUSBProductString);
			m_description = m_productDescriptor = prop ? prop.toString() : registryEntry.getName();
		}

		if (mask & (UsbDeviceStringId_Manufacturer | UsbDeviceStringId_ManufacturerDescriptor)) {
			prop = registryEntry.getProperty(kUSBVendorString);
			if (prop)
				m_manufacturer = m_manufacturerDescriptor = prop.toString();
		}

		if (mask & UsbDeviceStringId_Driver) {
			prop = registryEntry.getProperty("CFBundleIdentifier");
			if (prop)
				m_driver = prop.toString();
		}

		if (mask & UsbDeviceStringId_SerialNumberDescriptor) {
			prop = registryEntry.getProperty(kUSBSerialNumberString);
			if (prop)
				m_serialNumberDescriptor = prop.toString();
		}

		return;
	}
#endif

	if (!(mask & UsbDeviceStringId_Descriptors))
		return;

	// fallback to fetching descriptors directly from the device

	bool result =
		(device->isOpen() || device->open()) &&
		device->getStringDescriptor(string_utf16, 0, 0) != -1;

	if (!result)
		return;

	ushort_t langId = chooseUsbStringDescriptorLangId(*string_utf16);

	if ((mask & UsbDeviceStringId_ManufacturerDescriptor) && deviceDescriptor.iManufacturer) {
		result = device->getStringDescriptor(string_utf16, deviceDescriptor.iManufacturer, langId) != -1;
		if (result)
			m_manufacturerDescriptor = *string_utf16;
	}

	if ((mask & UsbDeviceStringId_ProductDescriptor) && deviceDescriptor.iProduct) {
		result = device->getStringDescriptor(string_utf16, deviceDescriptor.iProduct, langId) != -1;
		if (result)
			m_productDescriptor = *string_utf16;
	}

	if ((mask & UsbDeviceStringId_SerialNumberDescriptor) && deviceDescriptor.iSerialNumber) {
		result = device->getStringDescriptor(string_utf16, deviceDescriptor.iSerialNumber, langId) != -1;
		if (result)
			m_serialNumberDescriptor = *string_utf16;
	}
}

#endif

//..............................................................................

} // namespace io
} // namespace axl
