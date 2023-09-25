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

#define _AXL_SYS_WIN_CFGMGR_H

#include "axl_sys_win_CfgMgrPch.h"
#include "axl_sys_win_CfgMgrError.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

inline
bool
locateDevInst(
	uint_t* devInst,
	const sl::StringRef_w& instanceId,
	uint_t flags = 0
) {
	CONFIGRET cret = ::CM_Locate_DevNodeW((DEVINST*)devInst, (wchar_t*)instanceId.sz(), flags);
	return completeWithCfgMgrError(cret);
}

inline
bool
getParentDevInst(
	uint_t* parentDevInst,
	uint_t devInst
) {
	CONFIGRET cret = ::CM_Get_Parent((DEVINST*)parentDevInst, devInst, 0);
	return completeWithCfgMgrError(cret);
}

inline
bool
getChildDevInst(
	uint_t* childDevInst,
	uint_t devInst
) {
	CONFIGRET cret = ::CM_Get_Child((DEVINST*)childDevInst, devInst, 0);
	return completeWithCfgMgrError(cret);
}

inline
bool
getSiblingDevInst(
	uint_t* siblingDevInst,
	uint_t devInst
) {
	CONFIGRET cret = ::CM_Get_Sibling((DEVINST*)siblingDevInst, devInst, 0);
	return completeWithCfgMgrError(cret);
}

bool
getDeviceInterfaceStringProperty(
	sl::String_w* string,
	const sl::StringRef_w& interfacePath,
	const DEVPROPKEY& key
);

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
