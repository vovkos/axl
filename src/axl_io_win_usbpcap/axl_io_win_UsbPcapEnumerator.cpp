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
#include "axl_io_win_UsbPcapEnumerator.h"
#include "axl_io_win_UsbPcap.h"

namespace axl {
namespace io {
namespace win {

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

ushort_t
chooseLanguageId(const sl::String_w& languageIds) {
	size_t length = languageIds.getLength();
	if (!length)
		return 0;

	ushort_t languageId = languageIds[0];
	if (PRIMARYLANGID(languageId) == LANG_ENGLISH)
		return languageId;

	for (size_t i = 1; i < length; i++) {
		ushort_t id = languageIds[i];
		if (PRIMARYLANGID(id) == LANG_ENGLISH)
			return id;
	}

	return languageId;
}

//..............................................................................

template <typename T>
class UsbPcapDeviceEnumerator {
protected:
	sl::List<UsbPcapDeviceBase<T> >* m_deviceList;
	sys::win::DeviceInfoSet m_deviceInfoSet;
	sl::Array<char> m_buffer;
	uint_t m_mask;

public:
	UsbPcapDeviceEnumerator();

	bool
	createDeviceSnapshot() {
		return m_deviceInfoSet.create();
	}

	size_t
	enumerate(
		sl::List<UsbPcapDeviceBase<T> >* deviceList,
		uint_t mask = 0
	);

	size_t
	enumerate(
		sl::List<UsbPcapDeviceBase<T> >* deviceList,
		const UsbPcapHub& hub,
		uint_t mask = 0
	);

protected:
	size_t
	enumerate();

	size_t
	enumerate(const UsbPcapHub& hub);

