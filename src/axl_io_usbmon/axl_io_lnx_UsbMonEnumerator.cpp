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
#include "axl_io_UsbDevice.h"
#include "axl_io_FilePathUtils.h"
#include "axl_sys_lnx_UdevHwdb.h"

namespace axl {
namespace io {

//..............................................................................

size_t
enumerateUsbMonDevices(
	sl::List<UsbMonDeviceDesc>* resultList,
	uint_t flags
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

	sl::String sysFsPrefix;
	sl::String string;
	sl::String_utf16 string_utf16;

	libusb_device** pp = list;
	for (; *pp; pp++) {
		UsbDevice device(*pp);

		uint_t busNumber = device.getBusNumber();
		string.format("/dev/usbmon%d", busNumber);
		if (!doesFileExist(string))
			continue; // usbmon device not found, can't sniff

		libusb_device_descriptor deviceDescriptor;
		result = device.getDeviceDescriptor(&deviceDescriptor);
		if (!result)
			continue;

		if (deviceDescriptor.bDeviceClass == LIBUSB_CLASS_HUB && !(flags & UsbMonEnumFlag_Hubs))
			continue;

		UsbMonDeviceDesc* deviceDesc = new UsbMonDeviceDesc;
		deviceDesc->m_captureDeviceName = string;
		deviceDesc->m_captureDeviceId = busNumber;
		deviceDesc->m_vendorId = deviceDescriptor.idVendor;
		deviceDesc->m_productId = deviceDescriptor.idProduct;
		deviceDesc->m_address = device.getDeviceAddress();
		deviceDesc->m_port = device.getPortNumber();
		deviceDesc->m_class = deviceDescriptor.bDeviceClass;
		deviceDesc->m_subClass = deviceDescriptor.bDeviceSubClass;
		deviceDesc->m_speed = device.getDeviceSpeed();
		deviceDesc->m_manufacturerDescriptorId = deviceDescriptor.iManufacturer;
		deviceDesc->m_productDescriptorId = deviceDescriptor.iProduct;
		deviceDesc->m_serialNumberDescriptorId = deviceDescriptor.iSerialNumber;

		deviceDesc->queryStrings(
			hwdb,
			&device,
			deviceDescriptor,
			&sysFsPrefix,
			&string,
			&string_utf16,
			flags
		);

		resultList->insertTail(deviceDesc);
	}

	return resultList->getCount();
}

//..............................................................................

} // namespace io
} // namespace axl
