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

#include "axl_sys_win_Registry.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

class DeviceInfoSet;

//..............................................................................

AXL_SELECT_ANY SP_DEVINFO_DATA g_nullDevInfoData = { sizeof(SP_DEVINFO_DATA) };

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class DeviceInfo {
	friend class DeviceInfoSet;

protected:
	HDEVINFO m_devInfoSet;
	SP_DEVINFO_DATA m_devInfoData;

public:
	DeviceInfo() {
		m_devInfoSet = INVALID_HANDLE_VALUE;
		m_devInfoData = g_nullDevInfoData;
	}

	DeviceInfo(
		HDEVINFO devInfoSet,
		const SP_DEVINFO_DATA* devInfoData
	) {
		m_devInfoSet = devInfoSet;
		m_devInfoData = *devInfoData;
	}

	HDEVINFO
	getDevInfoSet() {
		return m_devInfoSet;
	}

	SP_DEVINFO_DATA*
	getDevInfoData() {
		return &m_devInfoData;
	}

	bool
	getDeviceInstanceId(
		wchar_t* buffer,
		size_t length,
		dword_t* requiredLength
	) {
		bool_t result = ::SetupDiGetDeviceInstanceId(m_devInfoSet, &m_devInfoData, buffer, (dword_t)length, requiredLength);
		return err::complete(result != 0);
	}

	bool
	getDeviceInstanceId(sl::String_w* string);

	template <typename T>
	bool
	getDeviceInstanceId(sl::StringBase<T>* string);

	sl::String_w
	getDeviceInstanceId();

	bool
	getDeviceDriverPath(sl::String_w* string);

	template <typename T>
	bool
	getDeviceDriverPath(sl::StringBase<T>* string);

	sl::String_w
	getDeviceDriverPath();

	bool
	getDeviceRegistryProperty(
		uint_t propId,
		void* buffer,
		size_t size,
		dword_t* requiredSize
	) {
		bool_t result = ::SetupDiGetDeviceRegistryPropertyW(m_devInfoSet, &m_devInfoData, propId, NULL, (byte_t*)buffer, (dword_t)size, requiredSize);
		return err::complete(result != 0);
	}

	bool
	getDeviceRegistryProperty(
		uint_t propId,
		sl::Array<char>* buffer
	);

	template <typename T>
	bool
	getDeviceRegistryProperty(
		uint_t propId,
		sl::StringBase<T>* string
	);

	sl::String_w
	getDeviceRegistryProperty(uint_t propId);

	bool
	setDeviceRegistryProperty(
		uint_t propId,
		const void* buffer,
		size_t size
	) {
		bool_t result = ::SetupDiSetDeviceRegistryPropertyW(m_devInfoSet, &m_devInfoData, propId, (const byte_t*) buffer, (dword_t)size);
		return err::complete(result != 0);
	}

	bool
	openDeviceRegistryKey(
		RegKey* regKey,
		REGSAM keyAccess = KEY_ALL_ACCESS
	);

	HKEY
	openDeviceRegistryKey(REGSAM keyAccess = KEY_ALL_ACCESS);

	bool
	getDeviceInstallParams(SP_DEVINSTALL_PARAMS_W* params) {
		bool_t result = ::SetupDiGetDeviceInstallParamsW(m_devInfoSet, &m_devInfoData, params);
		return err::complete(result != 0);
	}

	bool
	setDeviceInstallParams(const SP_DEVINSTALL_PARAMS_W* params) {
		bool_t result = ::SetupDiSetDeviceInstallParamsW(m_devInfoSet, &m_devInfoData, (SP_DEVINSTALL_PARAMS_W*)params);
		return err::complete(result != 0);
	}

	bool
	getClassInstallParams(
		void* buffer,
		size_t size,
		dword_t* requiredSize
	) {
		bool_t result = ::SetupDiGetClassInstallParamsW(m_devInfoSet, &m_devInfoData, (SP_CLASSINSTALL_HEADER*)buffer, (dword_t)size, requiredSize);
		return err::complete(result != 0);
	}

	bool
	getClassInstallParams(sl::Array<char>* buffer);

	bool
	setClassInstallParams(
		const void* params,
		size_t size
	) {
		bool_t result = ::SetupDiSetClassInstallParamsW(m_devInfoSet, &m_devInfoData, (SP_CLASSINSTALL_HEADER*)params, size);
		return err::complete(result != 0);
	}

	bool
	callClassInstaller(DI_FUNCTION function) {
		bool_t result = ::SetupDiCallClassInstaller(function, m_devInfoSet, &m_devInfoData);
		return err::complete(result != 0);
	}

	bool
	restartDevice(bool* isRebootRequired);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
bool
DeviceInfo::getDeviceInstanceId(sl::StringBase<T>* string) {
	sl::String_w instanceId;
	getDeviceInstanceId(&instanceId);
	*string = instanceId;
	return true;
}

inline
sl::String_w
DeviceInfo::getDeviceInstanceId() {
	sl::String_w string;
	getDeviceInstanceId(&string);
	return string;
}

template <typename T>
bool
DeviceInfo::getDeviceDriverPath(sl::StringBase<T>* string) {
	sl::String_w driverPath;
	getDeviceDriverPath(&driverPath);
	*string = driverPath;
	return true;
}

inline
sl::String_w
DeviceInfo::getDeviceDriverPath() {
	sl::String_w string;
	getDeviceDriverPath(&string);
	return string;
}

template <typename T>
bool
DeviceInfo::getDeviceRegistryProperty(
	uint_t propId,
	sl::StringBase<T>* string
) {
	sl::Array<char> buffer;
	bool result = getDeviceRegistryProperty(propId, &buffer);
	if (!result)
		return false;

	size_t size = buffer.getCount();
	if (size <= sizeof(wchar_t))
		string->clear();
	else
		string->copy((const wchar_t*)buffer.cp(),  size / sizeof(wchar_t) - 1);

	return true;
}

inline
sl::String_w
DeviceInfo::getDeviceRegistryProperty(uint_t propId) {
	sl::String_w string;
	getDeviceRegistryProperty(propId, &string);
	return string;
}

inline
HKEY
DeviceInfo::openDeviceRegistryKey(REGSAM keyAccess) {
	HKEY h = ::SetupDiOpenDevRegKey(m_devInfoSet, &m_devInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, keyAccess);
	if (h == INVALID_HANDLE_VALUE)
		err::setLastSystemError();

	return h;
}

inline
bool
DeviceInfo::openDeviceRegistryKey(
	RegKey* regKey,
	REGSAM keyAccess
) {
	HKEY h = openDeviceRegistryKey(keyAccess);
	regKey->attach(h);
	return regKey->isOpen();
}

//..............................................................................

class DestroyDeviceInfoSet {
public:
	void
	operator () (HDEVINFO h) {
		::SetupDiDestroyDeviceInfoList(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class DeviceInfoSet: public sl::Handle<HDEVINFO, DestroyDeviceInfoSet, sl::MinusOne<HDEVINFO> > {
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
	) const;

	static
	bool
	getDeviceClassGuids(
		const sl::StringRef_w& name,
		GUID* buffer,
		size_t count,
		dword_t* requiredCount
	) {
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
DeviceInfoSet::create(uint_t flags) {
	close();

	m_h = ::SetupDiGetClassDevsW(NULL, NULL, NULL, flags | DIGCF_ALLCLASSES);
	return err::complete(m_h != INVALID_HANDLE_VALUE);
}

inline
bool
DeviceInfoSet::create(
	const GUID& classGuid,
	uint_t flags
) {
	close();

	m_h = ::SetupDiGetClassDevsW(&classGuid, NULL, NULL, flags);
	return err::complete(m_h != INVALID_HANDLE_VALUE);
}

inline
bool
DeviceInfoSet::create(
	const sl::StringRef_w& enumerator,
	uint_t flags
) {
	close();

	m_h = ::SetupDiGetClassDevsW(NULL, enumerator.szn(), NULL, flags);
	return err::complete(m_h != INVALID_HANDLE_VALUE);
}

inline
bool
DeviceInfoSet::getDeviceInfo(
	size_t i,
	DeviceInfo* deviceInfo
) const {
	bool_t result = ::SetupDiEnumDeviceInfo(m_h, (DWORD)i, &deviceInfo->m_devInfoData);
	if (!result)
		return err::failWithLastSystemError();

	deviceInfo->m_devInfoSet = m_h;
	return true;
}

inline
bool
DeviceInfoSet::getDeviceClassGuids(
	const sl::StringRef_w& name,
	sl::Array<GUID>* buffer
) {
	dword_t requiredCount = 0;
	getDeviceClassGuids(name, NULL, 0, &requiredCount);

	buffer->setCount(requiredCount);
	return getDeviceClassGuids(name, buffer->p(), requiredCount, &requiredCount);
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
