// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_WIN_SERVICEMGR_H

#include "axl_sys_win_Service.h"

namespace axl {
namespace sys {
namespace win {

//.............................................................................

class ServiceMgr: public ServiceHandle
{
public:
	bool 
	open (
		const wchar_t* machineName,
		const wchar_t* databaseName,
		dword_t access
		);

	bool
	createService (
		Service* service,
		const wchar_t* name,
		const wchar_t* displayName,
		dword_t access,
		dword_t serviceType,
		dword_t startType,
		dword_t errorControl,
		const wchar_t* binaryFilePath,
		const wchar_t* loadOrderGroup = NULL,
		dword_t* tagId = NULL,
		const wchar_t* dependencies = NULL,
		const wchar_t* startName = NULL,
		const wchar_t* password = NULL
		);

	bool
	openService (
		Service* service,
		const wchar_t* name,
		dword_t access = SERVICE_ALL_ACCESS
		);
};

//.............................................................................

} // namespace win
} // namespace sys 
} // namespace axl
