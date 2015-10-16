// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_WIN_LIBRARY_H

#include "axl_sl_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//.............................................................................

class FreeLibrary
{
public:
	void
	operator () (HMODULE h)
	{
		::FreeLibrary (h);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Library: public sl::Handle <HMODULE, FreeLibrary>
{
public:
	bool 
	loadLibrary (const wchar_t* fileName);

	void*
	getProcAddress (const char* name)
	{ 
		void* p = ::GetProcAddress (m_h, name);
		return err::complete <void*> (p, NULL);
	}
};

//.............................................................................

} // namespace win
} // namespace sys 
} // namespace axl
