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
#include "axl_io_HidMonEnumerator.h"
#include "axl_io_UsbDevice.h"
#include "axl_io_HidRdParser.h"
#include "axl_io_FilePathUtils.h"
#include "axl_io_MappedFile.h"
#include "axl_sys_lnx_UdevEnumerate.h"
#include "axl_sys_lnx_UdevListEntry.h"
#include "axl_sys_lnx_UdevDevice.h"
#include "axl_sys_lnx_UdevHwdb.h"

#include <linux/input.h>

#if (_AXL_DEBUG)
#	define _AXL_IO_LNX_PRINT_DEVICE_PROPERTIES 0
#endif

namespace axl {
namespace io {

//..............................................................................

#if (_AXL_IO_LNX_PRINT_DEVICE_PROPERTIES)

void
printDeviceInfo(const sys::lnx::UdevDevice& device) {
	printf("getDevPath: %s\n", device.getDevPath().sz());
	printf("getSysPath: %s\n", device.getSysPath().sz());
	printf("getSysName: %s\n", device.getSysName().sz());
	printf("getSysNum: %s\n", device.getSysNum().sz());
	printf("getDevPath: %s\n", device.getDevPath().sz());
	printf("getDevNode: %s\n", device.getDevNode().sz());
	printf("getDevType: %s\n", device.getDevType().sz());
	printf("getSubsystem: %s\n", device.getSubsystem().sz());
	printf("getDriver: %s\n", device.getDriver().sz());
	printf("getAction: %s\n", device.getAction().sz());

	sys::lnx::UdevListEntry it = device.getPropertyList();
	for (; it; it++)
		printf("property[%s] = '%s'\n", it.getName().sz(), it.getValue().sz());

	it = device.getTagList();
	for (; it; it++)
		printf("tag: %s\n", it.getName().sz());

	it = device.getSysAttrList();
	for (; it; it++)
		printf("sysattr[%s] = '%s'\n", it.getName().sz(), device.getSysAttrValue(it.getName()).sz());

	it = device.getDevLinkList();
	for (; it; it++)
		printf("devlink: %s\n", it.getName().sz());
}

#endif

size_t
readFile(
	sl::Array<char>* buffer,
	const sl::StringRef& fileName
) {
	io::File file;
	size_t size;

	return
		file.open(
			fileName,
		io::FileFlag_OpenExisting |
			io::FileFlag_ReadOnly |
			io::FileFlag_ShareWrite
		) &&
		(size = file.getSize()) != -1 &&
		buffer->setCount(size) &&
		(size = file.read(buffer->p(), size)) != -1 &&
		buffer->setCount(size) ?
			size :
			-1;
}


size_t
enumerateHidMonDevices(
	sl::List<HidMonDeviceDesc>* deviceList,
	uint_t flags
) {
	sys::lnx::Udev udev;
	sys::lnx::UdevEnumerate enumerator;
	sys::lnx::UdevHwdb hwdb;

	bool result =
		udev.create() &&
		enumerator.create(udev) &&
		enumerator.addMatchSubsystem("hidraw") &&
		enumerator.scanDevices();

	if (!result)
		return false;

	result = hwdb.create(udev);
	if (!result)
		AXL_TRACE("WARNING: can't open libudev hwdb: %s\n", err::getLastErrorDescription().sz());

	sl::String fileName;
	sl::Array<char> descriptors;
	sl::Array<char> reportDescriptor;

	sys::lnx::UdevListEntry it = enumerator.getListEntry();
	for (; it; it++) {
		sys::lnx::UdevDevice rawDevice;
		result = rawDevice.createFromSysPath(udev, it.getName());
		if (!result)
			continue;

		sys::lnx::UdevDevice hidDevice = rawDevice.getParentWithSubsystemDevType("hid");
		sys::lnx::UdevDevice usbDevice = rawDevice.getParentWithSubsystemDevType("usb", "usb_device");
		sys::lnx::UdevDevice ifaceDevice = rawDevice.getParentWithSubsystemDevType("usb", "usb_interface");
		if (!hidDevice || !usbDevice || !ifaceDevice)
			continue;

#if (_AXL_IO_LNX_PRINT_DEVICE_PROPERTIES)
		printf("---hidraw---\n");
		printDeviceInfo(rawDevice);

		printf("---hid---\n");
		printDeviceInfo(hidDevice);

		printf("---usb---\n");
		printDeviceInfo(usbDevice);

		printf("---iface---\n");
		printDeviceInfo(ifaceDevice);
#endif

		fileName = usbDevice.getSysPath();
		fileName += "/descriptors";
		result = readFile(&descriptors, fileName) != -1;
		if (!result)
			continue;

		fileName = hidDevice.getSysPath();
		fileName += "/report_descriptor";
		result = readFile(&reportDescriptor, fileName) != -1;
		if (!result)
			continue;

		uint_t bus = strtoul(usbDevice.getSysAttrValue("busnum").sz(), NULL, 10);
		fileName.format("/dev/usbmon%d", bus);
		if (!doesFileExist(fileName))
			continue; // usbmon device not found, can't sniff

		HidMonDeviceDesc* deviceDesc = new HidMonDeviceDesc;
		deviceDesc->m_hidDeviceName = rawDevice.getDevNode();
		deviceDesc->m_captureDeviceName = fileName;
		deviceDesc->m_captureDeviceId = bus;
		deviceDesc->m_vendorId = strtoul(usbDevice.getSysAttrValue("idVendor").sz(), NULL, 16);
		deviceDesc->m_productId = strtoul(usbDevice.getSysAttrValue("idProduct").sz(), NULL, 16);
		deviceDesc->m_releaseNumber = strtoul(usbDevice.getSysAttrValue("bcdDevice").sz(), NULL, 16);
		deviceDesc->m_address = strtoul(usbDevice.getSysNum().sz(), NULL, 10);
		deviceDesc->m_class = strtoul(ifaceDevice.getSysAttrValue("bInterfaceClass").sz(), NULL, 10);
		deviceDesc->m_subClass = strtoul(ifaceDevice.getSysAttrValue("bInterfaceSubClass").sz(), NULL, 10);
		deviceDesc->m_interfaceId = strtoul(ifaceDevice.getSysAttrValue("bInterfaceNumber").sz(), NULL, 10);
		deviceDesc->m_endpointId = 0;

		uint_t speed = strtoul(usbDevice.getSysAttrValue("speed").sz(), NULL, 10);
		switch (speed) {
		case 1:
			deviceDesc->m_speed = LIBUSB_SPEED_LOW;
			break;

		case 12:
			deviceDesc->m_speed = LIBUSB_SPEED_FULL;
			break;

		case 480:
			deviceDesc->m_speed = LIBUSB_SPEED_HIGH;
			break;

		case 5000:
			deviceDesc->m_speed = LIBUSB_SPEED_SUPER;
			break;

		case 10000:
			deviceDesc->m_speed = LIBUSB_SPEED_SUPER_PLUS;
			break;

		default:
			deviceDesc->m_speed = LIBUSB_SPEED_UNKNOWN;
		}

		if (flags & UsbDeviceStringId_Description)
			deviceDesc->m_description = usbDevice.getSysAttrValue("product");

		if (flags & UsbDeviceStringId_Manufacturer)
			deviceDesc->m_manufacturer = usbDevice.getSysAttrValue("manufacturer");

		if (flags & UsbDeviceStringId_Driver)
			deviceDesc->m_driver = hidDevice.getDriver();

		uint_t currentInterfaceId = -1;
		const uchar_t* p = (uchar_t*)descriptors.cp();
		const uchar_t* end = (uchar_t*)descriptors.getEnd();

		while (p < end) {
			uchar_t length = p[0];
			uchar_t descriptorType = p[1];

			union {
				const void* m_p;
				const libusb_interface_descriptor* m_deviceDesc;
				const libusb_interface_descriptor* m_ifaceDesc;
				const libusb_endpoint_descriptor* m_epDesc;
			} desc = { p };

			switch (descriptorType) {
			case LIBUSB_DT_INTERFACE:
				currentInterfaceId = desc.m_ifaceDesc->bInterfaceNumber;
				break;

			case LIBUSB_DT_ENDPOINT:
				if (currentInterfaceId == deviceDesc->m_interfaceId &&
					(desc.m_epDesc->bEndpointAddress & 0x80) &&
					(desc.m_epDesc->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) == LIBUSB_TRANSFER_TYPE_INTERRUPT
				) {
					deviceDesc->m_endpointId = desc.m_epDesc->bEndpointAddress;
					p = end; // we are done
				}

				break;
			}

			p += length;
		}

		HidRdParser::findFirstUsage(
			&deviceDesc->m_usagePage,
			&deviceDesc->m_usage,
			reportDescriptor,
			reportDescriptor.getCount()
		);

		sl::takeOver(&deviceDesc->m_reportDescriptor, &reportDescriptor);
		deviceList->insertTail(deviceDesc);
	}

	return deviceList->getCount();
}

//..............................................................................

} // namespace io
} // namespace axl
