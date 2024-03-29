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
#include "axl_io_SerialPortEnumerator.h"
#include "axl_io_psx_File.h"
#include <linux/serial.h>
#include "axl_sys_lnx_Udev.h"
#include "axl_sys_lnx_UdevEnumerate.h"
#include "axl_sys_lnx_UdevListEntry.h"
#include "axl_sys_lnx_UdevDevice.h"

#if (_AXL_DEBUG)
#	define _AXL_IO_LNX_PRINT_DEVICE_PROPERTIES 0
#endif

namespace axl {
namespace io {

//..............................................................................

class SerialPortEnumerator {
public:
	static
	size_t
	createPortList(
		sl::List<SerialPortDesc>* portList,
		uint_t flags
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
SerialPortEnumerator::createPortList(
	sl::List<SerialPortDesc>* portList,
	uint_t flags
) {
	portList->clear();

	sys::lnx::Udev udev;
	sys::lnx::UdevEnumerate enumerator;

	bool result =
		udev.create() &&
		enumerator.create(udev) &&
		enumerator.addMatchSubsystem("tty") &&
		enumerator.scanDevices();

	if (!result) {
		static const char* deviceNameTable[][2] = {
			{ "/dev/ttyS0",   "Serial device /dev/ttyS0" },
			{ "/dev/ttyS1",   "Serial device /dev/ttyS1" },
			{ "/dev/ttyUSB0", "USB serial device /dev/ttyUSB0" },
			{ "/dev/ttyUSB1", "USB serial device /dev/ttyUSB1" },
		};

		for (size_t i = 0; i < countof(deviceNameTable); i++) {
			SerialPortDesc* portDesc = new SerialPortDesc;
			portDesc->m_deviceName = deviceNameTable[i][0];
			portDesc->m_description = deviceNameTable[i][1];
			portList->insertTail(portDesc);
		}

		return portList->getCount();
	}

	sys::lnx::UdevListEntry it = enumerator.getListEntry();
	for (; it; it++) {
		sl::StringRef path = it.getName();
		sys::lnx::UdevDevice device;
		result = device.createFromSysPath(udev, path);
		if (!result)
			continue;

		sys::lnx::UdevDevice parentDevice = device.getParent();
		if (!parentDevice) // no parent device, this is a virtual tty
			continue;

		sl::StringRef name = device.getDevNode();
		sl::StringRef driver = parentDevice.getDriver();
		if (driver == "serial8250") {
			// serial8250 driver has a hardcoded number of ports
			// the only way to tell which actually exist on a given system
			// is to try to open them and make an ioctl call

			serial_struct serialInfo;
			io::psx::File file;
			bool result =
				file.open(name, O_RDWR | O_NONBLOCK | O_NOCTTY) &&
				file.ioctl(TIOCGSERIAL, &serialInfo);

			if (!result || serialInfo.type == PORT_UNKNOWN)
				continue;
		}

		SerialPortDesc* portDesc = new SerialPortDesc;
		portDesc->m_deviceName = name;

		if (flags & SerialPortDescFlag_Description) {
			sl::StringRef model = device.getPropertyValue("ID_MODEL");
			sl::StringRef modelFromDb = device.getPropertyValue("ID_MODEL_FROM_DATABASE");

			if (modelFromDb.isEmpty())
				portDesc->m_description = model;
			else {
				portDesc->m_description = modelFromDb;
				if (!model.isEmpty() && model != modelFromDb)
					portDesc->m_description.appendFormat(" (%s)", model.sz());
			}
		}

		if (flags & SerialPortDescFlag_Manufacturer) {
			sl::StringRef vendor = device.getPropertyValue("ID_VENDOR");
			sl::StringRef vendorFromDb = device.getPropertyValue("ID_VENDOR_FROM_DATABASE");

			if (vendorFromDb.isEmpty())
				portDesc->m_manufacturer = vendor;
			else {
				portDesc->m_manufacturer = vendorFromDb;
				if (!vendor.isEmpty() && vendor != vendorFromDb)
					portDesc->m_manufacturer.appendFormat(" (%s)", vendor.sz());
			}
		}

		if (flags & SerialPortDescFlag_HardwareIds) {
			sl::StringRef vendorId = device.getPropertyValue("ID_VENDOR_ID");
			sl::StringRef modelId = device.getPropertyValue("ID_MODEL_ID");

			if (!vendorId.isEmpty() && !modelId.isEmpty())
				portDesc->m_hardwareIds = vendorId + ':' + modelId;
		}

		if (flags & SerialPortDescFlag_Driver)
			portDesc->m_driver = driver;

		if (flags & SerialPortDescFlag_Location) {
			sl::StringRef path = device.getPropertyValue("ID_PATH");
			portDesc->m_location = !path.isEmpty() ? path : device.getDevPath();
		}

#if (_AXL_IO_LNX_PRINT_DEVICE_PROPERTIES)
		printf("----------------------\n");
		printf("getSysPath: %s\n", device.getSysPath().sz());
		printf("getSysName: %s\n", device.getSysName().sz());
		printf("getSysNum: %s\n", device.getSysNum().sz());
		printf("getDevPath: %s\n", device.getDevPath().sz());
		printf("getDevNode: %s\n", device.getDevNode().sz());
		printf("getDevType: %s\n", parentDevice.getDevType().sz());
		printf("getSubsystem: %s\n", device.getSubsystem().sz());
		printf("getDriver: %s\n", parentDevice.getDriver().sz());
		printf("getAction: %s\n", parentDevice.getAction().sz());

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
#endif

		portList->insertTail(portDesc);
	}

	return portList->getCount();
}

//..............................................................................

size_t
enumerateSerialPorts(
	sl::List<SerialPortDesc>* portList,
	uint_t flags
) {
	return SerialPortEnumerator::createPortList(portList, flags);
}

//..............................................................................

} // namespace io
} // namespace axl
