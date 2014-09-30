// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_IO_USBERROR_H

#include "axl_io_UsbError.h"
#include "axl_err_Error.h"
#include "axl_rtl_String.h"

namespace axl {
namespace io {

//.............................................................................

// {87EA0738-52E9-4769-B727-7A17377B921E}
AXL_RTL_DEFINE_GUID (
	GUID_UsbError,
	0x87ea0738, 0x52e9, 0x4769, 0xb7, 0x27, 0x7a, 0x17, 0x37, 0x7b, 0x92, 0x1e
	);

//.............................................................................

class CUsbErrorProvider: public err::CErrorProvider
{
public:
	static
	rtl::CString 
	getErrorDescription (int code)
	{
		return libusb_error_name (code);
	}

	virtual 
	rtl::CString 
	getErrorDescription (const err::TError* error)
	{
		return getErrorDescription (error->m_code);
	}
};

//.............................................................................

class CUsbError: public err::CError
{
public:
	CUsbError ()
	{
	}

	CUsbError (int code)
	{
		create (code);
	}

	err::TError* 
	create (int code);
};

//.............................................................................

inline
err::CError
setUsbError (int code)
{
	setError (CUsbError (code));
}

//.............................................................................

} // namespace io {
} // namespace axl {
