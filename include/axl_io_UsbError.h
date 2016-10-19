// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_USBERROR_H

#include "axl_io_UsbPch.h"

namespace axl {
namespace io {

//..............................................................................

// {87EA0738-52E9-4769-B727-7A17377B921E}
AXL_SL_DEFINE_GUID (
	g_usbErrorGuid,
	0x87ea0738, 0x52e9, 0x4769, 0xb7, 0x27, 0x7a, 0x17, 0x37, 0x7b, 0x92, 0x1e
	);

//..............................................................................

class UsbErrorProvider: public err::ErrorProvider
{
public:
	static
	sl::StringRef
	getErrorDescription (int code)
	{
		return libusb_error_name (code);
	}

	virtual
	sl::StringRef
	getErrorDescription (const err::ErrorRef& error)
	{
		return getErrorDescription (error->m_code);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
registerUsbErrorProvider ()
{
	err::getErrorMgr ()->registerProvider (
		g_usbErrorGuid,
		sl::getSimpleSingleton <UsbErrorProvider> ()
		);
}

//..............................................................................

class UsbError: public err::Error
{
public:
	UsbError ()
	{
	}

	UsbError (int code)
	{
		create (code);
	}

	err::ErrorHdr*
	create (int code);
};

//..............................................................................

inline
size_t
setUsbError (int code)
{
	return setError (UsbError (code));
}

//..............................................................................

} // namespace io
} // namespace axl
