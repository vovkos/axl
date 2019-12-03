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

#define _AXL_SYS_PROCESS_H

#include "axl_err_Error.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_Process.h"
#elif (_AXL_OS_POSIX)
#	include "axl_sys_psx_Process.h"
#endif

namespace axl {
namespace sys {

//..............................................................................

inline
uint_t
getCurrentProcessId()
{
#if (_AXL_OS_WIN)
	return ::GetCurrentProcessId();
#else
	return (uint_t)::getpid();
#endif
}

inline
bool
createProcess(const sl::StringRef& commandLine)
{
#if (_AXL_OS_WIN)
	HINSTANCE hProcess = ::ShellExecuteW(0, L"open", sl::String_w (commandLine), 0, 0, SW_SHOWNORMAL);
	return err::complete(hProcess != NULL);
#else
	// TODO
	return true;
#endif
}

inline
sl::String
getProcessImageName(uint_t pid)
{
#if (_AXL_OS_WIN)
	return sys::win::getProcessImageName(pid);
#elif (_AXL_OS_POSIX)
	return sys::psx::getProcessImageName(pid);
#endif
}

//..............................................................................

} // namespace sys
} // namespace axl
