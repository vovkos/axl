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

#define _AXL_SYS_WIN_DEVICEINFO_H

#include "axl_sl_Handle.h"
#include "axl_sl_Array.h"
#include "axl_sl_String.h"
#include "axl_sl_BoxList.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

class DeviceInfoSet;

//..............................................................................

AXL_SELECT_ANY SP_DEVINFO_DATA g_nullDevInfoData = { sizeof(SP_DEVINFO_DATA) };

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class DeviceInfo
{
	friend class DeviceInfoSet;

protected:
	HDEVINFO m_devInfoSet;
	SP_DEVINFO_DATA m_devInfoData;

public:
	DeviceInfo()
	{
		m_devInfoSet = INVALID_HANDLE_VALUE;
		m_devInfoData = g_nullDevInfoData;
	}

	DeviceInfo(
		HDEVINFO devInfoSet,
		const SP_DEVINFO_DATA* devInfoData
		)
	{
		m_devInfoSet = devInfoSet;
		m_devInfoData = *devInfoData;
	}

	HDEVINFO
	getDevInfoSet()
	{
		return m_devInfoSet;
	}

	SP_DEVINFO_DATA*
	getDevInfoData()
	{
		return &m_devInfoData;
	}

	bool
	getDeviceRegistryProperty(
		uint_t propId,
		void* buffer,
		size_t size,
		dword_t* requiredSize
		)
	{
		bool_t result = ::SetupDiGetDeviceRegistryPropertyW(m_devInfoSet, &m_devInfoData, propId, NULL, (byte_t*)buffer, (dword_t)size, requiredSize);
		return err::complete(result != 0);
	}

	bool
	getDeviceRegistryProperty(
		uint_t propId,
		sl::Array<char>* buffer
		);

	bool
	getDeviceRegistryProperty(
		uint_t propId,
		sl::String_w* string
		);

	bool
	getDeviceRegistryProperty(
		uint_t propId,
		sl::String* string
		);

	bool
	setDeviceRegistryProperty(
		uint_t propId,
		const void* buffer,
		size_t size
		)
	{
		bool_t result = ::SetupDiSetDeviceRegistryPropertyW(m_devInfoSet, &m_devInfoData, propId, (const byte_t*) buffer, (dword_t)size);
		return err::complete(result != 0);
	}

	HKEY
	openDeviceRegistryKey(REGSAM keyAccess); // KEY_ALL_ACCESS, KEY_QUERY_VALUE, KEY_SET_VALUE etc

	bool
	getDeviceInstallParams(SP_DEVINSTALL_PARAMS_W* params)
	{
		bool_t result = ::SetupDiGetDeviceInstallParamsW(m_devInfoSet, &m_devInfoData, params);
		return err::complete(result != 0);
	}

	bool
	setDeviceInstallParams(const SP_DEVINSTALL_PARAMS_W* params)
	{
		bool_t result = ::SetupDiSetDeviceInstallParamsW(m_devInfoSet, &m_devInfoData, (SP_DEVINSTALL_PARAMS_W*)params);
		return err::complete(result != 0);
	}

	bool
	getClassInstallParams(
		void* buffer,
		size_t size,
		dword_t* requiredSize
		)
	{
		bool_t result = ::SetupDiGetClassInstallParamsW(m_devInfoSet, &m_devInfoData, (SP_CLASSINSTALL_HEADER*)buffer, (dword_t)size, requiredSize);
		return err::complete(result != 0);
	}

	bool
	getClassInstallParams(sl::Array<char>* buffer);

	bool
	setClassInstallParams(
		const void* params,
		size_t size
		)
	{
		bool_t result = ::SetupDiSetClassInstallParamsW(m_devInfoSet, &m_devInfoData, (SP_CLASSINSTALL_HEADER*)params, size);
		return err::complete(result != 0);
	}

	bool
	callClassInstaller(DI_FUNCTION function)
	{
		bool_t result = ::SetupDiCallClassInstaller(function, m_devInfoSet, &m_devInfoData);
		return err::complete(result != 0);
	}

	bool
	restartDevice(bool* isRebootRequired);
};

//..............................................................................

class DestroyDeviceInfoSet
{
public:
	void
	operator () (HDEVINFO h)
	{
		::SetupDiDestroyDeviceInfoList(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class DeviceInfoSet: public sl::Handle<HDEVINFO, DestroyDeviceInfoSet, sl::MinusOne<HDEVINFO> >
{
public:
	bool
	create(uint_t flags = DIGCF_PRESENT);

	bool
	create(
		const GUID& classGuid,
		uint_t flags = DIGCF_PRESENT
		);

	bool
	create(
		const sl::StringRef_w& enumerator,
		uint_t flags = DIGCF_PRESENT
		);

	bool
	getDeviceInfo(
		size_t i,
		DeviceInfo* deviceInfo
		);

	static
	bool
	getDeviceClassGuids(
		const sl::StringRef_w& name,
		GUID* buffer,
		size_t count,
		dword_t* requiredCount
		)
	{
		bool_t result = ::SetupDiClassGuidsFromNameW(name.sz(), buffer, (dword_t)count, requiredCount);
		return err::complete(result != 0);
	}

	static
	bool
	getDeviceClassGuids(
		const sl::StringRef_w& name,
		sl::Array<GUID>* buffer
		);
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
