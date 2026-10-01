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
#include "axl_sl_Handle.h"

#if (WINVER < _WIN32_WINNT_WIN8)

// required excerpt from cfgmgr32.h

//..............................................................................

DECLARE_HANDLE(HCMNOTIFICATION);
typedef HCMNOTIFICATION* PHCMNOTIFICATION;

#define CM_NOTIFY_FILTER_FLAG_ALL_INTERFACE_CLASSES 0x00000001
#define CM_NOTIFY_FILTER_FLAG_ALL_DEVICE_INSTANCES  0x00000002

typedef enum _CM_NOTIFY_FILTER_TYPE {
	CM_NOTIFY_FILTER_TYPE_DEVICEINTERFACE = 0,
	CM_NOTIFY_FILTER_TYPE_DEVICEHANDLE,
	CM_NOTIFY_FILTER_TYPE_DEVICEINSTANCE,
	CM_NOTIFY_FILTER_TYPE_MAX
} CM_NOTIFY_FILTER_TYPE, *PCM_NOTIFY_FILTER_TYPE;

typedef struct _CM_NOTIFY_FILTER {
	DWORD cbSize;
	DWORD Flags;
	CM_NOTIFY_FILTER_TYPE FilterType;
	DWORD Reserved;

	union {
		struct {
			GUID ClassGuid;
		} DeviceInterface;

		struct {
			HANDLE hTarget;
		} DeviceHandle;

		struct {
			WCHAR InstanceId[MAX_DEVICE_ID_LEN];
		} DeviceInstance;
	} u;
} CM_NOTIFY_FILTER, *PCM_NOTIFY_FILTER;

typedef enum _CM_NOTIFY_ACTION {
	CM_NOTIFY_ACTION_DEVICEINTERFACEARRIVAL = 0,
	CM_NOTIFY_ACTION_DEVICEINTERFACEREMOVAL,
	CM_NOTIFY_ACTION_DEVICEQUERYREMOVE,
	CM_NOTIFY_ACTION_DEVICEQUERYREMOVEFAILED,
	CM_NOTIFY_ACTION_DEVICEREMOVEPENDING,
	CM_NOTIFY_ACTION_DEVICEREMOVECOMPLETE,
	CM_NOTIFY_ACTION_DEVICECUSTOMEVENT,
	CM_NOTIFY_ACTION_DEVICEINSTANCEENUMERATED,
	CM_NOTIFY_ACTION_DEVICEINSTANCESTARTED,
	CM_NOTIFY_ACTION_DEVICEINSTANCEREMOVED,
	CM_NOTIFY_ACTION_MAX
} CM_NOTIFY_ACTION, *PCM_NOTIFY_ACTION;

typedef struct _CM_NOTIFY_EVENT_DATA {
	CM_NOTIFY_FILTER_TYPE FilterType;
	DWORD Reserved;

	union {
		struct {
			GUID ClassGuid;
			WCHAR SymbolicLink[ANYSIZE_ARRAY];
		} DeviceInterface;

		struct {
			GUID EventGuid;
			LONG NameOffset;
			DWORD DataSize;
			BYTE Data[ANYSIZE_ARRAY];
		} DeviceHandle;

		struct {
			WCHAR InstanceId[ANYSIZE_ARRAY];
		} DeviceInstance;
	} u;
} CM_NOTIFY_EVENT_DATA, *PCM_NOTIFY_EVENT_DATA;

typedef
DWORD
(CALLBACK* PCM_NOTIFY_CALLBACK)(
	HCMNOTIFICATION hNotify,
	PVOID Context,
	CM_NOTIFY_ACTION Action,
	PCM_NOTIFY_EVENT_DATA EventData,
	DWORD EventDataSize
);

#endif

//..............................................................................

namespace axl {
namespace sys {
namespace win {

//..............................................................................

typedef
CONFIGRET
WINAPI
CmRegisterNotificationFunc(
	PCM_NOTIFY_FILTER filter,
	PVOID context,
	PCM_NOTIFY_CALLBACK callback,
	PHCMNOTIFICATION notification
);

typedef
CONFIGRET
WINAPI
CmUnregisterNotificationFunc(HCMNOTIFICATION notification);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

extern AXL_SELECT_ANY CmRegisterNotificationFunc* cmRegisterNotification = NULL;
extern AXL_SELECT_ANY CmUnregisterNotificationFunc* cmUnregisterNotification = NULL;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
initCfgMgrFunctions();

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// CfgMgr notifications work on Windows 8 and above

class UnregisterCfgMgrNotification {
public:
	void
	operator () (HCMNOTIFICATION h) {
		ASSERT(cmUnregisterNotification);
		cmUnregisterNotification(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CfgMgrNotification: public sl::Handle<HCMNOTIFICATION, UnregisterCfgMgrNotification> {
public:
	bool
	create(
		const CM_NOTIFY_FILTER* filter,
		void* context,
		PCM_NOTIFY_CALLBACK callback
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#if (_AXL_SYS_WIN_GETDEVICEINTERFACESTRINGPROPERTY) // hide it for now

bool
getDeviceInterfaceStringProperty(
	sl::String_w* string,
	const sl::StringRef_w& interfacePath,
	const DEVPROPKEY& key
);

#endif

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
