// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_WIN_JOBOBJECT_H

#include "axl_g_win_Handle.h"

namespace axl {
namespace mt {
namespace win {

//.............................................................................

class JobObject: public sl::Handle <HANDLE, g::win::CloseHandle, sl::Zero <HANDLE> >
{
public:
	bool 
	create (
		SECURITY_ATTRIBUTES* secAttr,
		const wchar_t* name
		)
	{
		close ();

		m_h = ::CreateJobObjectW (secAttr, name);
		return err::complete (m_h != NULL);
	}

	bool 
	open (
		uint_t access,
		bool doInheritHandle,
		const wchar_t* name
		)
	{
		close ();

		m_h = ::OpenJobObjectW (access, doInheritHandle, name);
		return err::complete (m_h != NULL);
	}

	bool
	assignProcess (HANDLE process)
	{ 
		bool_t result = ::AssignProcessToJobObject (m_h, process);
		return err::complete (result);
	}

	bool
	setInformation (
		JOBOBJECTINFOCLASS infoClass,
		const void* p,
		size_t size
		)
	{
		bool_t result = ::SetInformationJobObject (m_h, infoClass, (void*) p,  (dword_t) size);
		return err::complete (result);
	}

	size_t 
	queryInformation (
		JOBOBJECTINFOCLASS infoClass,
		void* p,
		size_t size
		);
};

//.............................................................................

} // namespace win
} // namespace mt 
} // namespace axl
