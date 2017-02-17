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

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
DeviceInfo::getDeviceRegistryProperty (
	uint_t propId,
	sl::Array <char>* buffer
	)
{
	dword_t requiredSize = 0;
	getDeviceRegistryProperty (propId, NULL, 0, &requiredSize);

	buffer->setCount (requiredSize);
	return getDeviceRegistryProperty (propId, buffer->p (), requiredSize, NULL);
}

bool
DeviceInfo::getDeviceRegistryProperty (
	uint_t propId,
	sl::String* string
	)
{
	sl::Array <char> buffer;
	bool result = getDeviceRegistryProperty (propId, &buffer);
	if (!result)
		return false;

	string->copy ((const utf16_t*) buffer.cp (), buffer.getCount () / sizeof (utf16_t));
	return true;
}

bool
DeviceInfo::getDeviceRegistryProperty (
	uint_t propId,
	sl::String_w* string
	)
{
	sl::Array <char> buffer;
	bool result = getDeviceRegistryProperty (propId, &buffer);
	if (!result)
		return false;

	string->copy ((const utf16_t*) buffer.cp (), buffer.getCount () / sizeof (utf16_t));
	return true;
}

HKEY
DeviceInfo::openDeviceRegistryKey (REGSAM keyAccess)
{
	HKEY key = ::SetupDiOpenDevRegKey (m_devInfoSet, &m_devInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, keyAccess);
	if (key == INVALID_HANDLE_VALUE)
		err::setLastSystemError ();

	return key;
}

bool
DeviceInfo::getClassInstallParams (sl::Array <char>* buffer)
{
	dword_t requiredSize = 0;
	getClassInstallParams (NULL, 0, &requiredSize);

	buffer->setCount (requiredSize);
	return getClassInstallParams (buffer->p (), requiredSize, NULL);
}

bool
DeviceInfo::restartDevice (bool* isRebootRequired)
{
	bool result;

	SP_PROPCHANGE_PARAMS propChangeParams = { 0 };

	propChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
	propChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
	propChangeParams.StateChange = DICS_STOP;
	propChangeParams.Scope = DICS_FLAG_CONFIGSPECIFIC;
	propChangeParams.HwProfile = 0; // current profile

	result =
		setClassInstallParams (&propChangeParams, sizeof (propChangeParams)) &&
		callClassInstaller (DIF_PROPERTYCHANGE);

	if (!result)
		return false;

	propChangeParams.StateChange = DICS_START;

	result =
		setClassInstallParams (&propChangeParams, sizeof (propChangeParams)) &&
		callClassInstaller (DIF_PROPERTYCHANGE);

	if (!result)
		return false;

	SP_DEVINSTALL_PARAMS_W devInstallParams = { 0 };
	devInstallParams.cbSize = sizeof (devInstallParams);
	result = getDeviceInstallParams (&devInstallParams);
	if (!result)
		return false;

	if (isRebootRequired)
		*isRebootRequired = (devInstallParams.Flags & DI_NEEDREBOOT) != 0;

	return true;
}

//..............................................................................

bool
DeviceInfoSet::create (uint_t flags)
{
	close ();

	m_h = ::SetupDiGetClassDevsW (NULL, NULL, NULL, flags | DIGCF_ALLCLASSES);
	return err::complete (m_h != INVALID_HANDLE_VALUE);
}

bool
DeviceInfoSet::create (
	const GUID& classGuid,
	uint_t flags
	)
{
	close ();

	m_h = ::SetupDiGetClassDevsW (&classGuid, NULL, NULL, flags);
	return err::complete (m_h != INVALID_HANDLE_VALUE);
}

bool
DeviceInfoSet::create (
	const sl::StringRef_w& enumerator,
	uint_t flags
	)
{
	close ();

	m_h = ::SetupDiGetClassDevsW (NULL, enumerator.szn (), NULL, flags);
	return err::complete (m_h != INVALID_HANDLE_VALUE);
}

bool
DeviceInfoSet::getDeviceInfo (
	size_t i,
	DeviceInfo* deviceInfo
	)
{
	bool_t result = ::SetupDiEnumDeviceInfo (m_h, (DWORD) i, &deviceInfo->m_devInfoData);
	if (!result)
		return err::failWithLastSystemError ();

	deviceInfo->m_devInfoSet = m_h;
	return true;
}

bool
DeviceInfoSet::getDeviceClassGuids (
	const sl::StringRef_w& name,
	sl::Array <GUID>* buffer
	)
{
	dword_t requiredCount = 0;
	getDeviceClassGuids (name, NULL, 0, &requiredCount);

	buffer->setCount (requiredCount);
	return getDeviceClassGuids (name, buffer->p (), requiredCount, &requiredCount);
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
