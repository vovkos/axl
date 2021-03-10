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
#include "axl_sys_win_DeviceInfo.h"

#define _AXL_ENUMERATE_DEVICE_CLASS 1

#include <initguid.h>
#include <devguid.h>

namespace axl {
namespace io {

//..............................................................................

class SerialPortEnumerator
{
public:
	static
	size_t
	createPortList(sl::List<SerialPortDesc>* portList);

protected:
	static
	SerialPortDesc*
	createPortDesc(sys::win::DeviceInfo* deviceInfo);
};

//..............................................................................

size_t
SerialPortEnumerator::createPortList(sl::List<SerialPortDesc>* portList)
{
	bool result;

	portList->clear();

	sys::win::DeviceInfoSet deviceInfoSet;

#ifdef _AXL_ENUMERATE_DEVICE_CLASS
	result = deviceInfoSet.create(GUID_DEVCLASS_PORTS, DIGCF_PRESENT);
#else
	result = deviceInfoSet.create(GUID_DEVINTERFACE_COMPORT, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
#endif

	if (!result)
		return 0;

	for (size_t i = 0;; i++)
	{
		sys::win::DeviceInfo deviceInfo;
		result = deviceInfoSet.getDeviceInfo(i, &deviceInfo);
		if (!result)
			break;

		SerialPortDesc* portDesc = createPortDesc(&deviceInfo);
		if (portDesc)
			portList->insertTail(portDesc);
	}

	return portList->getCount();
}

SerialPortDesc*
SerialPortEnumerator::createPortDesc(sys::win::DeviceInfo* deviceInfo)
{
	long result;

	sys::win::RegKeyHandle devRegKey = deviceInfo->openDeviceRegistryKey(KEY_QUERY_VALUE);
	if (devRegKey == INVALID_HANDLE_VALUE)
		return NULL;

	dword_t type = 0;
	dword_t size = 0;

	result = ::RegQueryValueExA(devRegKey, "PortName", NULL, &type, NULL, &size);
	if (result != ERROR_SUCCESS)
		return NULL;

	char buffer[256];
	sl::Array<char> bufferString(ref::BufKind_Stack, buffer, sizeof(buffer));
	bufferString.setCount(size);

	result = ::RegQueryValueExA(
		devRegKey,
		"PortName",
		NULL,
		&type,
		(byte_t*)bufferString.p(),
		&size
		);

	if (result != ERROR_SUCCESS)
		return NULL;

	SerialPortDesc* portDesc = AXL_MEM_NEW(SerialPortDesc);
	portDesc->m_deviceName = bufferString;
	deviceInfo->getDeviceRegistryProperty(SPDRP_FRIENDLYNAME, &portDesc->m_description);

	if (portDesc->m_description.isEmpty()) // try another property
		deviceInfo->getDeviceRegistryProperty(SPDRP_DEVICEDESC, &portDesc->m_description);

	return portDesc;
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
