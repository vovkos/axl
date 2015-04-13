// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_WIN_MODULE_H

#include "axl_rtl_Handle.h"
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

class Library: public rtl::Handle <HMODULE, FreeLibrary>
{
public:
	bool 
	loadLibrary (const wchar_t* fileName)
	{
		close ();

		m_h = ::LoadLibraryW (fileName);
		return err::complete (m_h != NULL);
	}

	void*
	getProcAddress (const char* name)
	{ 
		void* p = ::GetProcAddress (m_h, name);
		return err::complete <void*> (p, NULL);
	}
};

//.............................................................................

} // namespace win
} // namespace mt 
} // namespace axl
