#include "pch.h"
#include "axl_io_Serial.h"
#include "axl_sys_win_DeviceInfo.h"

namespace axl {
namespace io {

//.............................................................................

class SerialPortEnumerator
{
public:
	static
	size_t
	createPortList (sl::StdList <SerialPortDesc>* portList);

protected:
	static
	SerialPortDesc* 
	createPortDesc (sys::win::DeviceInfo* deviceInfo);
};

//.............................................................................

size_t
SerialPortEnumerator::createPortList (sl::StdList <SerialPortDesc>* portList)
{
	bool result;

	portList->clear ();

	sys::win::DeviceInfoSet deviceInfoSet;
	result = deviceInfoSet.create (GUID_CLASS_COMPORT);
	if (!result)
		return 0;

	for (size_t i = 0;; i++)
	{
		sys::win::DeviceInfo deviceInfo;
		result = deviceInfoSet.getDeviceInfo (i, &deviceInfo);
		if (!result)
			continue;

		SerialPortDesc* portDesc = createPortDesc (&deviceInfo);
		if (portDesc)
			portList->insertTail (portDesc);
	}

	return portList->getCount ();
}

SerialPortDesc* 
SerialPortEnumerator::createPortDesc (sys::win::DeviceInfo* deviceInfo)
{
	int result;

	sys::win::RegKeyHandle devRegKey = deviceInfo->openDeviceRegistryKey (KEY_QUERY_VALUE);
	if (devRegKey == INVALID_HANDLE_VALUE)
		return NULL;

	dword_t type = 0;
	dword_t size = 0;

	result = ::RegQueryValueExA (devRegKey, "PortName", NULL, &type, NULL, &size);
	if (result != ERROR_SUCCESS)
		return NULL;

	char buffer [256];
	sl::Array <char> bufferString (ref::BufKind_Stack, buffer, sizeof (buffer));
	bufferString.setCount (size);

	result = ::RegQueryValueExA (
		devRegKey,
		"PortName",
		NULL,
		&type,
		(byte_t*) bufferString.a (),
		&size
		);

	if (result != ERROR_SUCCESS)
		return NULL;

	SerialPortDesc* portDesc = AXL_MEM_NEW (SerialPortDesc);
	portDesc->m_deviceName = buffer;
	deviceInfo->getDeviceRegistryProperty (SPDRP_DEVICEDESC, &portDesc->m_description);
	return portDesc;
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
