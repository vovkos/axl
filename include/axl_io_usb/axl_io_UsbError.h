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

#define _AXL_IO_USBERROR_H

#include "axl_err_ErrorMgr.h"

namespace axl {
namespace io {

//..............................................................................

// {87EA0738-52E9-4769-B727-7A17377B921E}
AXL_SL_DEFINE_GUID(
	g_usbErrorGuid,
	0x87ea0738, 0x52e9, 0x4769, 0xb7, 0x27, 0x7a, 0x17, 0x37, 0x7b, 0x92, 0x1e
);

//..............................................................................

class UsbErrorProvider: public err::ErrorProvider {
public:
	static
	sl::StringRef
	getErrorDescription(int code);

	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error) {
		return getErrorDescription(error->m_code);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
registerUsbErrorProvider() {
	err::getErrorMgr()->registerProvider(
		g_usbErrorGuid,
		sl::getSimpleSingleton<UsbErrorProvider> ()
	);
}

//..............................................................................

class UsbError: public err::Error {
public:
	UsbError() {}

	UsbError(int code) {
		create(code);
	}

	size_t
	create(int code) {
		return createSimpleError(g_usbErrorGuid, code);
	}
};

//..............................................................................

} // namespace io
} // namespace axl
