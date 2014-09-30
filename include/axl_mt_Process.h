// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_PROCESS_H

#include "axl_err_Error.h"

namespace axl {
namespace mt {

//.............................................................................

inline
bool
createProcess (const char* commandLine)
{	
#if (_AXL_ENV == AXL_ENV_WIN)
	HINSTANCE hProcess = ::ShellExecuteW (0, L"open", rtl::CString_w (commandLine), 0, 0, SW_SHOWNORMAL);
	return err::complete (hProcess != NULL);
#else
	// TODO	
	return true;
#endif	
}

//.............................................................................

} // namespace mt
} // namespace axl

