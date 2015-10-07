// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_ERR_ERRORNO_H

#include "axl_err_Error.h"
#include "axl_sl_String.h"

namespace axl {
namespace err {

//.............................................................................

// {25A6A7B5-F662-48ae-BCB6-9A5CB5CE5BB9}
AXL_SL_DEFINE_GUID (
	g_ErrnoGuid,
	0x25a6a7b5, 0xf662, 0x48ae, 0xbc, 0xb6, 0x9a, 0x5c, 0xb5, 0xce, 0x5b, 0xb9
	);

//.............................................................................

class ErrnoProvider: public ErrorProvider
{
public:
	static
	sl::String
	getErrorDescription (int code);

	virtual
	sl::String
	getErrorDescription (const ErrorHdr* error)
	{
		return getErrorDescription (error->m_code);
	}
};

//.............................................................................

class Errno: public Error
{
public:
	Errno ()
	{
	}

	Errno (int code)
	{
		create (code);
	}

	ErrorHdr*
	create (int code)
	{
		return createSimpleError (g_ErrnoGuid, code);
	}
};

//.............................................................................

inline
Error
setErrno (int code)
{
	return setError (Errno (code));
}

//.............................................................................

} // namespace err
} // namespace axl

