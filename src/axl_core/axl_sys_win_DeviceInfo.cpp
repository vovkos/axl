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

#include "pch.h"
#include "axl_sys_win_DeviceInfo.h"
#include "axl_sys_win_Registry.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
DeviceInfo::getDeviceInstanceId(sl::String_w* string) {
	dword_t length = 0;
	getDeviceInstanceId(NULL, 0, &length);

	wchar_t* p = string->createBuffer(length);
	bool result = getDeviceInstanceId(p, length, NULL);
	if (!result)
		return false;

	ASSERT(length && (*string)[length - 1] == 0);
	string->overrideLength(length - 1);
	return true;
}

bool
DeviceInfo::getDeviceDriverPath(sl::String_w* string) {
	sl::String_w serviceName;
	bool result = getDeviceRegistryProperty(SPDRP_SERVICE, &serviceName);
	if (!result)
		return false;

	sl::String_w regPath = L"System\\CurrentControlSet\\Services\\";
	regPath += serviceName;

	sys::win::RegKey regKey;
	return
		regKey.open(HKEY_LOCAL_MACHINE, regPath, KEY_QUERY_VALUE) &&
		regKey.queryValue(string, L"ImagePath");
}

bool
DeviceInfo::getDeviceRegistryProperty(
	uint_t propId,
	sl::Array<char>* buffer
) {
	dword_t requiredSize = 0;
	getDeviceRegistryProperty(propId, NULL, 0, &requiredSize);

	buffer->setCount(requiredSize);
	return getDeviceRegistryProperty(propId, buffer->p(), requiredSize, NULL);
}

bool
DeviceInfo::getClassInstallParams(sl::Array<char>* buffer) {
	dword_t requiredSize = 0;
	getClassInstallParams(NULL, 0, &requiredSize);

	buffer->setCount(requiredSize);
	return getClassInstallParams(buffer->p(), requiredSize, NULL);
}

bool
DeviceInfo::restartDevice(bool* isRebootRequired) {
	bool result;

	SP_PROPCHANGE_PARAMS propChangeParams = { 0 };

	propChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
	propChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
	propChangeParams.StateChange = DICS_STOP;
	propChangeParams.Scope = DICS_FLAG_CONFIGSPECIFIC;
	propChangeParams.HwProfile = 0; // current profile

	result =
		setClassInstallParams(&propChangeParams, sizeof(propChangeParams)) &&
		callClassInstaller(DIF_PROPERTYCHANGE);

	if (!result)
		return false;

	propChangeParams.StateChange = DICS_START;

	result =
		setClassInstallParams(&propChangeParams, sizeof(propChangeParams)) &&
		callClassInstaller(DIF_PROPERTYCHANGE);

	if (!result)
		return false;

	SP_DEVINSTALL_PARAMS_W devInstallParams = { 0 };
	devInstallParams.cbSize = sizeof(devInstallParams);
	result = getDeviceInstallParams(&devInstallParams);
	if (!result)
		return false;

	if (isRebootRequired)
		*isRebootRequired = (devInstallParams.Flags & DI_NEEDREBOOT) != 0;

	return true;
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
