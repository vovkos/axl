// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_ERR_ERRORNO_H

#include "axl_err_Error.h"
#include "axl_rtl_String.h"

namespace axl {
namespace err {

//.............................................................................

// {25A6A7B5-F662-48ae-BCB6-9A5CB5CE5BB9}
AXL_RTL_DEFINE_GUID (
	GUID_Errno,
	0x25a6a7b5, 0xf662, 0x48ae, 0xbc, 0xb6, 0x9a, 0x5c, 0xb5, 0xce, 0x5b, 0xb9
	);

//.............................................................................

class CErrnoProvider: public CErrorProvider
{
public:
	static
	rtl::CString
	getErrorDescription (int code)
	{
		char buffer [512] = { 0 };
#if (_AXL_ENV == AXL_ENV_WIN)
		strerror_s (buffer, countof (buffer) - 1, code);
		return buffer;
#else
		return strerror_r (error, buffer, countof (buffer) - 1);
#endif
	}

	virtual
	rtl::CString
	getErrorDescription (const TError* error)
	{
		return getErrorDescription (error->m_code);
	}
};

//.............................................................................

class CErrno: public CError
{
public:
	CErrno ()
	{
	}

	CErrno (int code)
	{
		create (code);
	}

	TError*
	create (int code);
};

//.............................................................................

inline
CError
setErrno (int code)
{
	setError (CErrno (code));
}

//.............................................................................

} // namespace err
} // namespace axl

