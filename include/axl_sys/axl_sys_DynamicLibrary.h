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

#define _AXL_SYS_DYNAMICLIBRARY_H

#include "axl_g_Pch.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_Library.h"
#	include "axl_sl_String.h"
#elif (_AXL_OS_POSIX)
#	include "axl_sys_psx_DynamicLibrary.h"
#endif

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

class DynamicLibrary
{
protected:
	win::Library m_library;

public:
	bool
	isOpen() const
	{
		return m_library.isOpen();
	}

	bool
	open(const sl::StringRef& fileName)
	{
		char buffer[256];
		sl::String_w fileName_w(ref::BufKind_Stack, buffer, sizeof(buffer));
		fileName_w = fileName;

		return m_library.loadLibrary(fileName_w);
	}

	void
	close()
	{
		m_library.close();
	}

	void
	detach()
	{
		m_library.detach();
	}

	void*
	getFunction(const sl::StringRef& name)
	{
		return m_library.getProcAddress(name);
	}
};

//..............................................................................

#elif (_AXL_OS_POSIX)

class DynamicLibrary
{
protected:
	psx::DynamicLibrary m_library;

public:
	bool
	isOpen() const
	{
		return m_library.isOpen();
	}

	bool
	open(const sl::StringRef& fileName)
	{
		return m_library.open(fileName);
	}

	void
	close()
	{
		m_library.close();
	}

	void
	detach()
	{
		m_library.detach();
	}

	void*
	getFunction(const sl::StringRef& name)
	{
		return m_library.getSymbol(name);
	}
};

#endif

//..............................................................................

} // namespace sys
} // namespace axl
