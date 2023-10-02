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

#define _AXL_IO_HIDMONENUMERATOR_H

#include "axl_io_UsbMonEnumerator.h"

namespace axl {
namespace io {

//..............................................................................

struct HidMonDeviceDesc: UsbMonDeviceDesc {
	sl::String m_hidDeviceName;
	sl::Array<char> m_reportDescriptor;
	uint_t m_interfaceId;
	uint_t m_endpointId;
};

//..............................................................................

size_t
enumerateHidMonDevices(
	sl::List<HidMonDeviceDesc>* deviceList,
	uint_t flags = 0
);

//..............................................................................

} // namespace io
} // namespace axl
