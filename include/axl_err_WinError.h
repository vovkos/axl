// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_ERR_WINERROR_H

#include "axl_err_Error.h"
#include "axl_sl_String.h"

namespace axl {
namespace err {

//.............................................................................

// {54E100E8-2137-40b8-BCD8-00AC1D0BAA16}
AXL_SL_DEFINE_GUID (
	g_winErrorGuid,
	0x54e100e8, 0x2137, 0x40b8, 0xbc, 0xd8, 0x0, 0xac, 0x1d, 0xb, 0xaa, 0x16
	);

//.............................................................................

class WinErrorProvider: public ErrorProvider
{
public:
	static
	sl::String 
	getErrorDescription (dword_t code);

	virtual 
	sl::StringRef
	getErrorDescription (const ErrorRef& error)
	{
		return getErrorDescription (error->m_code);
	}
};

//.............................................................................

class WinError: public Error
{
public:
	WinError ()
	{
	}

	WinError (dword_t code)
	{
		create (code);
	}

	ErrorHdr* 
	create (dword_t code);
};

//.............................................................................

inline
size_t
setWinError (dword_t code)
{
	return setError (WinError (code));
}

//.............................................................................

} // namespace err
} // namespace axl

