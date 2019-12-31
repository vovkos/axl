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

#define _AXL_SYS_WIN_SERVICEMGR_H

#include "axl_sys_win_Service.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class ServiceMgr: public ServiceHandle
{
public:
	bool
	open(
		const sl::StringRef_w& machineName,
		const sl::StringRef_w& databaseName,
		dword_t access
		);

	bool
	createService(
		Service* service,
		const sl::StringRef_w& name,
		const sl::StringRef_w& displayName,
		dword_t access,
		dword_t serviceType,
		dword_t startType,
		dword_t errorControl,
		const sl::StringRef_w& binaryFilePath,
		const sl::StringRef_w& loadOrderGroup = NULL,
		dword_t* tagId = NULL,
		const sl::StringRef_w& dependencies = NULL,
		const sl::StringRef_w& startName = NULL,
		const sl::StringRef_w& password = NULL
		);

	bool
	openService(
		Service* service,
		const sl::StringRef_w& name,
		dword_t access = SERVICE_ALL_ACCESS
		);
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
