// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_PROCESS_H

#include "axl_err_Error.h"

namespace axl {
namespace sys {

//.............................................................................

inline
bool
createProcess (const sl::StringRef& commandLine)
{	
#if (_AXL_OS_WIN)
	HINSTANCE hProcess = ::ShellExecuteW (0, L"open", sl::String_w (commandLine), 0, 0, SW_SHOWNORMAL);
	return err::complete (hProcess != NULL);
#else
	// TODO	
	return true;
#endif	
}

//.............................................................................

} // namespace sys
} // namespace axl

