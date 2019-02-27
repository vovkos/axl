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

#define _AXL_ERR_NTSTATUS_H

#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

// {81443347-3BC9-4d5e-8B23-32D780EDB52B}
AXL_SL_DEFINE_GUID(
	g_ntStatusGuid,
	0x81443347, 0x3bc9, 0x4d5e, 0x8b, 0x23, 0x32, 0xd7, 0x80, 0xed, 0xb5, 0x2b
	);

//..............................................................................

class NtStatusProvider: public err::ErrorProvider
{
public:
	static
	sl::String
	getErrorDescription(long status);

	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error)
	{
		return getErrorDescription(error->m_code);
	}
};

//..............................................................................

class NtStatus: public err::Error
{
public:
	NtStatus()
	{
	}

	NtStatus(long status)
	{
		create(status);
	}

	size_t
	create(long status);
};

//..............................................................................

inline
size_t
setNtStatus(long status)
{
	return setError(NtStatus(status));
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
