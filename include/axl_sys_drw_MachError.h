// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_SYS_DRW_MachError_H

#include "axl_err_Error.h"
#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace drw {

//.............................................................................

// {E8840A46-6DE7-4427-B356-24B2ED4B08D1}
AXL_SL_DEFINE_GUID (
	g_MachErrorGuid,
	0xe8840a46, 0x6de7, 0x4427, 0xb3, 0x56, 0x24, 0xb2, 0xed, 0x4b, 0x8, 0xd1
	);

//.............................................................................

class MachErrorProvider: public err::ErrorProvider
{
public:
	static
	sl::String 
	getErrorDescription (mach_error_t code)
	{
		return ::mach_error_string (code);
	}

	virtual 
	sl::String 
	getErrorDescription (const err::ErrorHdr* error)
	{
		return getErrorDescription (error->m_code);
	}
};

//.............................................................................

class MachError: public err::Error
{
public:
	MachError ()
	{
	}

	MachError (mach_error_t code)
	{
		create (code);
	}

	err::ErrorHdr*
	create (mach_error_t code);
};

//.............................................................................

inline
err::Error
setMachError (mach_error_t code)
{
	return err::setError (MachError (code));
}

//.............................................................................

} // namespace drw
} // namespace sys
} // namespace axl

