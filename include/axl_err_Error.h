// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_ERR_ERROR_H

#include "axl_ref_Buf.h"
#include "axl_rtl_Guid.h"

namespace axl {
namespace rtl {

template <typename T> class StringBase;
typedef StringBase <char> String;

} // namespace rtl
} // namespace axl

namespace axl {
namespace err {

//.............................................................................

enum ErrorModeKind
{
	ErrorModeKind_NoThrow       = 0,
	ErrorModeKind_CppException  = 1,

	// these 2 are almost useless unless in C-style code without classes

	ErrorModeKind_SehException  = 2,
	ErrorModeKind_SetJmpLongJmp = 4,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ErrorModeKind
getErrorMode ();

ErrorModeKind // returns previous one
setErrorMode (ErrorModeKind mode);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ScopeErrorMode
{
protected:
	ErrorModeKind m_oldMode;

public:
	ScopeErrorMode (ErrorModeKind mode)
	{
		m_oldMode = setErrorMode (mode);
	}

	~ScopeErrorMode ()
	{
		setErrorMode (m_oldMode);
	}
};

//.............................................................................

// axl std errors

extern
AXL_SELECT_ANY
const rtl::Guid GUID_StdError = rtl::GUID_Null;

enum StdErrorKind
{
	StdErrorKind_NoError,
	StdErrorKind_String,
	StdErrorKind_Stack,
};

//.............................................................................

// system errors

#if (_AXL_ENV == AXL_ENV_WIN)

extern const rtl::Guid GUID_WinError;

#define GUID_SystemError GUID_WinError

enum StatusKind
{
	StatusKind_Success                  = ERROR_SUCCESS,
	StatusKind_Pending                  = ERROR_IO_PENDING,
	StatusKind_Cancelled                = ERROR_OPERATION_ABORTED,
	StatusKind_Unsuccessful             = ERROR_GEN_FAILURE,
	StatusKind_InsufficientResources    = ERROR_NO_SYSTEM_RESOURCES,
	StatusKind_NotImplemented           = ERROR_INVALID_FUNCTION,
	StatusKind_InvalidHandle            = ERROR_INVALID_HANDLE,
	StatusKind_AddressAlreadyExists     = ERROR_DUP_NAME,
	StatusKind_InvalidAddressComponent  = ERROR_INVALID_NETNAME,
	StatusKind_TooManyAddresses         = ERROR_TOO_MANY_NAMES,
	StatusKind_InvalidAddress           = ERROR_UNEXP_NET_ERR,
	StatusKind_AddressClosed            = ERROR_NETNAME_DELETED,
	StatusKind_BufferOverflow           = ERROR_MORE_DATA,
	StatusKind_InvalidParameter         = ERROR_INVALID_PARAMETER,
	StatusKind_ConnectionRefused        = ERROR_CONNECTION_REFUSED,
	StatusKind_ConnectionInvalid        = ERROR_CONNECTION_INVALID,
	StatusKind_AddressAlreadyAssociated = ERROR_ADDRESS_ALREADY_ASSOCIATED,
	StatusKind_AddressNotAssociated     = ERROR_ADDRESS_NOT_ASSOCIATED,
	StatusKind_ConnectionActive         = ERROR_CONNECTION_ACTIVE,
	StatusKind_ConnectionAborted        = ERROR_CONNECTION_ABORTED,
	StatusKind_ConnectionReset          = ERROR_NETNAME_DELETED,
	StatusKind_IoTimeout                = ERROR_SEM_TIMEOUT,
	StatusKind_GracefulDisconnect       = ERROR_GRACEFUL_DISCONNECT,
	StatusKind_DataNotAccepted          = ERROR_INVALID_DATA,
	StatusKind_MoreProcessingRequired   = ERROR_MORE_DATA,
	StatusKind_InvalidDeviceState       = ERROR_BAD_COMMAND,
	StatusKind_NetworkUnreachable       = ERROR_NETWORK_UNREACHABLE,
	StatusKind_HostUnreachable          = ERROR_HOST_UNREACHABLE,
	StatusKind_ProtocolUnreachable      = ERROR_PROTOCOL_UNREACHABLE,
	StatusKind_PortUnreachable          = ERROR_PORT_UNREACHABLE,
	StatusKind_InvalidDeviceRequest     = ERROR_INVALID_FUNCTION,
	StatusKind_RequestAborted           = ERROR_REQUEST_ABORTED,
	StatusKind_BufferTooSmall           = ERROR_INSUFFICIENT_BUFFER,
	StatusKind_InvalidBufferSize        = ERROR_INVALID_USER_BUFFER,
	StatusKind_ObjectNameNotFound       = ERROR_FILE_NOT_FOUND,
	StatusKind_AccessDenied             = ERROR_ACCESS_DENIED,
	StatusKind_SharingViolation         = ERROR_SHARING_VIOLATION,
	StatusKind_NoMoreEntries            = ERROR_NO_MORE_ITEMS,
};

inline
uint_t
getLastSystemErrorCode ()
{
	return ::GetLastError ();
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#elif (_AXL_ENV == AXL_ENV_NT)

extern const rtl::Guid GUID_NtError;

#define GUID_SystemError GUID_NtError

enum StatusKind
{
	StatusKind_Success                  = STATUS_SUCCESS,
	StatusKind_Pending                  = STATUS_PENDING,
	StatusKind_Cancelled                = STATUS_CANCELLED,
	StatusKind_Unsuccessful             = STATUS_UNSUCCESSFUL,
	StatusKind_InsufficientResources    = STATUS_INSUFFICIENT_RESOURCES,
	StatusKind_NotImplemented           = STATUS_NOT_IMPLEMENTED,
	StatusKind_InvalidHandle            = STATUS_INVALID_HANDLE,
	StatusKind_AddressAlreadyExists     = STATUS_ADDRESS_ALREADY_EXISTS,
	StatusKind_InvalidAddressComponent  = STATUS_INVALID_ADDRESS_COMPONENT,
	StatusKind_TooManyAddresses         = STATUS_TOO_MANY_ADDRESSES,
	StatusKind_InvalidAddress           = STATUS_INVALID_ADDRESS,
	StatusKind_AddressClosed            = STATUS_ADDRESS_CLOSED,
	StatusKind_BufferOverflow           = STATUS_BUFFER_OVERFLOW,
	StatusKind_InvalidParameter         = STATUS_INVALID_PARAMETER,
	StatusKind_ConnectionRefused        = STATUS_CONNECTION_REFUSED,
	StatusKind_ConnectionInvalid        = STATUS_CONNECTION_INVALID,
	StatusKind_AddressAlreadyAssociated = STATUS_ADDRESS_ALREADY_ASSOCIATED,
	StatusKind_AddressNotAssociated     = STATUS_ADDRESS_NOT_ASSOCIATED,
	StatusKind_ConnectionActive         = STATUS_CONNECTION_ACTIVE,
	StatusKind_ConnectionAborted        = STATUS_CONNECTION_ABORTED,
	StatusKind_ConnectionReset          = STATUS_CONNECTION_RESET,
	StatusKind_IoTimeout                = STATUS_IO_TIMEOUT,
	StatusKind_GracefulDisconnect       = STATUS_GRACEFUL_DISCONNECT,
	StatusKind_DataNotAccepted          = STATUS_DATA_NOT_ACCEPTED,
	StatusKind_MoreProcessingRequired   = STATUS_MORE_PROCESSING_REQUIRED,
	StatusKind_InvalidDeviceState       = STATUS_INVALID_DEVICE_STATE,
	StatusKind_NetworkUnreachable       = STATUS_NETWORK_UNREACHABLE,
	StatusKind_HostUnreachable          = STATUS_HOST_UNREACHABLE,
	StatusKind_ProtocolUnreachable      = STATUS_PROTOCOL_UNREACHABLE,
	StatusKind_PortUnreachable          = STATUS_PORT_UNREACHABLE,
	StatusKind_InvalidDeviceRequest     = STATUS_INVALID_DEVICE_REQUEST,
	StatusKind_RequestAborted           = STATUS_REQUEST_ABORTED,
	StatusKind_BufferTooSmall           = STATUS_BUFFER_TOO_SMALL,
	StatusKind_InvalidBufferSize        = STATUS_INVALID_BUFFER_SIZE,
	StatusKind_ObjectNameNotFound       = STATUS_OBJECT_NAME_NOT_FOUND,
	StatusKind_AccessDenied             = STATUS_ACCESS_DENIED,
	StatusKind_SharingViolation         = STATUS_SHARING_VIOLATION,
	StatusKind_NoMoreEntries            = STATUS_NO_MORE_ENTRIES,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#elif (_AXL_ENV == AXL_ENV_POSIX)

extern const rtl::Guid GUID_Errno;

#define GUID_SystemError GUID_Errno

enum StatusKind
{
	StatusKind_Success                  = 0,
	StatusKind_Cancelled                = EINTR,
	StatusKind_Unsuccessful             = EIO,
	StatusKind_InsufficientResources    = ENOMEM,
	StatusKind_NotImplemented           = ENOSYS,
	StatusKind_InvalidHandle            = EBADF,
	StatusKind_AddressAlreadyExists     = EEXIST,
/*	StatusKind_InvalidAddressComponent  = ERROR_INVALID_NETNAME,
	StatusKind_TooManyAddresses         = ERROR_TOO_MANY_NAMES,
	StatusKind_InvalidAddress           = ERROR_UNEXP_NET_ERR,
	StatusKind_AddressClosed            = ERROR_NETNAME_DELETED,
	StatusKind_BufferOverflow           = ERROR_MORE_DATA,*/
	StatusKind_InvalidParameter         = EINVAL,
/*	StatusKind_ConnectionRefused        = ERROR_CONNECTION_REFUSED,
	StatusKind_ConnectionInvalid        = ERROR_CONNECTION_INVALID,
	StatusKind_AddressAlreadyAssociated = ERROR_ADDRESS_ALREADY_ASSOCIATED,
	StatusKind_AddressNotAssociated     = ERROR_ADDRESS_NOT_ASSOCIATED,
	StatusKind_ConnectionActive         = ERROR_CONNECTION_ACTIVE,
	StatusKind_ConnectionAborted        = ERROR_CONNECTION_ABORTED,
	StatusKind_ConnectionReset          = ERROR_NETNAME_DELETED,
	StatusKind_IoTimeout                = ERROR_SEM_TIMEOUT,
	StatusKind_GracefulDisconnect       = ERROR_GRACEFUL_DISCONNECT,
	StatusKind_DataNotAccepted          = ERROR_INVALID_DATA,
	StatusKind_MoreProcessingRequired   = ERROR_MORE_DATA, */
	StatusKind_InvalidDeviceState       = EBUSY,
/*	StatusKind_NetworkUnreachable       = ERROR_NETWORK_UNREACHABLE,
	StatusKind_HostUnreachable          = ERROR_HOST_UNREACHABLE,
	StatusKind_ProtocolUnreachable      = ERROR_PROTOCOL_UNREACHABLE,
	StatusKind_PortUnreachable          = ERROR_PORT_UNREACHABLE,*/
	StatusKind_InvalidDeviceRequest     = ENOSYS,
/*	StatusKind_RequestAborted           = ERROR_REQUEST_ABORTED,
	StatusKind_BufferTooSmall           = ERROR_INSUFFICIENT_BUFFER,
	StatusKind_InvalidBufferSize        = ERROR_INVALID_USER_BUFFER,
	StatusKind_ObjectNameNotFound       = ERROR_FILE_NOT_FOUND,
	StatusKind_AccessDenied             = ERROR_ACCESS_DENIED,
	StatusKind_SharingViolation         = ERROR_SHARING_VIOLATION,
	StatusKind_NoMoreEntries            = ERROR_NO_MORE_ITEMS, */
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

struct ErrorData
{
	class GetSize
	{
	public:
		size_t
		operator () (const ErrorData& x)
		{
			ASSERT (x.m_size >= sizeof (ErrorData));
			return AXL_MAX (x.m_size, sizeof (ErrorData));
		}
	};

	uint32_t m_size;
	rtl::Guid m_guid;
	uint32_t m_code;

	// possibly followed by error data

	rtl::String
	getDescription () const;

	bool
	isKind (
		const rtl::Guid& guid,
		uint_t code
		) const;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

extern
AXL_SELECT_ANY
const ErrorData g_noError =
{
	sizeof (ErrorData),
	GUID_StdError,
	StatusKind_Success,
};

//.............................................................................

// ref-counted error buffer

class Error: public ref::Buf <ErrorData, ErrorData::GetSize>
{
public:
	Error ()
	{
	}

	Error (const ErrorData& src)
	{
		copy (src);
	}

	Error (
		ref::BufKind kind,
		void* p,
		size_t size
		)
	{
		setBuffer (kind, p, size);
	}

	Error (
		const rtl::Guid& guid,
		uint_t code,
		ref::BufKind kind = ref::BufKind_Stack,
		ErrorData* p = (ErrorData*) _alloca (MinBufSize),
		size_t size = MinBufSize
		)
	{
		if (p)
			setBuffer (kind, p, size);

		createSimpleError (guid, code);
	}

	Error (
		uint_t code,
		ref::BufKind kind = ref::BufKind_Stack,
		ErrorData* p = (ErrorData*) _alloca (MinBufSize),
		size_t size = MinBufSize
		)
	{
		if (p)
			setBuffer (kind, p, size);

		createSystemError (code);
	}

	bool
	isKind (
		const rtl::Guid& guid,
		uint_t code
		) const
	{
		return m_p && m_p->isKind (guid, code);
	}

	rtl::String
	getDescription () const;

	ErrorData*
	copy (const ErrorData& src);

	ErrorData*
	push (const ErrorData& error);

	// pack

	template <typename Pack>
	ErrorData*
	pack_va (
		const rtl::Guid& guid,
		uint_t code,
		axl_va_list va
		)
	{
		size_t packSize;
		Pack () (NULL, &packSize, va);

		size_t size = sizeof (ErrorData) + packSize;
		getBuffer (size);
		if (!m_p)
			return NULL;

		m_p->m_size = size;
		m_p->m_guid = guid;
		m_p->m_code = code;

		Pack () (m_p + 1, &packSize, va);
		return m_p;
	}

	template <typename Pack>
	ErrorData*
	pack (
		const rtl::Guid& guid,
		uint_t code,
		...
		)
	{
		AXL_VA_DECL (va, code);
		return pack_va <Pack> (guid, code, va);
	}

	template <typename Pack>
	ErrorData*
	pushPack_va (
		const rtl::Guid& guid,
		uint_t code,
		axl_va_list va
		)
	{
		if (!m_p)
			return pack_va <Pack> (guid, code, va);

		Error error;
		error.pack_va <Pack> (guid, code, va);
		return push (*error);
	}

	template <typename Pack>
	ErrorData*
	pushPack (
		const rtl::Guid& guid,
		uint_t code,
		...
		)
	{
		AXL_VA_DECL (va, code);
		return pushPack_va <Pack> (guid, code, va);
	}

	// format

	ErrorData*
	format_va (
		const rtl::Guid& guid,
		uint_t code,
		const char* formatString,
		axl_va_list va
		);

	ErrorData*
	format (
		const rtl::Guid& guid,
		uint_t code,
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return format_va (guid, code, formatString, va);
	}

	ErrorData*
	pushFormat_va (
		const rtl::Guid& guid,
		uint_t code,
		const char* formatString,
		axl_va_list va
		)
	{
		if (!m_p)
			return format_va (guid, code, formatString, va);

		Error error;
		error.format_va (guid, code, formatString, va);
		return push (*error);
	}

	ErrorData*
	pushFormat (
		const rtl::Guid& guid,
		uint_t code,
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return pushFormat_va (guid, code, formatString, va);
	}

	// simple error

	ErrorData*
	createSimpleError (
		const rtl::Guid& guid,
		uint_t code
		);

	ErrorData*
	pushSimpleError (
		const rtl::Guid& guid,
		uint_t code
		)
	{
		if (!m_p)
			return createSimpleError (guid, code);

		Error error;
		error.createSimpleError (guid, code);
		return push (*error);
	}

	// system error (push is irrelevant for system errors)

	ErrorData*
	createSystemError (uint_t code)
	{
		return createSimpleError (GUID_SystemError, code);
	}

	// string error

	ErrorData*
	createStringError (
		const char* p,
		size_t length = -1
		);

	ErrorData*
	pushStringError (
		const char* p,
		size_t length = -1
		)
	{
		if (!m_p)
			return createStringError (p, length);

		Error error;
		error.createStringError (p, length);
		return push (*error);
	}

	ErrorData*
	formatStringError_va (
		const char* formatString,
		axl_va_list va
		);

	ErrorData*
	formatStringError (
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return formatStringError_va (formatString, va);
	}

	ErrorData*
	pushFormatStringError_va (
		const char* formatString,
		axl_va_list va
		)
	{
		if (!m_p)
			return formatStringError_va (formatString, va);

		Error error;
		error.formatStringError_va (formatString, va);
		return push (*error);
	}


	ErrorData*
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

Error
getError ();

Error
setError (const Error& error);

inline
Error
pushError (const Error& error)
{
	Error stack = getError ();
	stack.push (*error);
	return setError (stack);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// pack

template <typename Pack>
Error
setPackError_va (
	const rtl::Guid& guid,
	uint_t code,
	axl_va_list va
	)
{
	Error error;
	error.pack_va <Pack> (guid, code, va);
	return setError (error);
}

template <typename Pack>
Error
setPackError (
	const rtl::Guid& guid,
	uint_t code,
	...
	)
{
	AXL_VA_DECL (va, code);
	return setPackError_va <Pack> (guid, code, va);
}

template <typename Pack>
Error
pushPackError_va (
	const rtl::Guid& guid,
	uint_t code,
	axl_va_list va
	)
{
	Error error;
	error.pack_va <Pack> (guid, code, va);
	return pushError (error);
}

template <typename Pack>
Error
pushPackError (
	const rtl::Guid& guid,
	uint_t code,
	...
	)
{
	AXL_VA_DECL (va, code);
	return pushPackError_va <Pack> (guid, code, va);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// format

inline
Error
setFormatError_va (
	const rtl::Guid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	)
{
	Error error;
	error.format_va (guid, code, formatString, va);
	return setError (error);
}

inline
Error
setFormatError (
	const rtl::Guid& guid,
	uint_t code,
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return setFormatError_va (guid, code, formatString, va);
}

inline
Error
pushFormatError_va (
	const rtl::Guid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	)
{
	Error error;
	error.format_va (guid, code, formatString, va);
	return pushError (error);
}

inline
Error
pushFormatError (
	const rtl::Guid& guid,
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
Error
setError (
	const rtl::Guid& guid,
	uint_t code
	)
{
	return setError (Error (guid, code));
}

inline
Error
pushError (
	const rtl::Guid& guid,
	uint_t code
	)
{
	return pushError (Error (guid, code));
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// string error

inline
Error
createStringError (
	const char* p,
	size_t length = -1
	)
{
	Error error;
	error.createStringError (p, length);
	return error;
}

inline
Error
setStringError (
	const char* p,
	size_t length = -1
	)
{
	return setError (createStringError (p, length));
}

inline
Error
pushStringError (
	const char* p,
	size_t length = -1
	)
{
	Error error;
	error.createStringError (p, -1);
	return setError (error);
}

inline
Error
formatStringError_va (
	const char* formatString,
	axl_va_list va
	)
{
	Error error;
	error.formatStringError_va (formatString, va);
	return error;
}

inline
Error
formatStringError (
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return formatStringError_va (formatString, va);
}

inline
Error
setFormatStringError_va (
	const char* formatString,
	axl_va_list va
	)
{
	return setError (formatStringError_va (formatString, va));
}

inline
Error
setFormatStringError (
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return setFormatStringError_va (formatString, va);
}

inline
Error
pushFormatStringError_va (
	const char* formatString,
	axl_va_list va
	)
{
	Error error;
	error.formatStringError_va (formatString, va);
	return pushError (error);
}

inline
Error
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
	Error error
	)
{
	setError (error);
	return failResult;
}

inline
bool
fail (Error error)
{
	return fail <bool> (false, error);
}

inline
Error
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

class ErrorProvider
{
public:
	virtual
	rtl::String
	getErrorDescription (const ErrorData* error) = 0;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class StdErrorProvider: public ErrorProvider
{
public:
	virtual
	rtl::String
	getErrorDescription (const ErrorData* error);

protected:
	rtl::String
	getStackErrorDescription (const ErrorData* error);
};

//.............................................................................

} // namespace err
} // namespace axl

