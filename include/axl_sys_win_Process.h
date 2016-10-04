// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_WIN_PROCESS_H

#include "axl_sys_win_Handle.h"
#include "axl_sl_String.h"
#include "axl_sl_Array.h"

namespace axl {
namespace sys {
namespace win {

//.............................................................................

class Process: public Handle
{
public:
	bool 
	createProcess (
		const sl::StringRef_w& appName,
		const sl::StringRef_w& cmdLine,
		const SECURITY_ATTRIBUTES* processAttr,
		const SECURITY_ATTRIBUTES* threadAttr,
		bool inheritHandles,
		dword_t flags,
		const void* environment,
		const sl::StringRef_w& currentDir,
		const STARTUPINFO* startupInfo,
		handle_t* threadHandle
		);

	bool 
	createProcess (
		const sl::StringRef_w& cmdLine,
		bool inheritHandles,
		dword_t flags,
		const STARTUPINFO* startupInfo		
		)
	{
		return createProcess (
			NULL, 
			cmdLine, 
			NULL, 
			NULL, 
			inheritHandles, 
			flags, 
			NULL, 
			NULL, 
			startupInfo, 
			NULL
			);
	}

	dword_t
	getProcessId ()
	{
		dword_t id = ::GetProcessId (m_h);
		return err::complete <dword_t> (id, -1);
	}

	bool
	getExitCode (dword_t* exitCode)
	{
		bool_t result = ::GetExitCodeProcess (m_h, exitCode);
		return err::complete (result);
	}
};

//.............................................................................

bool
syncExec (
	const sl::StringRef_w& cmdLine,
	sl::Array <char>* output,
	dword_t* exitCode = NULL
	);

//.............................................................................

} // namespace win
} // namespace sys 
} // namespace axl
