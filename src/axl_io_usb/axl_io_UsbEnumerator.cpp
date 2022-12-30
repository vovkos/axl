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
	sl::String_w buffer;
	win::UsbHubDb hubDb;
	sys::win::DeviceInfoSet deviceInfoSet;
	deviceInfoSet.create(DIGCF_DEVICEINTERFACE);
#endif

	libusb_device** pp = *loDeviceList;
	for (; *pp; pp++) {
		UsbDevice device(*pp);
		libusb_device_descriptor descriptor;
		bool result = device.getDeviceDescriptor(&descriptor);
		if (!result)
			continue;

		if (!(flags & UsbEnumFlag_Hubs) && descriptor.bDeviceClass == 0x09)
			continue;

		UsbDeviceEntry* deviceEntry = AXL_MEM_NEW(UsbDeviceEntry);
		deviceEntry->m_vendorId = descriptor.idVendor;
		deviceEntry->m_productId = descriptor.idProduct;
		deviceEntry->m_address = device.getDeviceAddress();
		deviceEntry->m_port = device.getPortNumber();
		deviceEntry->m_class = descriptor.bDeviceClass;
		deviceEntry->m_subClass = descriptor.bDeviceSubClass;
		deviceEntry->m_speed = device.getDeviceSpeed();
		deviceEntry->m_manufacturerDescriptorId = descriptor.iManufacturer;
		deviceEntry->m_productDescriptorId = descriptor.iProduct;
		deviceEntry->m_serialNumberDescriptorId = descriptor.iSerialNumber;

#if (_AXL_OS_WIN && _AXL_IO_USBDEVICE_GETSESSIONDATA)
		sys::win::DeviceInfo deviceInfo;
		result = deviceInfoSet.findDeviceInfoByDevInst(device.getSessionData(), &deviceInfo);
		if (result)
			deviceEntry->fetch(&hubDb, &deviceInfo, deviceEntry->m_port, &buffer, flags);
#endif

		deviceList->insertTail(deviceEntry);
	}

	return deviceList->getCount();
}

//..............................................................................

} // namespace io
} // namespace axl
