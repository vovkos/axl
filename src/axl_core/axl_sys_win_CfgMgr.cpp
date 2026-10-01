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
#include "axl_sys_win_CfgMgr.h"
#include "axl_sys_win_CfgMgrError.h"
#include "axl_sys_win_Library.h"

#include <initguid.h>
#include <devpkey.h>

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
initCfgMgrFunctions() {
	Library cfgMgr;
	if (!cfgMgr.loadLibrary(L"cfgmgr32.dll"))
		return false;

	cmRegisterNotification = (CmRegisterNotificationFunc*)cfgMgr.getProcAddress("CM_Register_Notification");
	cmUnregisterNotification = (CmUnregisterNotificationFunc*)cfgMgr.getProcAddress("CM_Unregister_Notification");
	cfgMgr.detach(); // keep it loaded

	return
		cmRegisterNotification != NULL &&
		cmUnregisterNotification != NULL;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
CfgMgrNotification::create(
	const CM_NOTIFY_FILTER* filter,
	void* context,
	PCM_NOTIFY_CALLBACK callback
) {
	close();

	if (!cmRegisterNotification || !cmUnregisterNotification)
		return err::fail(err::SystemErrorCode_NotImplemented);

	HCMNOTIFICATION h;
	CONFIGRET cret = cmRegisterNotification((PCM_NOTIFY_FILTER)filter, context, callback, &h);
	if (cret != CR_SUCCESS)
		return fail(CfgMgrError(cret));

	m_h = h;
	return true;
}

//..............................................................................

#if (_AXL_SYS_WIN_GETDEVICEINTERFACESTRINGPROPERTY)

bool
getDeviceInterfaceStringProperty(
	sl::String_w* string,
	const sl::StringRef_w& interfacePath,
	const DEVPROPKEY& key
) {
	DEVPROPTYPE type;
	ulong_t size = 0;

	CONFIGRET cret = ::CM_Get_Device_Interface_PropertyW(
		interfacePath.sz(),
		&key,
		&type,
		NULL,
		&size,
		0
	);

	if (cret != CR_BUFFER_SMALL)
		return err::fail(sys::win::CfgMgrError(cret));

	if (type != DEVPROP_TYPE_STRING ||
		size < sizeof(wchar_t) ||
		(size & (sizeof(wchar_t) - 1))
	)
		return err::fail("CM_Get_Device_Interface_PropertyW returned unexpected value");

	wchar_t* p = string->createBuffer(size / sizeof(wchar_t) - 1);
	if (!p)
		return false;

	cret = ::CM_Get_Device_Interface_PropertyW(
		interfacePath.sz(),
		&key,
		&type,
		(byte_t*)p,
		&size,
		0
	);

	return completeWithCfgMgrError(cret);
}

#endif

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
