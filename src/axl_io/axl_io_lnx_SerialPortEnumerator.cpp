#include "pch.h"
#include "axl_io_Serial.h"
#include "axl_io_psx_Fd.h"
#include "axl_sys_lnx_Udev.h"

namespace axl {
namespace io {

//.............................................................................

class SerialPortEnumerator
{
public:
	static
	size_t
	createPortList (sl::StdList <SerialPortDesc>* portList);
};

//.............................................................................

size_t
SerialPortEnumerator::createPortList (sl::StdList <SerialPortDesc>* portList)
{
	portList->clear ();

	sys::lnx::UdevContext udev;
	sys::lnx::UdevEnumerator enumerator = udev.createEnumerator ();

	bool result = 
		enumerator.addMatchSubsystem ("tty") &&
		enumerator.scanDevices ();

	if (!result)
		return 0;

	sys::lnx::UdevListEntry it = enumerator.getListEntry ();
	for (; it; it++)
	{
		const char* path = it.getName ();
		sys::lnx::UdevDevice device = udev.getDeviceFromSysPath (path);
		if (!device)
			continue;

		sys::lnx::UdevDevice parentDevice = device.getParent ();
		if (!parentDevice) // no parent device, this is a virtual tty
			continue;

		parentDevice.addRef (); // getParent does not add ref

		const char* name = device.getDevNode ();
		const char* driver = parentDevice.getDriver ();
		if (driver && strcmp (driver, "serial8250") == 0)
		{
			// serial8250 driver has a hardcoded number of ports
			// the only way to tell which actually exist on a given system
			// is to try to open them and make an ioctl call

			serial_struct serialInfo;
			io::psx::Fd fd;
			bool result =
				fd.open (name, O_RDWR | O_NONBLOCK | O_NOCTTY) &&
				fd.ioctl (TIOCGSERIAL, &serialInfo);

			if (!result || serialInfo.type == PORT_UNKNOWN)
				continue;
		}

		const char* description = device.getPropertyValue ("ID_MODEL_FROM_DATABASE");
		if (!description)
			description = device.getPropertyValue ("ID_MODEL");

		if (!description)
			description = driver;

		SerialPortDesc* portDesc = AXL_MEM_NEW (SerialPortDesc);
		portDesc->m_deviceName = name;
		portDesc->m_description = description;
		portList->insertTail (portDesc);
	}

	return portList->getCount ();
}

//.............................................................................

size_t
createSerialPortDescList (sl::StdList <SerialPortDesc>* portList)
{
	return SerialPortEnumerator::createPortList (portList);
}

//.............................................................................

} // namespace io
} // namespace axl
