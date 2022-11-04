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
#include "axl_io_UsbMonEnumerator.h"
#include "axl_io_UsbStringDescriptorLanguage.h"
#include "axl_io_UsbDevice.h"
#include "axl_io_FilePathUtils.h"
#include "axl_io_psx_File.h"
#include "axl_sys_lnx_UdevHwdb.h"
#include "axl_sys_lnx_UdevListEntry.h"

namespace axl {
namespace io {

//..............................................................................

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

#if (_AXL_IO_USBDEVICE_GETPORTPATH || _AXL_IO_USBDEVICE_GETPORTNUMBERS)
#	define _AXL_IO_USBMON_USE_SYSFS 1
#endif

#if (_AXL_IO_USBMON_USE_SYSFS)

enum {
	MaxUsbDepth            = 8,
	MaxSysFsPropertyLength = 256,
};

bool
getUsbDeviceSysFsPrefix(
	sl::String* prefix,
	const io::UsbDevice& device
) {
	prefix->clear();

	uint8_t path[MaxUsbDepth] = { 0 };
#if (_AXL_IO_USBDEVICE_GETPORTNUMBERS)
	size_t depth = device.getPortNumbers(path, countof(path));
#elif (_AXL_IO_USBDEVICE_GETPORTPATH)
	size_t depth = device.getPortPath(path, countof(path));
#endif

	if (depth == -1)
		return false;

	prefix->format("/sys/bus/usb/devices/%d-%d", device.getBusNumber(), path[0]);
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

#endif // _AXL_IO_USBMON_USE_SYSFS

size_t
enumerateUsbMonDevices(
	sl::List<UsbMonDeviceDesc>* resultList,
	uint_t mask
) {
	sys::lnx::Udev udev;
	sys::lnx::UdevHwdb hwdb;

	bool result =
		udev.create() &&
		hwdb.create(udev);

	if (!result)
		AXL_TRACE("WARNING: can't open libudev hwdb: %s\n", err::getLastErrorDescription().sz());

	UsbDeviceList list;
	size_t count = list.enumerateDevices();
	if (count == -1)
		return -1;

	sl::String string;
	sl::String_utf16 string_utf16;

	libusb_device** pp = list;
	for (; *pp; pp++) {
		UsbDevice device(*pp);

		uint_t busNumber = device.getBusNumber();
		string.format("/dev/usbmon%d", busNumber);
		if (!doesFileExist(string))
			continue; // if usbmon device not found then we can't sniff

		UsbMonDeviceDesc* deviceDesc = AXL_MEM_NEW(UsbMonDeviceDesc);
		deviceDesc->m_captureDeviceName = string;
		deviceDesc->m_address = device.getDeviceAddress();
		deviceDesc->m_speed = (UsbMonDeviceSpeed)device.getDeviceSpeed();
		resultList->insertTail(deviceDesc);

		libusb_device_descriptor deviceDescriptor;
		result = device.getDeviceDescriptor(&deviceDescriptor);
		if (!result)
			continue;

		deviceDesc->m_vendorId = deviceDescriptor.idVendor;
		deviceDesc->m_productId = deviceDescriptor.idProduct;
		deviceDesc->m_class = deviceDescriptor.bDeviceClass;
		deviceDesc->m_subClass = deviceDescriptor.bDeviceSubClass;
		deviceDesc->m_manufacturerDescriptorId = deviceDescriptor.iManufacturer;
		deviceDesc->m_productDescriptorId = deviceDescriptor.iProduct;
		deviceDesc->m_serialNumberDescriptorId = deviceDescriptor.iSerialNumber;
		deviceDesc->m_flags |= UsbMonDeviceDescFlag_DeviceDescriptor;

		if (mask & UsbMonDeviceDescMask_Description) {
			if (hwdb.isOpen()) {
				string.format("usb:v%04Xp%04X*", deviceDescriptor.idVendor, deviceDescriptor.idProduct);
				deviceDesc->m_description = getHwdbString(hwdb, string, "ID_MODEL_FROM_DATABASE");
			}

#if (_AXL_IO_USBMON_USE_SYSFS)
			if (deviceDesc->m_description.isEmpty()) {
				getUsbDeviceSysFsPrefix(&string, device);
				string += "product";
				readSysFsProperty(&deviceDesc->m_description, string);
			}
#endif

			if (!deviceDesc->m_description.isEmpty())
				deviceDesc->m_flags |= UsbMonDeviceDescFlag_Description;
		}

		if (mask & UsbMonDeviceDescMask_Manufacturer) {
			if (hwdb.isOpen()) {
				string.format("usb:v%04X*", deviceDescriptor.idVendor);
				deviceDesc->m_manufacturer = getHwdbString(hwdb, string, "ID_VENDOR_FROM_DATABASE");
			}

#if (_AXL_IO_USBMON_USE_SYSFS)
			if (deviceDesc->m_manufacturer.isEmpty()) {
				getUsbDeviceSysFsPrefix(&string, device);
				string += "manufacturer";
				readSysFsProperty(&deviceDesc->m_manufacturer, string);
			}
#endif

			if (!deviceDesc->m_manufacturer.isEmpty())
				deviceDesc->m_flags |= UsbMonDeviceDescFlag_Manufacturer;
		}

		if (!(mask & UsbMonDeviceDescMask_Descriptors))
			continue;

		result = device.open();
		if (!result)
			continue;

		device.getStringDescriptor(&string_utf16, 0, 0);
		ushort_t langId = chooseUsbStringDescriptorLanguage(string_utf16);

		if ((mask & UsbMonDeviceDescMask_ManufacturerDescriptor) && deviceDescriptor.iManufacturer) {
			result = device.getStringDescriptor(&string_utf16, deviceDescriptor.iManufacturer, langId);
			if (result) {
				deviceDesc->m_manufacturerDescriptor = string_utf16;
				deviceDesc->m_flags |= UsbMonDeviceDescFlag_ManufacturerDescriptor;
			}
		}

		if ((mask & UsbMonDeviceDescMask_ProductDescriptor) && deviceDescriptor.iProduct) {
			result = device.getStringDescriptor(&string_utf16, deviceDescriptor.iProduct, langId);
			if (result) {
				deviceDesc->m_productDescriptor = string_utf16;
				deviceDesc->m_flags |= UsbMonDeviceDescFlag_ProductDescriptor;
			}
		}

		if ((mask & UsbMonDeviceDescMask_SerialNumberDescriptor) && deviceDescriptor.iSerialNumber) {
			result = device.getStringDescriptor(&string_utf16, deviceDescriptor.iSerialNumber, langId);
			if (result) {
				deviceDesc->m_serialNumberDescriptor = string_utf16;
				deviceDesc->m_flags |= UsbMonDeviceDescFlag_SerialNumberDescriptor;
			}
		}
	}

	return resultList->getCount();
}

//..............................................................................

} // namespace io
} // namespace axl