	UsbPcapDeviceBase<T>*
	createDevice(
		const UsbPcapHub& hub,
		io::win::File& hubDevice,
		const USB_NODE_CONNECTION_INFORMATION& connectionInfo
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

template <typename T>
UsbPcapDeviceEnumerator<T>::UsbPcapDeviceEnumerator() {
	m_deviceList = NULL;
	m_mask = 0;
	createDeviceSnapshot();
}

template <typename T>
size_t
UsbPcapDeviceEnumerator<T>::enumerate(
	sl::List<UsbPcapDeviceBase<T> >* deviceList,
	uint_t mask
) {
	m_deviceList = deviceList;
	m_mask = mask;
	return enumerate();
}

template <typename T>
size_t
UsbPcapDeviceEnumerator<T>::enumerate(
	sl::List<UsbPcapDeviceBase<T> >* deviceList,
	const UsbPcapHub& hub,
	uint_t mask
) {
	m_deviceList = deviceList;
	m_mask = mask;
	return enumerate(hub);
}

template <typename T>
size_t
UsbPcapDeviceEnumerator<T>::enumerate() {
	sl::List<UsbPcapHub> hubList;
	size_t count = enumerateUsbPcapRootHubs(&hubList);
	if (count == -1)
		return -1;

	sl::ConstIterator<UsbPcapHub> it = hubList.getHead();
	for (; it; it++) {
		count = enumerate(**it);
		if (count == -1)
			return -1;
	}

	return m_deviceList->getCount();
}

template <typename T>
size_t
UsbPcapDeviceEnumerator<T>::enumerate(const UsbPcapHub& hub) {
	io::win::File hubDevice;

	dword_t actualSize;
	USB_NODE_INFORMATION nodeInfo;

	bool result =
		hubDevice.create(
			hub.m_hubDeviceName,
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

	if (actualSize < sizeof(USB_NODE_INFORMATION)) {
		err::setError(err::SystemErrorCode_BufferTooSmall);
		return -1;
	}

	sl::String_w attachedHubDeviceName;

	size_t portCount = nodeInfo.u.HubInformation.HubDescriptor.bNumberOfPorts;
	for (size_t i = 1; i <= portCount; i++) {
		USB_NODE_CONNECTION_INFORMATION connectionInfo = { 0 };
		connectionInfo.ConnectionIndex = i;

		result = hubDevice.ioctl(
			IOCTL_USB_GET_NODE_CONNECTION_INFORMATION,
			&connectionInfo,
			sizeof(USB_NODE_CONNECTION_INFORMATION),
			&connectionInfo,
			sizeof(USB_NODE_CONNECTION_INFORMATION),
			&actualSize
		);

		if (!result)
			return -1;

		ASSERT(actualSize >= sizeof(USB_NODE_CONNECTION_INFORMATION));
		if (connectionInfo.ConnectionStatus == NoDeviceConnected)
			continue;

		if (!connectionInfo.DeviceIsHub) {
			createDevice(hub, hubDevice, connectionInfo);
		} else {
			if (m_mask & UsbPcapEnumMask_Hubs)
				createDevice(hub, hubDevice, connectionInfo);

			// recursively enumerate the attached hub

			result = getConnectionName(&attachedHubDeviceName, hubDevice, connectionInfo.ConnectionIndex);
			if (!result)
				return -1;

			UsbPcapHub attachedHub;
			attachedHub.m_pcapDeviceName = hub.m_pcapDeviceName;
			normalizeDeviceName(&attachedHub.m_hubDeviceName, attachedHubDeviceName);
			enumerate(attachedHub);
		}
	}

	return m_deviceList->getCount();
}

template <typename T>
UsbPcapDeviceBase<T>*
UsbPcapDeviceEnumerator<T>::createDevice(
	const UsbPcapHub& hub,
	io::win::File& hubDevice,
	const USB_NODE_CONNECTION_INFORMATION& connectionInfo
) {
	sys::win::DeviceInfo deviceInfo;
	sl::String_w driverKeyName;

	bool result =
		getDriverKeyName(&driverKeyName, hubDevice, connectionInfo.ConnectionIndex) &&
		findDeviceInfo(&deviceInfo, driverKeyName);

	if (!result)
		return NULL;

	UsbPcapDeviceBase<T>* device = AXL_MEM_NEW(UsbPcapDeviceBase<T>);
	device->m_hubPort = connectionInfo.ConnectionIndex;
	device->m_pcapDeviceName = hub.m_pcapDeviceName;
	device->m_hubDeviceName = hub.m_hubDeviceName;
	device->m_deviceAddress = connectionInfo.DeviceAddress;
	device->m_deviceClass = connectionInfo.DeviceDescriptor.bDeviceClass;
	device->m_deviceSubClass = connectionInfo.DeviceDescriptor.bDeviceSubClass;
	device->m_vendorId = connectionInfo.DeviceDescriptor.idVendor;
	device->m_productId = connectionInfo.DeviceDescriptor.idProduct;
	device->m_flags = 0;

	if (connectionInfo.DeviceIsHub)
		device->m_flags |= UsbPcapDeviceFlag_Hub;

	if (connectionInfo.LowSpeed)
		device->m_flags |= UsbPcapDeviceFlag_LowSpeed;

	if (m_mask & UsbPcapEnumMask_Description) {
		deviceInfo.getDeviceRegistryProperty(SPDRP_FRIENDLYNAME, &device->m_description);

		if (device->m_description.isEmpty()) // try another property
			deviceInfo.getDeviceRegistryProperty(SPDRP_DEVICEDESC, &device->m_description);
	}

	if (m_mask & UsbPcapEnumMask_Manufacturer)
		deviceInfo.getDeviceRegistryProperty(SPDRP_MFG, &device->m_manufacturer);

	if (m_mask & UsbPcapEnumMask_HardwareId)
		deviceInfo.getDeviceRegistryProperty(SPDRP_HARDWAREID, &device->m_hardwareId);

	if (m_mask & UsbPcapEnumMask_Driver) {
		device->m_driverKeyName = driverKeyName;
		deviceInfo.getDeviceDriverPath(&device->m_driverPath);
	}

	if (m_mask & UsbPcapEnumMask_Location)
		deviceInfo.getDeviceRegistryProperty(SPDRP_LOCATION_PATHS, &device->m_location);

	if (m_mask & UsbPcapEnumMask_InstanceId)
		deviceInfo.getDeviceInstanceId(&device->m_instanceId);

	if (m_mask & UsbPcapEnumMask_Descriptors) {
		sl::String_w string;
		getStringDescriptor(&string, hubDevice, connectionInfo.ConnectionIndex, 0, 0);
		ushort_t languageId = chooseLanguageId(string);

		if (connectionInfo.DeviceDescriptor.iManufacturer) {
			getStringDescriptor(
				&string,
				hubDevice,
				connectionInfo.ConnectionIndex,
				connectionInfo.DeviceDescriptor.iManufacturer,
				languageId
			);

			device->m_descriptorTable[UsbPcapDescriptor_Manufacturer] = string;
		}

		if (connectionInfo.DeviceDescriptor.iProduct) {
			getStringDescriptor(
				&string,
				hubDevice,
				connectionInfo.ConnectionIndex,
				connectionInfo.DeviceDescriptor.iProduct,
				languageId
			);

			device->m_descriptorTable[UsbPcapDescriptor_Product] = string;
		}

		if (connectionInfo.DeviceDescriptor.iSerialNumber) {
			getStringDescriptor(
				&string,
				hubDevice,
				connectionInfo.ConnectionIndex,
				connectionInfo.DeviceDescriptor.iSerialNumber,
				languageId
			);

			device->m_descriptorTable[UsbPcapDescriptor_SerialNumber] = string;
		}
	}

	m_deviceList->insertTail(device);
	return device;
}

template <typename T>
bool
UsbPcapDeviceEnumerator<T>::findDeviceInfo(
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

template <typename T>
bool
UsbPcapDeviceEnumerator<T>::getConnectionName(
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

template <typename T>
bool
UsbPcapDeviceEnumerator<T>::getDriverKeyName(
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

template <typename T>
bool
UsbPcapDeviceEnumerator<T>::getStringDescriptor(
	sl::String_w* string,
	io::win::File& hubDevice,
	size_t index,
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
enumerateUsbPcapRootHubs(sl::List<UsbPcapHub>* hubList) {
	hubList->clear();

	using namespace axl::sys::win;

	enum {
		DirBufferSize = 4 * 1024
	};

	long status;
	static sl::StringRef_w deviceDirName = L"\\device";
	static sl::StringRef_w deviceType = L"device";
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

	sl::String_w hubSymlink;
	sl::Array<char> dirBuffer;
	dirBuffer.setCount(DirBufferSize);

	BOOLEAN isFirstQuery = TRUE;
	ULONG queryContext = 0;

	sl::String_utf16 name;
	sl::String_utf16 typeName;

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
			sl::StringRef_utf16 type(dirInfo->TypeName.Buffer, dirInfo->TypeName.Length / sizeof(wchar_t), true);
			sl::StringRef_utf16 name(dirInfo->Name.Buffer, dirInfo->Name.Length / sizeof(wchar_t), true);

			if (!type.isEqualIgnoreCase(deviceType) ||
				!name.isPrefixIgnoreCase(usbPcapPrefix))
				continue;

			sl::String_w pcapDeviceName = symlinkPrefix + name;
			UsbPcap pcap;

			bool result =
				pcap.open(pcapDeviceName, io::FileFlag_ReadOnly) &&
				pcap.getHubSymlink(&hubSymlink);

			if (!result)
				return -1;

			UsbPcapHub* hub = AXL_MEM_NEW(UsbPcapHub);
			hub->m_pcapDeviceName = pcapDeviceName;
			normalizeDeviceName(&hub->m_hubDeviceName, hubSymlink);
			hubList->insertTail(hub);
		}

		isFirstQuery = FALSE;
	}

	return hubList->getCount();
}

size_t
enumerateUsbPcapDevices(
	sl::List<UsbPcapDevice>* deviceList,
	const UsbPcapHub& hub,
	uint_t flags
) {
	UsbPcapDeviceEnumerator<char> enumerator;
	return enumerator.enumerate(deviceList, hub, flags);
}

size_t
enumerateUsbPcapDevices(
	sl::List<UsbPcapDevice>* deviceList,
	uint_t flags
) {
	UsbPcapDeviceEnumerator<char> enumerator;
	return enumerator.enumerate(deviceList, flags);
}

size_t
enumerateUsbPcapDevices(
	sl::List<UsbPcapDevice_w>* deviceList,
	const UsbPcapHub& hub,
	uint_t flags
) {
	UsbPcapDeviceEnumerator<wchar_t> enumerator;
	return enumerator.enumerate(deviceList, hub, flags);
}

size_t
enumerateUsbPcapDevices(
	sl::List<UsbPcapDevice_w>* deviceList,
	uint_t flags
) {
	UsbPcapDeviceEnumerator<wchar_t> enumerator;
	return enumerator.enumerate(deviceList, flags);
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
