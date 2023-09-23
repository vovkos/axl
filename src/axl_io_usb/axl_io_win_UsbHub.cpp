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
#include "axl_io_win_UsbHub.h"

#include <initguid.h>

DEFINE_GUID(
	GUID_DEVINTERFACE_USB_HUB,
	0xf18a0e88, 0xc30c, 0x11d0, 0x88, 0x15, 0x00, 0xa0, 0xc9, 0x06, 0xbe, 0xd8
);

namespace axl {
namespace io {
namespace win {

//..............................................................................

size_t
UsbHub::getDescriptorImpl(
	void* p, // must be big enough
	uint_t port,
	ushort_t value,
	ushort_t index,
	ushort_t length
) {
	USB_DESCRIPTOR_REQUEST* req = (USB_DESCRIPTOR_REQUEST*)p;
	req->SetupPacket.wValue = value;
	req->SetupPacket.wIndex = index;
	req->SetupPacket.wLength = length;
	req->ConnectionIndex = port;

	dword_t offset = offsetof(USB_DESCRIPTOR_REQUEST, Data);
	dword_t size = offset + length;
	dword_t actualSize;

	bool result = ioctl(
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		p,
		size,
		p,
		size,
		&actualSize
	);

	return result ? actualSize : -1;
}

bool
UsbHub::getConfigurationDescriptor(
	sl::Array<char>* descriptor,
	uint_t port
) {
	char buffer0[sizeof(USB_DESCRIPTOR_REQUEST) + sizeof(USB_CONFIGURATION_DESCRIPTOR)] = { 0 };

	size_t result = getDescriptorImpl(
		buffer0,
		port,
		USB_CONFIGURATION_DESCRIPTOR_TYPE << 8,
		0,
		sizeof(USB_CONFIGURATION_DESCRIPTOR)
	);

	if (result == -1)
		return false;

	USB_CONFIGURATION_DESCRIPTOR* configDesc = (USB_CONFIGURATION_DESCRIPTOR*)(((USB_DESCRIPTOR_REQUEST*)buffer0)->Data);
	if (result < offsetof(USB_DESCRIPTOR_REQUEST, Data) + sizeof(USB_CONFIGURATION_DESCRIPTOR) ||
		configDesc->bDescriptorType != USB_CONFIGURATION_DESCRIPTOR_TYPE ||
		configDesc->wTotalLength < sizeof(USB_CONFIGURATION_DESCRIPTOR)
	)
		return err::fail("invalid USB_CONFIGURATION_DESCRIPTOR");

	size_t size = offsetof(USB_DESCRIPTOR_REQUEST, Data) + configDesc->wTotalLength;
	descriptor->setCount(size + 1);

	result = getDescriptorImpl(
		descriptor->p(),
		port,
		USB_CONFIGURATION_DESCRIPTOR_TYPE << 8,
		0,
		descriptor->getCount()
	);

	if (result == -1)
		return false;

	ASSERT(result == size);
	descriptor->setCount(result);
	descriptor->remove(0, offsetof(USB_DESCRIPTOR_REQUEST, Data));
	return true;
}

bool
UsbHub::getStringDescriptor(
	sl::String_w* string,
	uint_t port,
	uchar_t descriptorId,
	ushort_t languageId
) {
	char buffer[
		sizeof(USB_DESCRIPTOR_REQUEST) +
		sizeof(USB_STRING_DESCRIPTOR) +
		MAXIMUM_USB_STRING_LENGTH
	] = { 0 };

	size_t result = getDescriptorImpl(
		buffer,
		port,
		(USB_STRING_DESCRIPTOR_TYPE << 8) | descriptorId,
		languageId,
		sizeof(USB_STRING_DESCRIPTOR)
	);

	if (result == -1)
		return false;

	USB_STRING_DESCRIPTOR* stringDesc = (USB_STRING_DESCRIPTOR*)(((USB_DESCRIPTOR_REQUEST*)buffer)->Data);
	if (stringDesc->bDescriptorType != USB_STRING_DESCRIPTOR_TYPE ||
		stringDesc->bLength < offsetof(USB_STRING_DESCRIPTOR, bString)
	)
		return err::fail("invalid USB_CONFIGURATION_DESCRIPTOR");

	size_t length = (stringDesc->bLength - offsetof(USB_STRING_DESCRIPTOR, bString)) / sizeof(wchar_t);

	result = getDescriptorImpl(
		buffer,
		port,
		(USB_STRING_DESCRIPTOR_TYPE << 8) | descriptorId,
		languageId,
		offsetof(USB_DESCRIPTOR_REQUEST, Data) + stringDesc->bLength
	);

	if (result == -1)
		return false;

	string->copy(stringDesc->bString, length);
	return true;
}

//..............................................................................

UsbHub*
UsbHubDb::getUsbHub(sys::win::DeviceInfo* deviceInfo) {
	DEVINST parentDevInst = 0;
	CONFIGRET cr = ::CM_Get_Parent(&parentDevInst, deviceInfo->getDevInfoData()->DevInst, 0);
	if (cr != CR_SUCCESS) {
		err::setError("can't get DEVINST of the device's parent");
		return NULL;
	}

	UsbHub* hub = m_hubMap.findValue(parentDevInst, NULL);
	if (hub)
		return hub;

	sys::win::DeviceInfo parentDeviceInfo;
	sys::win::DeviceInfoSet deviceInfoSet;
	deviceInfoSet.attach(deviceInfo->getDevInfoSet());
	bool result = deviceInfoSet.findDeviceInfoByDevInst(parentDevInst, &parentDeviceInfo);
	deviceInfoSet.detach();

	if (!result) {
		err::setError("can't find SP_DEVINFO_DATA of the device's parent");
		return NULL;
	}

	char buffer[256];
	sl::String_w path(rc::BufKind_Stack, buffer, sizeof(buffer));
	SP_DEVICE_INTERFACE_DATA ifaceData = { 0 };
	ifaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	result =
		parentDeviceInfo.enumDeviceInterfaces(GUID_DEVINTERFACE_USB_HUB, 0, &ifaceData) &&
		parentDeviceInfo.getDeviceInterfacePath(&ifaceData, &path);

	if (!result)
		return NULL;

	hub = new UsbHub;
	result = hub->create(
		path,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING
	);

	if (!result) {
		delete hub;
		return NULL;
	}

	m_hubList.insertTail(hub);
	m_hubMap[parentDevInst] = hub;
	return hub;
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
