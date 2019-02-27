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

#define _AXL_SYS_WIN_JOBOBJECT_H

#include "axl_sys_win_Handle.h"
#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class JobObject: public sl::Handle<HANDLE, CloseHandle, sl::Zero<HANDLE> >
{
public:
	bool
	create(
		SECURITY_ATTRIBUTES* secAttr,
		const sl::StringRef_w& name
		)
	{
		close();

		m_h = ::CreateJobObjectW(secAttr, name.szn());
		return err::complete(m_h != NULL);
	}

	bool
	open(
		uint_t access,
		bool doInheritHandle,
		const sl::StringRef_w& name
		)
	{
		close();

		m_h = ::OpenJobObjectW(access, doInheritHandle, name.szn());
		return err::complete(m_h != NULL);
	}

	bool
	assignProcess(HANDLE process)
	{
		bool_t result = ::AssignProcessToJobObject(m_h, process);
		return err::complete(result);
	}

	bool
	setInformation(
		JOBOBJECTINFOCLASS infoClass,
		const void* p,
		size_t size
		)
	{
		bool_t result = ::SetInformationJobObject(m_h, infoClass, (void*)p,  (dword_t)size);
		return err::complete(result);
	}

	size_t
	queryInformation(
		JOBOBJECTINFOCLASS infoClass,
		void* p,
		size_t size
		);
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
