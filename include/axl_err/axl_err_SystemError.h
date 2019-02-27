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

#define _AXL_ERR_ERROR_H

#include "axl_sl_Guid.h"

#if (_AXL_OS_WIN)

namespace axl {
namespace sys {
namespace win {

extern const sl::Guid g_winErrorGuid;

} // namespace axl
} // namespace sys
} // namespace win

#endif

namespace axl {
namespace err {

//..............................................................................

#if (_AXL_OS_WIN)

AXL_SELECT_ANY const sl::Guid& g_systemErrorGuid = sys::win::g_winErrorGuid;

enum SystemErrorCode
{
	SystemErrorCode_Success                  = ERROR_SUCCESS,
	SystemErrorCode_Cancelled                = ERROR_OPERATION_ABORTED,
	SystemErrorCode_Unsuccessful             = ERROR_GEN_FAILURE,
	SystemErrorCode_InsufficientResources    = ERROR_NO_SYSTEM_RESOURCES,
	SystemErrorCode_NotImplemented           = ERROR_INVALID_FUNCTION,
	SystemErrorCode_InvalidHandle            = ERROR_INVALID_HANDLE,
	SystemErrorCode_AddressAlreadyExists     = ERROR_DUP_NAME,
	SystemErrorCode_InvalidAddress           = ERROR_UNEXP_NET_ERR,
	SystemErrorCode_AddressClosed            = ERROR_NETNAME_DELETED,
	SystemErrorCode_BufferOverflow           = ERROR_MORE_DATA,
	SystemErrorCode_InvalidParameter         = ERROR_INVALID_PARAMETER,
	SystemErrorCode_ConnectionRefused        = ERROR_CONNECTION_REFUSED,
	SystemErrorCode_ConnectionAborted        = ERROR_CONNECTION_ABORTED,
	SystemErrorCode_ConnectionReset          = ERROR_NETNAME_DELETED,
	SystemErrorCode_IoTimeout                = WAIT_TIMEOUT,
	SystemErrorCode_InvalidDeviceState       = ERROR_INVALID_HANDLE_STATE,
	SystemErrorCode_NetworkUnreachable       = ERROR_NETWORK_UNREACHABLE,
	SystemErrorCode_HostUnreachable          = ERROR_HOST_UNREACHABLE,
	SystemErrorCode_InvalidDeviceRequest     = ERROR_INVALID_FUNCTION,
	SystemErrorCode_BufferTooSmall           = ERROR_INSUFFICIENT_BUFFER,
	SystemErrorCode_ObjectNameNotFound       = ERROR_FILE_NOT_FOUND,
	SystemErrorCode_AccessDenied             = ERROR_ACCESS_DENIED,
};

inline
uint_t
getLastSystemErrorCode()
{
	return ::GetLastError();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#elif (_AXL_OS_POSIX)

extern const sl::Guid g_errnoGuid;
AXL_SELECT_ANY const sl::Guid& g_systemErrorGuid = g_errnoGuid;

enum SystemErrorCode
{
	SystemErrorCode_Success                  = 0,
	SystemErrorCode_Cancelled                = ECANCELED,
	SystemErrorCode_Unsuccessful             = EIO,
	SystemErrorCode_InsufficientResources    = ENOMEM,
	SystemErrorCode_NotImplemented           = ENOSYS,
	SystemErrorCode_InvalidHandle            = EBADF,
	SystemErrorCode_AddressAlreadyExists     = EEXIST,
	SystemErrorCode_InvalidAddress           = EINVAL,
	SystemErrorCode_BufferOverflow           = EOVERFLOW,
	SystemErrorCode_InvalidParameter         = EINVAL,
	SystemErrorCode_ConnectionRefused        = ECONNREFUSED,
	SystemErrorCode_ConnectionAborted        = ECONNABORTED,
	SystemErrorCode_ConnectionReset          = ECONNRESET,
	SystemErrorCode_IoTimeout                = ETIMEDOUT,
	SystemErrorCode_InvalidDeviceState       = EBUSY,
	SystemErrorCode_NetworkUnreachable       = ENETUNREACH,
	SystemErrorCode_HostUnreachable          = EHOSTUNREACH,
	SystemErrorCode_InvalidDeviceRequest     = ENODEV,
	SystemErrorCode_BufferTooSmall           = ENOBUFS,
	SystemErrorCode_ObjectNameNotFound       = ENOENT,
	SystemErrorCode_AccessDenied             = EACCES,
};

inline
uint_t
getLastSystemErrorCode()
{
	return errno;
}

#endif

//..............................................................................

} // namespace err
} // namespace axl
