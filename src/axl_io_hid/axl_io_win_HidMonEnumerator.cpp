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
#include "axl_sys_win_DeviceInfo.h"
#include "axl_sys_win_CfgMgr.h"
#include "axl_io_win_File.h"
#include "axl_io_win_UsbHub.h"
#include "axl_sl_StringHashTable.h"

extern "C" {

#include <initguid.h>
#include <hidclass.h>
#include <hidsdi.h>
#include <usbioctl.h>
#include <usb.h>

}

namespace axl {
namespace io {

//..............................................................................

size_t
enumerateHidMonDevices(
	sl::List<HidMonDeviceDesc>* deviceList,
	uint_t flags
) {
	static const wchar_t hwIdMiToken[] = L"&MI_";

	bool result;
	sl::String_w hidInterfacePath;
	sl::String_w string;
	uchar_t reportDescriptorBuffer[4 * 1024];

	// step 0 -- clear the output list

	deviceList->clear();

	// step 1 -- build a map of USPPcap-enabled devices

	sl::SimpleHashTable<uint32_t, const UsbMonDeviceDesc*> captureDeviceMap;

	sl::List<UsbMonDeviceDesc> captureDeviceList;
	enumerateUsbMonDevices(
		&captureDeviceList,
		UsbDeviceStringId_ConfigDescriptor |
		UsbDeviceStringId_HubInterfacePath
	);

	sl::ConstIterator<io::UsbMonDeviceDesc> it = captureDeviceList.getHead();
	for (size_t i = 0; it; it++, i++)
		captureDeviceMap[it->m_devInst] = &**it;

	// step 2 -- enumerate all HID devices

	sys::win::DeviceInfoSet deviceInfoSet;
	deviceInfoSet.create(GUID_DEVINTERFACE_HID, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	sl::StringHashTable_w<io::win::UsbHub> hubMap;

	for (size_t i = 0;; i++) {
		sys::win::DeviceInfo deviceInfo;
		result = deviceInfoSet.getDeviceInfo(i, &deviceInfo);
		if (!result)
			break;

		// step 2.1 -- find a corresponding USB capture device

		const UsbMonDeviceDesc* captureDeviceDesc = NULL;
		uint_t devInst = deviceInfo.getDevInfoData()->DevInst;
		for (;;) {
			result = sys::win::getParentDevInst(&devInst, devInst);
			if (!result)
				break;

			sl::ConstMapIterator<uint32_t,  const UsbMonDeviceDesc*> it = captureDeviceMap.find(devInst);
			if (it) {
				captureDeviceDesc = it->m_value;
				break;
			}
		}

		if (!captureDeviceDesc)
			continue;

		// step 2.2 -- get the HID interface path

		SP_DEVICE_INTERFACE_DATA ifaceData = { 0 };
		ifaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

		result =
			deviceInfo.enumDeviceInterfaces(GUID_DEVINTERFACE_HID, 0, &ifaceData) &&
			deviceInfo.getDeviceInterfacePath(&ifaceData, &hidInterfacePath);

		if (!result)
			continue;

		// step 2.3 -- fetch the HID report descriptor

		io::win::File file;
		result = file.create(hidInterfacePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING);
		if (!result)
			continue;

		io::win::File file2;
		result = file2.create(hidInterfacePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING);
		ASSERT(result);

		_HIDP_PREPARSED_DATA* preparsedData;
		result = HidD_GetPreparsedData(file, &preparsedData) != 0;
		file.close();

		if (!result)
			continue;

		int reportDescriptorSize = hid_winapi_descriptor_reconstruct_pp_data(
			preparsedData,
			reportDescriptorBuffer,
			sizeof(reportDescriptorBuffer)
		);

		HidD_FreePreparsedData(preparsedData);

		if (reportDescriptorSize == -1)
			continue;

		// step 2.2 -- find the interface ID for this capture device

		uchar_t interfaceId = 0;

		result = deviceInfo.getDeviceRegistryProperty(SPDRP_HARDWAREID, &string);
		if (result) {
			const wchar_t* p = string.cp();
			while (p < string.getEnd() && *p) {
				const wchar_t* p2 = wcsstr(p, hwIdMiToken);
				if (p2) {
					interfaceId = wcstoul(p2 + lengthof(hwIdMiToken), NULL, 16);
					break;
				}

				p += wcslen(p) + 1;
			}
		}

		// step 2.3 -- find the endpoint ID for this interface ID

		uchar_t currentInterfaceId = 0;
		uchar_t endpointId = 0;

		const char* p = captureDeviceDesc->m_configDescriptor.cp();
		const char* end = captureDeviceDesc->m_configDescriptor.getEnd();

		while (p < end) {
			uchar_t length = p[0];
			uchar_t descriptorType = p[1];

			union {
				const void* m_p;
				const USB_CONFIGURATION_DESCRIPTOR* m_configDesc;
				const USB_INTERFACE_DESCRIPTOR* m_ifaceDesc;
				const USB_ENDPOINT_DESCRIPTOR* m_epDesc;
			} desc = { p };

			switch (descriptorType) {
			case USB_INTERFACE_DESCRIPTOR_TYPE:
				currentInterfaceId = desc.m_ifaceDesc->bInterfaceNumber;
				break;

			case USB_ENDPOINT_DESCRIPTOR_TYPE:
				if (currentInterfaceId == interfaceId &&
					(desc.m_epDesc->bEndpointAddress & 0x80) &&
					(desc.m_epDesc->bmAttributes & USB_ENDPOINT_TYPE_MASK) == USB_ENDPOINT_TYPE_INTERRUPT
				) {
					endpointId = desc.m_epDesc->bEndpointAddress;
					p = end; // we are done
				}

				break;
			}

			p += length;
		}

		// step 2.4 -- create the resulting HID device desc

		HidMonDeviceDesc* deviceDesc = new HidMonDeviceDesc;
		*(UsbMonDeviceDesc*)deviceDesc = *captureDeviceDesc;
		deviceDesc->m_hidDeviceName = hidInterfacePath;
		deviceDesc->m_interfaceId = interfaceId;
		deviceDesc->m_endpointId = endpointId;
		deviceDesc->m_reportDescriptor.copy((char*)reportDescriptorBuffer, reportDescriptorSize);
		deviceList->insertTail(deviceDesc);

		if (captureDeviceDesc->m_hubInterfacePath.isEmpty())
			continue;

		// step 2.5 -- fetch descriptor strings

		io::win::UsbHub* hub = &hubMap.visit(captureDeviceDesc->m_hubInterfacePath)->m_value;
		if (!hub->isOpen()) {
			result = hub->open(captureDeviceDesc->m_hubInterfacePath);
			if (!result)
				continue;
		}

		deviceDesc->queryStrings(
			hub,
			&deviceInfo,
			captureDeviceDesc->m_port,
			captureDeviceDesc->m_manufacturerDescriptorId,
			captureDeviceDesc->m_productDescriptorId,
			captureDeviceDesc->m_serialNumberDescriptorId,
			&string,
			flags
		);
	}

	return deviceList->getCount();
}

//..............................................................................

} // namespace io
} // namespace axl
