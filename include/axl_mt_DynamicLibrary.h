// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_DYNAMICLIBRARY_H

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_mt_win_Library.h"
#	include "axl_rtl_String.h"
#elif (_AXL_ENV == AXL_ENV_POSIX)
#	include "axl_mt_psx_DynamicLibrary.h"
#endif

namespace axl {
namespace mt {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

class DynamicLibrary
{
public:
	win::Library m_library;

public:
	bool
	load (const char* fileName)
	{
		char buffer [256];
		rtl::String_w fileName_w (ref::BufKind_Stack, buffer, sizeof (buffer));
		fileName_w = fileName;
		
		return m_library.loadLibrary (fileName_w);
	}

	void
	release ()
	{
		m_library.close ();
	}

	void*
	getFunction (const char* name)
	{
		return m_library.getProcAddress (name);
	}
};

//.............................................................................

#elif (_AXL_ENV == AXL_ENV_POSIX)

class DynamicLibrary
{
protected:
	psx::DynamicLibrary m_dynamicLibrary;

public:
	bool
	load (const char* fileName)
	{
		return m_dynamicLibrary.open (fileName);
	}

	void
	release ()
	{
		m_dynamicLibrary.close ();
	}

	void*
	getFunction (const char* name)
	{
		return m_dynamicLibrary.getSymbol (name);
	}
};

#endif

//.............................................................................

} // namespace mt
} // namespace axl