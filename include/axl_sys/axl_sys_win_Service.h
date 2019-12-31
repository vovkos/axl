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

#define _AXL_SYS_WIN_SERVICE_H

#include "axl_sys_win_Handle.h"
#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class CloseServiceHandle
{
public:
	void
	operator () (SC_HANDLE h)
	{
		::CloseServiceHandle(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef sl::Handle<SC_HANDLE, CloseServiceHandle> ServiceHandle;

//..............................................................................

class Service: public ServiceHandle
{
public:
	bool
	start(
		const wchar_t* argv[],
		size_t argc
		)
	{
		bool_t result = ::StartServiceW(m_h, argc, argv);
		return err::complete(result);
	}

	bool
	start()
	{
		return start(NULL, 0);
	}

	bool
	stop()
	{
		SERVICE_STATUS serviceStatus;
		return control(SERVICE_CONTROL_STOP, &serviceStatus);
	}

	bool
	control(
		dword_t code,
		SERVICE_STATUS* serviceStatus
		)
	{
		bool_t result = ::ControlService(m_h, code, serviceStatus);
		return err::complete(result);
	}

	bool
	remove()
	{
		bool_t result = ::DeleteService(m_h);
		return err::complete(result);
	}

	bool
	setServiceType(dword_t serviceType)
	{
		return changeServiceConfig(serviceType, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE);
	}

	bool
	setStartType(dword_t startType)
	{
		return changeServiceConfig(SERVICE_NO_CHANGE, startType, SERVICE_NO_CHANGE);
	}

	bool
	setErrorControl(dword_t errorControl)
	{
		return changeServiceConfig(SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, errorControl);
	}

	bool
	setDescription(const sl::StringRef_w& description);

	bool
	queryServiceStatus(SERVICE_STATUS* serviceStatus)
	{
		bool_t result = ::QueryServiceStatus(m_h, serviceStatus);
		return err::complete(result);
	}

	bool
	changeServiceConfig(
		dword_t serviceType,
		dword_t startType,
		dword_t errorControl,
		const sl::StringRef_w& binaryFilePath = NULL,
		const sl::StringRef_w& loadOrderGroup = NULL,
		dword_t* tagId = NULL,
		const sl::StringRef_w& dependencies = NULL,
		const sl::StringRef_w& serviceStartName = NULL,
		const sl::StringRef_w& password = NULL,
		const sl::StringRef_w& displayName = NULL
		)
	{
		bool_t result = ::ChangeServiceConfigW(
			m_h,
			serviceType,
			startType,
			errorControl,
			binaryFilePath.szn(),
			loadOrderGroup.szn(),
			tagId,
			dependencies.szn(),
			serviceStartName.szn(),
			password.szn(),
			displayName.szn()
			);

		return err::complete(result);
	}

	bool
	changeServiceConfig2(
		dword_t infoLevel,
		const void* info
		)
	{
		bool_t result = ::ChangeServiceConfig2W(m_h, infoLevel, (void*)info);
		return err::complete(result);
	}
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
