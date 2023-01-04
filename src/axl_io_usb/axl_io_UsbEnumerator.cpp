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
#include "axl_io_UsbEnumerator.h"
#include "axl_io_UsbDevice.h"

#if (_AXL_OS_WIN)
#	include "axl_io_win_UsbHub.h"
#endif

namespace axl {
namespace io {

//..............................................................................

size_t
enumerateUsbDevices(
	libusb_context* context,
	UsbDeviceList* loDeviceList,
	sl::List<UsbDeviceEntry>* deviceList,
	uint_t flags
) {
	UsbDeviceList tmpDeviceList;
	if (!loDeviceList)
		loDeviceList = &tmpDeviceList;

	deviceList->clear();
	loDeviceList->enumerateDevices(context);

#if (_AXL_OS_WIN)
	sl::String_w string;
	win::UsbHubDb hubDb;
	sys::win::DeviceInfoSet deviceInfoSet;
	deviceInfoSet.create(DIGCF_DEVICEINTERFACE);
#elif (_AXL_OS_LINUX)
	sl::String sysFsPrefix;
	sl::String string;
	sl::String_utf16 string_utf16;

	sys::lnx::Udev udev;
	sys::lnx::UdevHwdb hwdb;

	bool result =
		udev.create() &&
		hwdb.create(udev);

	if (!result)
		AXL_TRACE("WARNING: can't open libudev hwdb: %s\n", err::getLastErrorDescription().sz());
#elif (_AXL_OS_DARWIN)
	sl::String string;
	sl::String_utf16 string_utf16;

	cf::MutableDictionary usbDeviceDict = iok::createServiceMatchingDictionary(kIOUSBDeviceClassName);
	cf::MutableDictionary propertyMatchDict;
	propertyMatchDict.create();
	usbDeviceDict.setValue(CFSTR(kIOPropertyMatchKey), propertyMatchDict);
#endif

	libusb_device** pp = *loDeviceList;
	for (; *pp; pp++) {
		UsbDevice device(*pp);
		libusb_device_descriptor deviceDescriptor;
		bool result = device.getDeviceDescriptor(&deviceDescriptor);
		if (!result)
			continue;

		if (!(flags & UsbEnumFlag_Hubs) && deviceDescriptor.bDeviceClass == 0x09)
			continue;

		UsbDeviceEntry* deviceEntry = AXL_MEM_NEW(UsbDeviceEntry);
		deviceEntry->m_vendorId = deviceDescriptor.idVendor;
		deviceEntry->m_productId = deviceDescriptor.idProduct;
		deviceEntry->m_address = device.getDeviceAddress();
		deviceEntry->m_port = device.getPortNumber();
		deviceEntry->m_class = deviceDescriptor.bDeviceClass;
		deviceEntry->m_subClass = deviceDescriptor.bDeviceSubClass;
		deviceEntry->m_speed = device.getDeviceSpeed();
		deviceEntry->m_manufacturerDescriptorId = deviceDescriptor.iManufacturer;
		deviceEntry->m_productDescriptorId = deviceDescriptor.iProduct;
		deviceEntry->m_serialNumberDescriptorId = deviceDescriptor.iSerialNumber;

#if (_AXL_OS_WIN)
#	if (_AXL_IO_USBDEVICE_GETSESSIONDATA)
		sys::win::DeviceInfo deviceInfo;
		result = deviceInfoSet.findDeviceInfoByDevInst(device.getSessionData(), &deviceInfo);
		if (result)
			deviceEntry->queryStrings(
				&hubDb,
				&deviceInfo,
				deviceEntry->m_port,
				&string,
				flags
			);
#	endif
#elif (_AXL_OS_LINUX)
		deviceEntry->queryStrings(
			hwdb,
			&device,
			deviceDescriptor,
			&sysFsPrefix,
			&string,
			&string_utf16,
			flags
		);
#elif (_AXL_OS_DARWIN)
		deviceEntry->queryStrings(
			usbDeviceDict,
			&propertyMatchDict,
			&device,
			deviceDescriptor,
			&string,
			&string_utf16,
			flags
		);
#endif

		deviceEntry->m_device = device.getDevice();
		deviceList->insertTail(deviceEntry);
	}

	return deviceList->getCount();
}

//..............................................................................

} // namespace io
} // namespace axl
