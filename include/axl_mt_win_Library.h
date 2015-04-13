// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_WIN_MODULE_H

#include "axl_g_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace mt {
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

class Library: public Handle <HMODULE, FreeLibrary>
{
public:
	bool 
	loadLibrary (const wchar_t* name)
	{
		close ();

		m_h = ::LoadLibraryW (name);
		return err::complete (m_h != NULL);
	}

	void*
	getProcAddress (const char* name)
	{ 
		void* p = ::GetProcAddress (m_h, name);
		return err::complete (p, NULL);
	}
};

//.............................................................................

} // namespace win
} // namespace mt 
} // namespace axl
