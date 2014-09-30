// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_ERR_ERROR_H

#include "axl_ref_Buf.h"
#include "axl_rtl_Guid.h"

namespace axl {
namespace rtl {

template <typename T> class CStringT;
typedef CStringT <char> CString;

} // namespace rtl
} // namespace axl

namespace axl {
namespace err {

//.............................................................................

enum EErrorMode
{
	EErrorMode_NoThrow       = 0,
	EErrorMode_CppException  = 1,

	// these 2 are almost useless unless in C-style code without classes

	EErrorMode_SehException  = 2,
	EErrorMode_SetJmpLongJmp = 4,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

EErrorMode
getErrorMode ();

EErrorMode // returns previous one
setErrorMode (EErrorMode mode);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CScopeErrorMode
{
protected:
	EErrorMode m_oldMode;

public:
	CScopeErrorMode (EErrorMode mode)
	{
		m_oldMode = setErrorMode (mode);
	}

	~CScopeErrorMode ()
	{
		setErrorMode (m_oldMode);
	}
};

//.............................................................................

// axl std errors

extern
AXL_SELECT_ANY
const rtl::TGuid GUID_StdError = rtl::GUID_Null;

enum EStdError
{
	EStdError_NoError,
	EStdError_String,
	EStdError_Stack,
};

//.............................................................................

// system errors

#if (_AXL_ENV == AXL_ENV_WIN)

extern const rtl::TGuid GUID_WinError;

#define GUID_SystemError GUID_WinError

enum EStatus
{
	EStatus_Success                  = ERROR_SUCCESS,
	EStatus_Pending                  = ERROR_IO_PENDING,
	EStatus_Cancelled                = ERROR_OPERATION_ABORTED,
	EStatus_Unsuccessful             = ERROR_GEN_FAILURE,
	EStatus_InsufficientResources    = ERROR_NO_SYSTEM_RESOURCES,
	EStatus_NotImplemented           = ERROR_INVALID_FUNCTION,
	EStatus_InvalidHandle            = ERROR_INVALID_HANDLE,
	EStatus_AddressAlreadyExists     = ERROR_DUP_NAME,
	EStatus_InvalidAddressComponent  = ERROR_INVALID_NETNAME,
	EStatus_TooManyAddresses         = ERROR_TOO_MANY_NAMES,
	EStatus_InvalidAddress           = ERROR_UNEXP_NET_ERR,
	EStatus_AddressClosed            = ERROR_NETNAME_DELETED,
	EStatus_BufferOverflow           = ERROR_MORE_DATA,
	EStatus_InvalidParameter         = ERROR_INVALID_PARAMETER,
	EStatus_ConnectionRefused        = ERROR_CONNECTION_REFUSED,
	EStatus_ConnectionInvalid        = ERROR_CONNECTION_INVALID,
	EStatus_AddressAlreadyAssociated = ERROR_ADDRESS_ALREADY_ASSOCIATED,
	EStatus_AddressNotAssociated     = ERROR_ADDRESS_NOT_ASSOCIATED,
	EStatus_ConnectionActive         = ERROR_CONNECTION_ACTIVE,
	EStatus_ConnectionAborted        = ERROR_CONNECTION_ABORTED,
	EStatus_ConnectionReset          = ERROR_NETNAME_DELETED,
	EStatus_IoTimeout                = ERROR_SEM_TIMEOUT,
	EStatus_GracefulDisconnect       = ERROR_GRACEFUL_DISCONNECT,
	EStatus_DataNotAccepted          = ERROR_INVALID_DATA,
	EStatus_MoreProcessingRequired   = ERROR_MORE_DATA,
	EStatus_InvalidDeviceState       = ERROR_BAD_COMMAND,
	EStatus_NetworkUnreachable       = ERROR_NETWORK_UNREACHABLE,
	EStatus_HostUnreachable          = ERROR_HOST_UNREACHABLE,
	EStatus_ProtocolUnreachable      = ERROR_PROTOCOL_UNREACHABLE,
	EStatus_PortUnreachable          = ERROR_PORT_UNREACHABLE,
	EStatus_InvalidDeviceRequest     = ERROR_INVALID_FUNCTION,
	EStatus_RequestAborted           = ERROR_REQUEST_ABORTED,
	EStatus_BufferTooSmall           = ERROR_INSUFFICIENT_BUFFER,
	EStatus_InvalidBufferSize        = ERROR_INVALID_USER_BUFFER,
	EStatus_ObjectNameNotFound       = ERROR_FILE_NOT_FOUND,
	EStatus_AccessDenied             = ERROR_ACCESS_DENIED,
	EStatus_SharingViolation         = ERROR_SHARING_VIOLATION,
	EStatus_NoMoreEntries            = ERROR_NO_MORE_ITEMS,
};

inline
uint_t
getLastSystemErrorCode ()
{
	return ::GetLastError ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#elif (_AXL_ENV == AXL_ENV_NT)

extern const rtl::TGuid GUID_NtError;

#define GUID_SystemError GUID_NtError

enum EStatus
{
	EStatus_Success                  = STATUS_SUCCESS,
	EStatus_Pending                  = STATUS_PENDING,
	EStatus_Cancelled                = STATUS_CANCELLED,
	EStatus_Unsuccessful             = STATUS_UNSUCCESSFUL,
	EStatus_InsufficientResources    = STATUS_INSUFFICIENT_RESOURCES,
	EStatus_NotImplemented           = STATUS_NOT_IMPLEMENTED,
	EStatus_InvalidHandle            = STATUS_INVALID_HANDLE,
	EStatus_AddressAlreadyExists     = STATUS_ADDRESS_ALREADY_EXISTS,
	EStatus_InvalidAddressComponent  = STATUS_INVALID_ADDRESS_COMPONENT,
	EStatus_TooManyAddresses         = STATUS_TOO_MANY_ADDRESSES,
	EStatus_InvalidAddress           = STATUS_INVALID_ADDRESS,
	EStatus_AddressClosed            = STATUS_ADDRESS_CLOSED,
	EStatus_BufferOverflow           = STATUS_BUFFER_OVERFLOW,
	EStatus_InvalidParameter         = STATUS_INVALID_PARAMETER,
	EStatus_ConnectionRefused        = STATUS_CONNECTION_REFUSED,
	EStatus_ConnectionInvalid        = STATUS_CONNECTION_INVALID,
	EStatus_AddressAlreadyAssociated = STATUS_ADDRESS_ALREADY_ASSOCIATED,
	EStatus_AddressNotAssociated     = STATUS_ADDRESS_NOT_ASSOCIATED,
	EStatus_ConnectionActive         = STATUS_CONNECTION_ACTIVE,
	EStatus_ConnectionAborted        = STATUS_CONNECTION_ABORTED,
	EStatus_ConnectionReset          = STATUS_CONNECTION_RESET,
	EStatus_IoTimeout                = STATUS_IO_TIMEOUT,
	EStatus_GracefulDisconnect       = STATUS_GRACEFUL_DISCONNECT,
	EStatus_DataNotAccepted          = STATUS_DATA_NOT_ACCEPTED,
	EStatus_MoreProcessingRequired   = STATUS_MORE_PROCESSING_REQUIRED,
	EStatus_InvalidDeviceState       = STATUS_INVALID_DEVICE_STATE,
	EStatus_NetworkUnreachable       = STATUS_NETWORK_UNREACHABLE,
	EStatus_HostUnreachable          = STATUS_HOST_UNREACHABLE,
	EStatus_ProtocolUnreachable      = STATUS_PROTOCOL_UNREACHABLE,
	EStatus_PortUnreachable          = STATUS_PORT_UNREACHABLE,
	EStatus_InvalidDeviceRequest     = STATUS_INVALID_DEVICE_REQUEST,
	EStatus_RequestAborted           = STATUS_REQUEST_ABORTED,
	EStatus_BufferTooSmall           = STATUS_BUFFER_TOO_SMALL,
	EStatus_InvalidBufferSize        = STATUS_INVALID_BUFFER_SIZE,
	EStatus_ObjectNameNotFound       = STATUS_OBJECT_NAME_NOT_FOUND,
	EStatus_AccessDenied             = STATUS_ACCESS_DENIED,
	EStatus_SharingViolation         = STATUS_SHARING_VIOLATION,
	EStatus_NoMoreEntries            = STATUS_NO_MORE_ENTRIES,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#elif (_AXL_ENV == AXL_ENV_POSIX)

extern const rtl::TGuid GUID_Errno;

#define GUID_SystemError GUID_Errno

enum EStatus
{
	EStatus_Success                  = 0,
	EStatus_Cancelled                = EINTR,
	EStatus_Unsuccessful             = EIO,
	EStatus_InsufficientResources    = ENOMEM,
	EStatus_NotImplemented           = ENOSYS,
	EStatus_InvalidHandle            = EBADF,
	EStatus_AddressAlreadyExists     = EEXIST,
/*	EStatus_InvalidAddressComponent  = ERROR_INVALID_NETNAME,
	EStatus_TooManyAddresses         = ERROR_TOO_MANY_NAMES,
	EStatus_InvalidAddress           = ERROR_UNEXP_NET_ERR,
	EStatus_AddressClosed            = ERROR_NETNAME_DELETED,
	EStatus_BufferOverflow           = ERROR_MORE_DATA,*/
	EStatus_InvalidParameter         = EINVAL,
/*	EStatus_ConnectionRefused        = ERROR_CONNECTION_REFUSED,
	EStatus_ConnectionInvalid        = ERROR_CONNECTION_INVALID,
	EStatus_AddressAlreadyAssociated = ERROR_ADDRESS_ALREADY_ASSOCIATED,
	EStatus_AddressNotAssociated     = ERROR_ADDRESS_NOT_ASSOCIATED,
	EStatus_ConnectionActive         = ERROR_CONNECTION_ACTIVE,
	EStatus_ConnectionAborted        = ERROR_CONNECTION_ABORTED,
	EStatus_ConnectionReset          = ERROR_NETNAME_DELETED,
	EStatus_IoTimeout                = ERROR_SEM_TIMEOUT,
	EStatus_GracefulDisconnect       = ERROR_GRACEFUL_DISCONNECT,
	EStatus_DataNotAccepted          = ERROR_INVALID_DATA,
	EStatus_MoreProcessingRequired   = ERROR_MORE_DATA, */
	EStatus_InvalidDeviceState       = EBUSY,
/*	EStatus_NetworkUnreachable       = ERROR_NETWORK_UNREACHABLE,
	EStatus_HostUnreachable          = ERROR_HOST_UNREACHABLE,
	EStatus_ProtocolUnreachable      = ERROR_PROTOCOL_UNREACHABLE,
	EStatus_PortUnreachable          = ERROR_PORT_UNREACHABLE,*/
	EStatus_InvalidDeviceRequest     = ENOSYS,
/*	EStatus_RequestAborted           = ERROR_REQUEST_ABORTED,
	EStatus_BufferTooSmall           = ERROR_INSUFFICIENT_BUFFER,
	EStatus_InvalidBufferSize        = ERROR_INVALID_USER_BUFFER,
	EStatus_ObjectNameNotFound       = ERROR_FILE_NOT_FOUND,
	EStatus_AccessDenied             = ERROR_ACCESS_DENIED,
	EStatus_SharingViolation         = ERROR_SHARING_VIOLATION,
	EStatus_NoMoreEntries            = ERROR_NO_MORE_ITEMS, */
};

inline
uint_t
getLastSystemErrorCode ()
{
	return errno;
}

#endif

//.............................................................................

// POD structure

struct TError
{
	uint32_t m_size;
	rtl::TGuid m_guid;
	uint32_t m_code;

	// possibly followed by error data

	rtl::CString
	getDescription () const;

	bool
	isKind (
		const rtl::TGuid& guid,
		uint_t code
		) const;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

extern
AXL_SELECT_ANY
const TError noError =
{
	sizeof (TError),
	GUID_StdError,
	EStatus_Success,
};

//.............................................................................

class CGetErrorSize
{
public:
	size_t
	operator () (const TError& x)
	{
		ASSERT (x.m_size >= sizeof (TError));
		return AXL_MAX (x.m_size, sizeof (TError));
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// ref-counted error buffer

class CError: public ref::CBufT <TError, CGetErrorSize>
{
public:
	CError ()
	{
	}

	CError (const TError& src)
	{
		copy (src);
	}

	CError (
		ref::EBuf kind,
		void* p,
		size_t size
		)
	{
		setBuffer (kind, p, size);
	}

	CError (
		const rtl::TGuid& guid,
		uint_t code,
		ref::EBuf kind = ref::EBuf_Stack,
		TError* p = (TError*) _alloca (minBufSize),
		size_t size = minBufSize
		)
	{
		if (p)
			setBuffer (kind, p, size);

		createSimpleError (guid, code);
	}

	CError (
		uint_t code,
		ref::EBuf kind = ref::EBuf_Stack,
		TError* p = (TError*) _alloca (minBufSize),
		size_t size = minBufSize
		)
	{
		if (p)
			setBuffer (kind, p, size);

		createSystemError (code);
	}

	bool
	isKind (
		const rtl::TGuid& guid,
		uint_t code
		) const
	{
		return m_p && m_p->isKind (guid, code);
	}

	rtl::CString
	getDescription () const;

	TError*
	copy (const TError& src);

	TError*
	push (const TError& error);

	// pack

	template <typename TPack>
	TError*
	pack_va (
		const rtl::TGuid& guid,
		uint_t code,
		axl_va_list va
		)
	{
		size_t packSize;
		TPack () (NULL, &packSize, va);

		size_t size = sizeof (TError) + packSize;
		getBuffer (size);
		if (!m_p)
			return NULL;

		m_p->m_size = size;
		m_p->m_guid = guid;
		m_p->m_code = code;

		TPack () (m_p + 1, &packSize, va);
		return m_p;
	}

	template <typename TPack>
	TError*
	pack (
		const rtl::TGuid& guid,
		uint_t code,
		...
		)
	{
		AXL_VA_DECL (va, code);
		return pack_va <TPack> (guid, code, va);
	}

	template <typename TPack>
	TError*
	pushPack_va (
		const rtl::TGuid& guid,
		uint_t code,
		axl_va_list va
		)
	{
		if (!m_p)
			return pack_va <TPack> (guid, code, va);

		CError error;
		error.pack_va <TPack> (guid, code, va);
		return push (*error);
	}

	template <typename TPack>
	TError*
	pushPack (
		const rtl::TGuid& guid,
		uint_t code,
		...
		)
	{
		AXL_VA_DECL (va, code);
		return pushPack_va <TPack> (guid, code, va);
	}

	// format

	TError*
	format_va (
		const rtl::TGuid& guid,
		uint_t code,
		const char* formatString,
		axl_va_list va
		);

	TError*
	format (
		const rtl::TGuid& guid,
		uint_t code,
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return format_va (guid, code, formatString, va);
	}

	TError*
	pushFormat_va (
		const rtl::TGuid& guid,
		uint_t code,
		const char* formatString,
		axl_va_list va
		)
	{
		if (!m_p)
			return format_va (guid, code, formatString, va);

		CError error;
		error.format_va (guid, code, formatString, va);
		return push (*error);
	}

	TError*
	pushFormat (
		const rtl::TGuid& guid,
		uint_t code,
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return pushFormat_va (guid, code, formatString, va);
	}

	// simple error

	TError*
	createSimpleError (
		const rtl::TGuid& guid,
		uint_t code
		);

	TError*
	pushSimpleError (
		const rtl::TGuid& guid,
		uint_t code
		)
	{
		if (!m_p)
			return createSimpleError (guid, code);

		CError error;
		error.createSimpleError (guid, code);
		return push (*error);
	}

	// system error (push is irrelevant for system errors)

	TError*
	createSystemError (uint_t code)
	{
		return createSimpleError (GUID_SystemError, code);
	}

	// string error

	TError*
	createStringError (
		const char* p,
		size_t length = -1
		);

	TError*
	pushStringError (
		const char* p,
		size_t length = -1
		)
	{
		if (!m_p)
			return createStringError (p, length);

		CError error;
		error.createStringError (p, length);
		return push (*error);
	}

	TError*
	formatStringError_va (
		const char* formatString,
		axl_va_list va
		);

	TError*
	formatStringError (
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return formatStringError_va (formatString, va);
	}

	TError*
	pushFormatStringError_va (
		const char* formatString,
		axl_va_list va
		)
	{
		if (!m_p)
			return formatStringError_va (formatString, va);

		CError error;
		error.formatStringError_va (formatString, va);
		return push (*error);
	}


	TError*
	pushFormatStringError (
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return pushFormatStringError_va (formatString, va);
	}
};

//.............................................................................

// utility functions

CError
getError ();

CError
setError (const CError& error);

inline
CError
pushError (const CError& error)
{
	CError stack = getError ();
	stack.push (*error);
	return setError (stack);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// pack

template <typename TPack>
CError
setPackError_va (
	const rtl::TGuid& guid,
	uint_t code,
	axl_va_list va
	)
{
	CError error;
	error.pack_va <TPack> (guid, code, va);
	return setError (error);
}

template <typename TPack>
CError
setPackError (
	const rtl::TGuid& guid,
	uint_t code,
	...
	)
{
	AXL_VA_DECL (va, code);
	return setPackError_va <TPack> (guid, code, va);
}

template <typename TPack>
CError
pushPackError_va (
	const rtl::TGuid& guid,
	uint_t code,
	axl_va_list va
	)
{
	CError error;
	error.pack_va <TPack> (guid, code, va);
	return pushError (error);
}

template <typename TPack>
CError
pushPackError (
	const rtl::TGuid& guid,
	uint_t code,
	...
	)
{
	AXL_VA_DECL (va, code);
	return pushPackError_va <TPack> (guid, code, va);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// format

inline
CError
setFormatError_va (
	const rtl::TGuid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	)
{
	CError error;
	error.format_va (guid, code, formatString, va);
	return setError (error);
}

inline
CError
setFormatError (
	const rtl::TGuid& guid,
	uint_t code,
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return setFormatError_va (guid, code, formatString, va);
}

inline
CError
pushFormatError_va (
	const rtl::TGuid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	)
{
	CError error;
	error.format_va (guid, code, formatString, va);
	return pushError (error);
}

inline
CError
pushFormatError (
	const rtl::TGuid& guid,
	uint_t code,
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return pushFormatError_va (guid, code, formatString, va);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


// simple error

inline
CError
setError (
	const rtl::TGuid& guid,
	uint_t code
	)
{
	return setError (CError (guid, code));
}

inline
CError
pushError (
	const rtl::TGuid& guid,
	uint_t code
	)
{
	return pushError (CError (guid, code));
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// string error

inline
CError
createStringError (
	const char* p,
	size_t length = -1
	)
{
	CError error;
	error.createStringError (p, length);
	return error;
}

inline
CError
setStringError (
	const char* p,
	size_t length = -1
	)
{
	return setError (createStringError (p, length));
}

inline
CError
pushStringError (
	const char* p,
	size_t length = -1
	)
{
	CError error;
	error.createStringError (p, -1);
	return setError (error);
}

inline
CError
formatStringError_va (
	const char* formatString,
	axl_va_list va
	)
{
	CError error;
	error.formatStringError_va (formatString, va);
	return error;
}

inline
CError
formatStringError (
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return formatStringError_va (formatString, va);
}

inline
CError
setFormatStringError_va (
	const char* formatString,
	axl_va_list va
	)
{
	return setError (formatStringError_va (formatString, va));
}

inline
CError
setFormatStringError (
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return setFormatStringError_va (formatString, va);
}

inline
CError
pushFormatStringError_va (
	const char* formatString,
	axl_va_list va
	)
{
	CError error;
	error.formatStringError_va (formatString, va);
	return pushError (error);
}

inline
CError
pushFormatStringError (
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return pushFormatStringError_va (formatString, va);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// convenient helpers

template <typename T>
T
fail (
	T failResult,
	CError error
	)
{
	setError (error);
	return failResult;
}

inline
bool
fail (CError error)
{
	return fail <bool> (false, error);
}

inline
CError
setLastSystemError ()
{
	return setError (getLastSystemErrorCode ());
}

template <typename T>
T
failWithLastSystemError (T failResult)
{
	return fail (failResult, getLastSystemErrorCode ());
}

inline
bool
failWithLastSystemError ()
{
	return failWithLastSystemError <bool> (false);
}

template <typename T>
T
completeWithSystemError (
	T result,
	T failResult,
	uint_t errorCode
	)
{
	if (result == failResult)
		setError (errorCode);

	return result;
}

template <typename T>
T
complete (
	T result,
	T failResult
	)
{
	if (result == failResult)
		setLastSystemError ();

	return result;
}

inline
bool
complete (int result)
{
	return complete <bool> (result != 0, false);
}

//.............................................................................

// providers

class CErrorProvider
{
public:
	virtual
	rtl::CString
	getErrorDescription (const TError* error) = 0;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CStdErrorProvider: public CErrorProvider
{
public:
	virtual
	rtl::CString
	getErrorDescription (const TError* error);

protected:
	rtl::CString
	getStackErrorDescription (const TError* error);
};

//.............................................................................

} // namespace err
} // namespace axl

