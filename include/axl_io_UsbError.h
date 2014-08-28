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
	GetErrorDescription (int Error)
	{
		return libusb_error_name (Error);
	}

	virtual 
	rtl::CString 
	GetErrorDescription (const err::TError* pError)
	{
		return GetErrorDescription (pError->m_Code);
	}
};

//.............................................................................

class CUsbError: public err::CError
{
public:
	CUsbError ()
	{
	}

	CUsbError (int Error)
	{
		Create (Error);
	}

	err::TError* 
	Create (int Error);
};

//.............................................................................

inline
err::CError
SetUsbError (int Error)
{
	SetError (CUsbError (Error));
}

//.............................................................................

} // namespace io {
} // namespace axl {
