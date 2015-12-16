// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_WIN_SID_H

#include "axl_sl_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//.............................................................................

class FreeSid 
{
public:
	void
	operator () (PSID sid)
	{
		::FreeSid (sid);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Sid: public sl::Handle <PSID, FreeSid>
{
public:
	bool 
	create (
		SID_IDENTIFIER_AUTHORITY* identifierAuthority,
		size_t subAuthorityCount,
		dword_t subAuthority0,
		dword_t subAuthority1 = 0,
		dword_t subAuthority2 = 0,
		dword_t subAuthority3 = 0,
		dword_t subAuthority4 = 0,
		dword_t subAuthority5 = 0,
		dword_t subAuthority6 = 0,
		dword_t subAuthority7 = 0
		);
};

//.............................................................................

} // namespace win
} // namespace sys 
} // namespace axl
