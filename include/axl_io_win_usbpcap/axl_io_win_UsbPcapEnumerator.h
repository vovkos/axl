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

#pragma once

#define _AXL_IO_WIN_USBPCAPENUMERATOR_H

#include "axl_io_win_UsbPcapPch.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

enum UsbPcapDeviceFlag {
	UsbPcapDeviceFlag_Hub      = 0x01,
	UsbPcapDeviceFlag_LowSpeed = 0x02,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum UsbPcapEnumMask {
	UsbPcapEnumMask_Description  = 0x0001,
	UsbPcapEnumMask_Manufacturer = 0x0002,
	UsbPcapEnumMask_HardwareId   = 0x0004,
	UsbPcapEnumMask_Driver       = 0x0008,
	UsbPcapEnumMask_Location     = 0x0010,
	UsbPcapEnumMask_InstanceId   = 0x0020,
	UsbPcapEnumMask_Class        = 0x0040,
	UsbPcapEnumMask_Descriptors  = 0x0080,
	UsbPcapEnumMask_Hubs         = 0x0100,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum UsbPcapDescriptor {
	UsbPcapDescriptor_Manufacturer,
	UsbPcapDescriptor_Product,
	UsbPcapDescriptor_SerialNumber,
	UsbPcapDescriptor__Count
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct UsbPcapListEntry: sl::ListLink {
	sl::String_w m_pcapDeviceName;
	sl::String_w m_hubDeviceName;
};

typedef UsbPcapListEntry UsbPcapHub;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
struct UsbPcapDeviceBase: UsbPcapListEntry {
	sl::StringBase<T> m_description;
	sl::StringBase<T> m_manufacturer;
	sl::StringBase<T> m_hardwareId;
	sl::StringBase<T> m_driverKeyName;
	sl::StringBase<T> m_driverPath;
	sl::StringBase<T> m_location;
	sl::StringBase<T> m_instanceId;
	sl::StringBase<T> m_descriptorTable[UsbPcapDescriptor__Count];
	uint_t m_hubPort;
	uint_t m_deviceAddress;
	uint_t m_deviceClass;
	uint_t m_deviceSubClass;
	uint_t m_vendorId;
	uint_t m_productId;
	uint_t m_flags;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef UsbPcapDeviceBase<char> UsbPcapDevice;
typedef UsbPcapDeviceBase<wchar_t> UsbPcapDevice_w;

//..............................................................................


size_t
enumerateUsbPcapRootHubs(sl::List<UsbPcapHub>* hubList);

size_t
enumerateUsbPcapDevices(
	sl::List<UsbPcapDevice>* deviceList,
	uint_t mask = 0
);

size_t
enumerateUsbPcapDevices(
	sl::List<UsbPcapDevice_w>* deviceList,
	uint_t mask = 0
);

size_t
enumerateUsbPcapDevices(
	sl::List<UsbPcapDevice>* hubDescList,
	const UsbPcapHub& hub,
	uint_t mask = 0
);

size_t
enumerateUsbPcapDevices(
	sl::List<UsbPcapDevice_w>* hubDescList,
	const UsbPcapHub& hub,
	uint_t mask = 0
);

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
