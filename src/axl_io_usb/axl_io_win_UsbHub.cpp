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

	USB_DESCRIPTOR_REQUEST* req = (USB_DESCRIPTOR_REQUEST*)buffer;
	USB_STRING_DESCRIPTOR* desc = (USB_STRING_DESCRIPTOR*)req->Data;

	req->SetupPacket.wValue = (USB_STRING_DESCRIPTOR_TYPE << 8) | descriptorId;
	req->SetupPacket.wIndex = languageId;
	req->SetupPacket.wLength = sizeof(buffer) - sizeof(USB_DESCRIPTOR_REQUEST);
	req->ConnectionIndex = port;

	dword_t actualSize;
	bool result = ioctl(
		IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
		buffer,
		sizeof(buffer),
		buffer,
		sizeof(buffer),
		&actualSize
	);

	if (!result)
		return false;

	if (desc->bDescriptorType != USB_STRING_DESCRIPTOR_TYPE ||
		desc->bLength + sizeof(USB_DESCRIPTOR_REQUEST) > actualSize ||
		(desc->bLength & 1)
	)
		return err::fail(err::SystemErrorCode_InvalidDeviceState);

	string->copy(desc->bString, desc->bLength / sizeof(wchar_t));
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
