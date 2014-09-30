// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_ERR_WINERROR_H

#include "axl_err_Error.h"
#include "axl_rtl_String.h"

namespace axl {
namespace err {

//.............................................................................

// {54E100E8-2137-40b8-BCD8-00AC1D0BAA16}
AXL_RTL_DEFINE_GUID (
	GUID_WinError,
	0x54e100e8, 0x2137, 0x40b8, 0xbc, 0xd8, 0x0, 0xac, 0x1d, 0xb, 0xaa, 0x16
	);

//.............................................................................

class CWinErrorProvider: public CErrorProvider
{
public:
	static
	rtl::CString 
	getErrorDescription (dword_t code);

	virtual 
	rtl::CString 
	getErrorDescription (const TError* error)
	{
		return getErrorDescription (error->m_code);
	}
};

//.............................................................................

class CWinError: public CError
{
public:
	CWinError ()
	{
	}

	CWinError (dword_t code)
	{
		create (code);
	}

	TError* 
	create (dword_t code);
};

//.............................................................................

inline
CError
setWinError (dword_t code)
{
	setError (CWinError (code));
}

//.............................................................................

} // namespace err
} // namespace axl

