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
#include "axl_io_UsbMonEnumerator.h"
#include "axl_io_UsbStringDescriptorLanguage.h"
#include "axl_io_win_UsbPcap.h"
#include "axl_sys_win_NtDll.h"
#include "axl_sys_win_NtStatus.h"
#include "axl_sys_win_DeviceInfo.h"
#include <usbioctl.h>
#include <usb.h>

namespace axl {
namespace io {

//..............................................................................

size_t
normalizeDeviceName(
	sl::StringRef_w* deviceName,
	const sl::StringRef_w& name
) {
	static sl::StringRef_w symlinkPrefix = L"\\\\.\\";
	static sl::StringRef_w ntSymlinkPrefix = L"\\??\\";

	if (name.isPrefix(ntSymlinkPrefix))
		*deviceName = symlinkPrefix + name.getSubString(ntSymlinkPrefix.getLength());
	else if (name.isPrefix('\\'))
		*deviceName = name;
	else
		*deviceName = symlinkPrefix + name;

	return deviceName->getLength();
}

uint_t
getCaptureDeviceId(const sl::StringRef& deviceName) {
	intptr_t i = deviceName.getLength() - 1;
	while (i >= 0 && isdigit(deviceName[i]))
		i--;

	return atoi(deviceName.sz() + i + 1);
}

//..............................................................................

class UsbPcapDeviceEnumerator {
protected:
	sl::List<UsbMonDeviceDesc>* m_deviceList;
	sys::win::DeviceInfoSet m_deviceInfoSet;
	sl::Array<char> m_buffer;
	uint_t m_mask;

public:
	UsbPcapDeviceEnumerator() {
		m_deviceList = NULL;
		m_mask = 0;
	}

	size_t
	enumerate(
		sl::List<UsbMonDeviceDesc>* deviceList,
		uint_t mask = 0
	);

protected:
	size_t
	enumerateHub(
		const sl::String_w& pcapDeviceName,
		const sl::String_w& hubDeviceName
	);

	UsbMonDeviceDesc*
	createDevice(
		io::win::File& hubDevice,
		const sl::String& pcapDeviceName,
		const USB_NODE_CONNECTION_INFORMATION& connectionInfo,
		UsbMonDeviceSpeed speed
	);

	bool
	findDeviceInfo(
		sys::win::DeviceInfo* deviceInfo,
		const sl::String_w& driverKeyName
	);

	bool
	getConnectionName(
		sl::String_w* name,
		io::win::File& device,
		size_t index
	);

	bool
	getDriverKeyName(
		sl::String_w* name,
		io::win::File& hubDevice,
		size_t index
	);

