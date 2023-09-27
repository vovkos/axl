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

#include <initguid.h>
#include <devpkey.h>

namespace axl {
namespace sys {
namespace win {

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
