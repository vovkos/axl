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

#define _AXL_SYS_DRW_MACHERROR_H

#include "axl_err_Error.h"
#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace drw {

//..............................................................................

// {E8840A46-6DE7-4427-B356-24B2ED4B08D1}
AXL_SL_DEFINE_GUID(
	g_MachErrorGuid,
	0xe8840a46, 0x6de7, 0x4427, 0xb3, 0x56, 0x24, 0xb2, 0xed, 0x4b, 0x8, 0xd1
	);

//..............................................................................

class MachErrorProvider: public err::ErrorProvider
{
public:
	static
	sl::StringRef
	getErrorDescription(mach_error_t code)
	{
		return ::mach_error_string(code);
	}

	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error)
	{
		return getErrorDescription(error->m_code);
	}
};

//..............................................................................

class MachError: public err::Error
{
public:
	MachError()
	{
	}

	MachError(mach_error_t code)
	{
		create(code);
	}

	size_t
	create(mach_error_t code);
};

//..............................................................................

inline
size_t
setMachError(mach_error_t code)
{
	return err::setError(MachError(code));
}

//..............................................................................

} // namespace drw
} // namespace sys
} // namespace axl
