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
#include "axl_io_Serial.h"

#ifdef _AXL_SYS_LNX_LIBUDEV
#	include "axl_sys_lnx_Udev.h"
#endif

namespace axl {
namespace io {

//..............................................................................

class SerialPortEnumerator
{
public:
	static
	size_t
	createPortList(sl::List<SerialPortDesc>* portList);
};

//..............................................................................

size_t
SerialPortEnumerator::createPortList(sl::List<SerialPortDesc>* portList)
{
	portList->clear();

#ifdef _AXL_SYS_LNX_LIBUDEV
	sys::lnx::UdevContext udev;
	sys::lnx::UdevEnumerator enumerator = udev.createEnumerator();

	bool result =
		enumerator.addMatchSubsystem("tty") &&
		enumerator.scanDevices();

	if (!result)
		return 0;

	sys::lnx::UdevListEntry it = enumerator.getListEntry();
	for (; it; it++)
	{
		sl::StringRef path = it.getName();
		sys::lnx::UdevDevice device = udev.getDeviceFromSysPath(path);
		if (!device)
			continue;

		sys::lnx::UdevDevice parentDevice = device.getParent();
		if (!parentDevice) // no parent device, this is a virtual tty
			continue;

		parentDevice.addRef(); // getParent does not add ref

		sl::StringRef name = device.getDevNode();
		sl::StringRef driver = parentDevice.getDriver();
		if (driver == "serial8250")
		{
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

		sl::StringRef description = device.getPropertyValue("ID_MODEL_FROM_DATABASE");
		if (description.isEmpty())
			description = device.getPropertyValue("ID_MODEL");

		if (description.isEmpty())
			description = driver;

		SerialPortDesc* portDesc = AXL_MEM_NEW(SerialPortDesc);
		portDesc->m_deviceName = name;
		portDesc->m_description = description;
		portList->insertTail(portDesc);
	}
#else
	static const char* deviceNameTable[][2] =
	{
		{ "/dev/ttyS0",   "Serial device /dev/ttyS0" },
		{ "/dev/ttyS1",   "Serial device /dev/ttyS1" },
		{ "/dev/ttyUSB0", "USB Serial device /dev/ttyUSB0" },
		{ "/dev/ttyUSB1", "USB Serial device /dev/ttyUSB1" },
	};

	for (size_t i = 0; i < countof(deviceNameTable); i++)
	{
		SerialPortDesc* portDesc = AXL_MEM_NEW(SerialPortDesc);
		portDesc->m_deviceName = deviceNameTable[i][0];
		portDesc->m_description = deviceNameTable[i][1];
		portList->insertTail(portDesc);
	}
#endif

	return portList->getCount();
}

//..............................................................................

size_t
createSerialPortDescList(sl::List<SerialPortDesc>* portList)
{
	return SerialPortEnumerator::createPortList(portList);
}

//..............................................................................

} // namespace io
} // namespace axl
