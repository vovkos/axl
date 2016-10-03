// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_ERR_NTERROR_H

#include "axl_err_Error.h"
#include "axl_sl_String.h"

namespace axl {
namespace err {

//.............................................................................

typedef long NTSTATUS;

// {81443347-3BC9-4d5e-8B23-32D780EDB52B}
AXL_SL_DEFINE_GUID (
	g_ntErrorGuid,
	0x81443347, 0x3bc9, 0x4d5e, 0x8b, 0x23, 0x32, 0xd7, 0x80, 0xed, 0xb5, 0x2b
	);

//.............................................................................
	
class NtErrorProvider: public ErrorProvider
{
public:
	static
	sl::String 
	getErrorDescription (NTSTATUS status);

	virtual 
	sl::String 
	getErrorDescription (const ErrorRef& error)
	{
		return getErrorDescription (error->m_code);
	}
};

//.............................................................................

class NtError: public Error
{
public:
	NtError ()
	{
	}

	NtError (NTSTATUS status)
	{
		create (status);
	}

	ErrorHdr* 
	create (NTSTATUS status);
};

//.............................................................................

inline
size_t
setNtError (NTSTATUS status)
{
	return setError (NtError (status));
}

//.............................................................................


} // namespace err
} // namespace axl