	bool
	getStringDescriptor(
		sl::String_w* string,
		io::win::File& hubDevice,
		size_t index,
		uchar_t descriptorId,
		ushort_t languageId
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
UsbPcapDeviceEnumerator::enumerate(
	sl::List<UsbMonDeviceDesc>* deviceList,
	uint_t mask
) {
	deviceList->clear();

	bool result = m_deviceInfoSet.create();
	if (!result)
		return -1;

	m_deviceList = deviceList;
	m_mask = mask;

	using namespace axl::sys::win;

	enum {
		DirBufferSize = 4 * 1024
	};

	long status;
	static sl::StringRef_w deviceDirName = L"\\device";
	static sl::StringRef_w deviceTypeName = L"device";
	static sl::StringRef_w usbPcapPrefix = L"usbpcap";
	static sl::StringRef_w symlinkPrefix = L"\\\\.\\";

	UNICODE_STRING uniName;
	uniName.Buffer = (PWSTR)deviceDirName.cp();
	uniName.Length = deviceDirName.getLength() * sizeof(wchar_t);
	uniName.MaximumLength = uniName.Length + sizeof(wchar_t);

	OBJECT_ATTRIBUTES oa = { 0 };
	oa.Length = sizeof(oa);
	oa.ObjectName = &uniName;

	NtHandle deviceDir;
	status = ntOpenDirectoryObject(deviceDir.p(), DIRECTORY_QUERY | DIRECTORY_TRAVERSE, &oa);
	if (status < 0) {
		err::setError(NtStatus(status));
		return -1;
	}

	sl::String_w hubName;
	sl::String_w hubDeviceName;

	sl::Array<char> dirBuffer;
	dirBuffer.setCount(DirBufferSize);

	BOOLEAN isFirstQuery = TRUE;
	ULONG queryContext = 0;

	for (;;) {
		ULONG actualSize;

		status = ntQueryDirectoryObject(
			deviceDir,
			dirBuffer,
			DirBufferSize,
			FALSE,
			isFirstQuery,
			&queryContext,
			&actualSize
		);

		if (status < 0) {
			if (status == STATUS_NO_MORE_ENTRIES)
				break;

			err::setError(sys::win::NtStatus(status));
			return -1;
		}

		const OBJECT_DIRECTORY_INFORMATION* dirInfo = (OBJECT_DIRECTORY_INFORMATION*)dirBuffer.cp();
		for (; dirInfo->Name.Buffer; dirInfo++) {
			sl::StringRef_w name(dirInfo->Name.Buffer, dirInfo->Name.Length / sizeof(wchar_t), true);
			sl::StringRef_w typeName(dirInfo->TypeName.Buffer, dirInfo->TypeName.Length / sizeof(wchar_t), true);

			if (!typeName.isEqualIgnoreCase(deviceTypeName) ||
				!name.isPrefixIgnoreCase(usbPcapPrefix)
			)
				continue;

			sl::String_w pcapDeviceName = symlinkPrefix + name;
			win::UsbPcap pcap;

			bool result =
				pcap.open(pcapDeviceName, win::UsbPcap::Mode_Enumerate) &&
				pcap.getHubSymlink(&hubName);

			if (!result)
				continue; // ignore this hub

			normalizeDeviceName(&hubDeviceName, hubName);
			enumerateHub(pcapDeviceName, hubDeviceName);
		}

		isFirstQuery = FALSE;
	}

	return deviceList->getCount();
}

size_t
UsbPcapDeviceEnumerator::enumerateHub(
	const sl::String_w& pcapDeviceName,
	const sl::String_w& hubDeviceName
) {
	io::win::File hubDevice;

	dword_t actualSize;
	USB_NODE_INFORMATION nodeInfo;

	bool result =
		hubDevice.create(
			hubDeviceName,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING
		) &&
		hubDevice.ioctl(
			IOCTL_USB_GET_NODE_INFORMATION,
			NULL,
			0,
			&nodeInfo,
			sizeof(nodeInfo),
			&actualSize
		);

	if (!result)
		return -1;

	if (actualSize < sizeof(USB_NODE_INFORMATION))
		return err::fail<size_t>(-1, err::SystemErrorCode_BufferTooSmall);

	sl::String_w attachedHubName;
	sl::String_w attachedHubDeviceName;

	size_t deviceCount = 0;
	size_t portCount = nodeInfo.u.HubInformation.HubDescriptor.bNumberOfPorts;
	for (size_t i = 1; i <= portCount; i++) {
		USB_NODE_CONNECTION_INFORMATION connectionInfo = { 0 };
		connectionInfo.ConnectionIndex = i;

		result = hubDevice.ioctl(
			IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX,
			&connectionInfo,
			sizeof(connectionInfo),
			&connectionInfo,
			sizeof(connectionInfo),
			&actualSize
		);

		UsbMonDeviceSpeed speed;

		if (result)
			speed = (UsbMonDeviceSpeed)((uchar_t)connectionInfo.LowSpeed + 1);
		else {
			result = hubDevice.ioctl(
				IOCTL_USB_GET_NODE_CONNECTION_INFORMATION,
				&connectionInfo,
				sizeof(connectionInfo),
				&connectionInfo,
				sizeof(connectionInfo),
				&actualSize
			);

			if (!result)
				continue;

			speed = connectionInfo.LowSpeed ? UsbMonDeviceSpeed_Low : UsbMonDeviceSpeed_Full;
		}

		ASSERT(actualSize >= sizeof(USB_NODE_CONNECTION_INFORMATION));
		if (connectionInfo.ConnectionStatus == NoDeviceConnected)
			continue;

		if (!connectionInfo.DeviceIsHub) {
			createDevice(hubDevice, pcapDeviceName.s2(), connectionInfo, speed);
			deviceCount++;
		} else {
			if (m_mask & UsbMonDeviceDescMask_Hubs) {
				createDevice(hubDevice, pcapDeviceName.s2(), connectionInfo, speed);
				deviceCount++;
			}

			// recursively enumerate the attached hub

			result = getConnectionName(&attachedHubName, hubDevice, connectionInfo.ConnectionIndex);
			if (result) {
				normalizeDeviceName(&attachedHubDeviceName, attachedHubName);
				deviceCount += enumerateHub(pcapDeviceName, attachedHubDeviceName);
			}
		}
	}

	return deviceCount;
}

UsbMonDeviceDesc*
UsbPcapDeviceEnumerator::createDevice(
	io::win::File& hubDevice,
	const sl::String& pcapDeviceName,
	const USB_NODE_CONNECTION_INFORMATION& connectionInfo,
	UsbMonDeviceSpeed speed
) {
	sys::win::DeviceInfo deviceInfo;
	sl::String_w driverKeyName;

	bool result =
		getDriverKeyName(&driverKeyName, hubDevice, connectionInfo.ConnectionIndex) &&
		findDeviceInfo(&deviceInfo, driverKeyName);

	if (!result)
		return NULL;

	UsbMonDeviceDesc* device = AXL_MEM_NEW(UsbMonDeviceDesc);
	device->m_captureDeviceName = pcapDeviceName;
	device->m_captureDeviceId = getCaptureDeviceId(pcapDeviceName);
	device->m_vendorId = connectionInfo.DeviceDescriptor.idVendor;
	device->m_productId = connectionInfo.DeviceDescriptor.idProduct;
	device->m_address = (uint8_t)connectionInfo.DeviceAddress;
	device->m_class = connectionInfo.DeviceDescriptor.bDeviceClass;
	device->m_subClass = connectionInfo.DeviceDescriptor.bDeviceSubClass;
	device->m_manufacturerDescriptorId = connectionInfo.DeviceDescriptor.iManufacturer;
	device->m_productDescriptorId = connectionInfo.DeviceDescriptor.iProduct;
	device->m_serialNumberDescriptorId = connectionInfo.DeviceDescriptor.iSerialNumber;
	device->m_speed = speed;
	device->m_flags = UsbMonDeviceDescFlag_DeviceDescriptor; // on windows, descriptor is always available

	if (connectionInfo.DeviceIsHub)
		device->m_flags |= UsbMonDeviceDescFlag_Hub;

	if (m_mask & UsbMonDeviceDescMask_Description) {
		deviceInfo.getDeviceRegistryProperty(SPDRP_FRIENDLYNAME, &device->m_description);

		if (device->m_description.isEmpty()) // try another property
			deviceInfo.getDeviceRegistryProperty(SPDRP_DEVICEDESC, &device->m_description);
	}

	if (m_mask & UsbMonDeviceDescMask_Manufacturer) {
		deviceInfo.getDeviceRegistryProperty(SPDRP_MFG, &device->m_manufacturer);
	}

	if (m_mask & UsbMonDeviceDescMask_Driver)
		deviceInfo.getDeviceDriverPath(&device->m_driver);

	if (m_mask & UsbMonDeviceDescMask_Descriptors) {
		sl::String_w string;
		getStringDescriptor(&string, hubDevice, connectionInfo.ConnectionIndex, 0, 0);
		ushort_t languageId = chooseUsbStringDescriptorLanguage(string);

		if (connectionInfo.DeviceDescriptor.iManufacturer) {
			getStringDescriptor(
				&string,
				hubDevice,
				connectionInfo.ConnectionIndex,
				connectionInfo.DeviceDescriptor.iManufacturer,
				languageId
			);

			device->m_manufacturerDescriptor = string;
		}

		if (connectionInfo.DeviceDescriptor.iProduct) {
			getStringDescriptor(
				&string,
				hubDevice,
				connectionInfo.ConnectionIndex,
				connectionInfo.DeviceDescriptor.iProduct,
				languageId
			);

			device->m_productDescriptor = string;
		}

		if (connectionInfo.DeviceDescriptor.iSerialNumber) {
			getStringDescriptor(
				&string,
				hubDevice,
				connectionInfo.ConnectionIndex,
				connectionInfo.DeviceDescriptor.iSerialNumber,
				languageId
			);

			device->m_serialNumberDescriptor = string;
		}
	}

	m_deviceList->insertTail(device);
	return device;
}

bool
UsbPcapDeviceEnumerator::findDeviceInfo(
	sys::win::DeviceInfo* deviceInfo,
	const sl::String_w& driverKeyName
) {
	bool result;
	sl::String_w deviceDriver;

	for (size_t i = 0;; i++) {
		result = m_deviceInfoSet.getDeviceInfo(i, deviceInfo);
		if (!result)
			break;

		result = deviceInfo->getDeviceRegistryProperty(SPDRP_DRIVER, &deviceDriver);
		if (result && deviceDriver.isEqualIgnoreCase(driverKeyName))
			return true;
	}

	return false; // not found
}

bool
UsbPcapDeviceEnumerator::getConnectionName(
	sl::String_w* name,
	io::win::File& hubDevice,
	size_t index
) {
	bool result;
	dword_t actualSize;

	if (m_buffer.getCount() < sizeof(USB_NODE_CONNECTION_NAME))
		m_buffer.setCount(sizeof(USB_NODE_CONNECTION_NAME));

	for (;;) {
		USB_NODE_CONNECTION_NAME* connectionName = (USB_NODE_CONNECTION_NAME*)m_buffer.p();
		connectionName->ConnectionIndex = (ULONG)index;

		result = hubDevice.ioctl(
			IOCTL_USB_GET_NODE_CONNECTION_NAME,
			m_buffer.p(),
			m_buffer.getCount(),
			m_buffer.p(),
			m_buffer.getCount(),
			&actualSize
		);

		if (!result)
			return false;

		ASSERT(actualSize >= sizeof(USB_NODE_CONNECTION_NAME));
		if (connectionName->ActualLength <= m_buffer.getCount()) { // done
			*name = connectionName->NodeName;
			return true;
		}

		result = m_buffer.setCount(connectionName->ActualLength);
		if (!result)
			return false;
	}
};

bool
UsbPcapDeviceEnumerator::getDriverKeyName(
	sl::String_w* name,
	io::win::File& hubDevice,
	size_t index
) {
	bool result;
	dword_t actualSize;

	if (m_buffer.getCount() < sizeof(USB_NODE_CONNECTION_DRIVERKEY_NAME))
		m_buffer.setCount(sizeof(USB_NODE_CONNECTION_DRIVERKEY_NAME));

	for (;;) {
		USB_NODE_CONNECTION_DRIVERKEY_NAME* driverKeyName = (USB_NODE_CONNECTION_DRIVERKEY_NAME*)m_buffer.p();
		driverKeyName->ConnectionIndex = (ULONG)index;

		result = hubDevice.ioctl(
			IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
			m_buffer.p(),
			m_buffer.getCount(),
			m_buffer.p(),
			m_buffer.getCount(),
			&actualSize
		);

		if (!result)
			return false;

		ASSERT(actualSize >= sizeof(USB_NODE_CONNECTION_DRIVERKEY_NAME));
		if (driverKeyName->ActualLength <= m_buffer.getCount()) { // done
			*name = driverKeyName->DriverKeyName;
			return true;
		}

		result = m_buffer.setCount(driverKeyName->ActualLength);
		if (!result)
			return false;
	}
};

bool
UsbPcapDeviceEnumerator::getStringDescriptor(
	sl::String_w* string,
	io::win::File& hubDevice,
	size_t index,
	uchar_t descriptorId,
	ushort_t languageId
) {
	string->clear();

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
	req->ConnectionIndex = index;

	dword_t actualSize;
	bool result = hubDevice.ioctl(
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

size_t
enumerateUsbMonDevices(
	sl::List<UsbMonDeviceDesc>* deviceList,
	uint_t mask
) {
	UsbPcapDeviceEnumerator enumerator;
	return enumerator.enumerate(deviceList, mask);
}

//..............................................................................

} // namespace io
} // namespace axl
