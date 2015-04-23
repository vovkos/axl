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

enum ErrorMode
{
	ErrorMode_NoThrow       = 0,
	ErrorMode_CppException  = 1,

	// these 2 are almost useless unless in C-style code without classes

	ErrorMode_SehException  = 2,
	ErrorMode_SetJmpLongJmp = 4,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ErrorMode
getErrorMode ();

ErrorMode // returns previous one
setErrorMode (ErrorMode mode);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ScopeErrorMode
{
protected:
	ErrorMode m_oldMode;

public:
	ScopeErrorMode (ErrorMode mode)
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

enum StdErrorCode
{
	StdErrorCode_NoError,
	StdErrorCode_String,
	StdErrorCode_Stack,
};

//.............................................................................

// system errors

#if (_AXL_ENV == AXL_ENV_WIN)

extern const rtl::Guid GUID_WinError;

#define GUID_SystemError GUID_WinError

enum SystemErrorCode
{
	SystemErrorCode_Success                  = ERROR_SUCCESS,
	SystemErrorCode_Pending                  = ERROR_IO_PENDING,
	SystemErrorCode_Cancelled                = ERROR_OPERATION_ABORTED,
	SystemErrorCode_Unsuccessful             = ERROR_GEN_FAILURE,
	SystemErrorCode_InsufficientResources    = ERROR_NO_SYSTEM_RESOURCES,
	SystemErrorCode_NotImplemented           = ERROR_INVALID_FUNCTION,
	SystemErrorCode_InvalidHandle            = ERROR_INVALID_HANDLE,
	SystemErrorCode_AddressAlreadyExists     = ERROR_DUP_NAME,
	SystemErrorCode_InvalidAddressComponent  = ERROR_INVALID_NETNAME,
	SystemErrorCode_TooManyAddresses         = ERROR_TOO_MANY_NAMES,
	SystemErrorCode_InvalidAddress           = ERROR_UNEXP_NET_ERR,
	SystemErrorCode_AddressClosed            = ERROR_NETNAME_DELETED,
	SystemErrorCode_BufferOverflow           = ERROR_MORE_DATA,
	SystemErrorCode_InvalidParameter         = ERROR_INVALID_PARAMETER,
	SystemErrorCode_ConnectionRefused        = ERROR_CONNECTION_REFUSED,
	SystemErrorCode_ConnectionInvalid        = ERROR_CONNECTION_INVALID,
	SystemErrorCode_AddressAlreadyAssociated = ERROR_ADDRESS_ALREADY_ASSOCIATED,
	SystemErrorCode_AddressNotAssociated     = ERROR_ADDRESS_NOT_ASSOCIATED,
	SystemErrorCode_ConnectionActive         = ERROR_CONNECTION_ACTIVE,
	SystemErrorCode_ConnectionAborted        = ERROR_CONNECTION_ABORTED,
	SystemErrorCode_ConnectionReset          = ERROR_NETNAME_DELETED,
	SystemErrorCode_IoTimeout                = ERROR_SEM_TIMEOUT,
	SystemErrorCode_GracefulDisconnect       = ERROR_GRACEFUL_DISCONNECT,
	SystemErrorCode_DataNotAccepted          = ERROR_INVALID_DATA,
	SystemErrorCode_MoreProcessingRequired   = ERROR_MORE_DATA,
	SystemErrorCode_InvalidDeviceState       = ERROR_BAD_COMMAND,
	SystemErrorCode_NetworkUnreachable       = ERROR_NETWORK_UNREACHABLE,
	SystemErrorCode_HostUnreachable          = ERROR_HOST_UNREACHABLE,
	SystemErrorCode_ProtocolUnreachable      = ERROR_PROTOCOL_UNREACHABLE,
	SystemErrorCode_PortUnreachable          = ERROR_PORT_UNREACHABLE,
	SystemErrorCode_InvalidDeviceRequest     = ERROR_INVALID_FUNCTION,
	SystemErrorCode_RequestAborted           = ERROR_REQUEST_ABORTED,
	SystemErrorCode_BufferTooSmall           = ERROR_INSUFFICIENT_BUFFER,
	SystemErrorCode_InvalidBufferSize        = ERROR_INVALID_USER_BUFFER,
	SystemErrorCode_ObjectNameNotFound       = ERROR_FILE_NOT_FOUND,
	SystemErrorCode_AccessDenied             = ERROR_ACCESS_DENIED,
	SystemErrorCode_SharingViolation         = ERROR_SHARING_VIOLATION,
	SystemErrorCode_NoMoreEntries            = ERROR_NO_MORE_ITEMS,
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

enum SystemErrorCode
{
	SystemErrorCode_Success                  = STATUS_SUCCESS,
	SystemErrorCode_Pending                  = STATUS_PENDING,
	SystemErrorCode_Cancelled                = STATUS_CANCELLED,
	SystemErrorCode_Unsuccessful             = STATUS_UNSUCCESSFUL,
	SystemErrorCode_InsufficientResources    = STATUS_INSUFFICIENT_RESOURCES,
	SystemErrorCode_NotImplemented           = STATUS_NOT_IMPLEMENTED,
	SystemErrorCode_InvalidHandle            = STATUS_INVALID_HANDLE,
	SystemErrorCode_AddressAlreadyExists     = STATUS_ADDRESS_ALREADY_EXISTS,
	SystemErrorCode_InvalidAddressComponent  = STATUS_INVALID_ADDRESS_COMPONENT,
	SystemErrorCode_TooManyAddresses         = STATUS_TOO_MANY_ADDRESSES,
	SystemErrorCode_InvalidAddress           = STATUS_INVALID_ADDRESS,
	SystemErrorCode_AddressClosed            = STATUS_ADDRESS_CLOSED,
	SystemErrorCode_BufferOverflow           = STATUS_BUFFER_OVERFLOW,
	SystemErrorCode_InvalidParameter         = STATUS_INVALID_PARAMETER,
	SystemErrorCode_ConnectionRefused        = STATUS_CONNECTION_REFUSED,
	SystemErrorCode_ConnectionInvalid        = STATUS_CONNECTION_INVALID,
	SystemErrorCode_AddressAlreadyAssociated = STATUS_ADDRESS_ALREADY_ASSOCIATED,
	SystemErrorCode_AddressNotAssociated     = STATUS_ADDRESS_NOT_ASSOCIATED,
	SystemErrorCode_ConnectionActive         = STATUS_CONNECTION_ACTIVE,
	SystemErrorCode_ConnectionAborted        = STATUS_CONNECTION_ABORTED,
	SystemErrorCode_ConnectionReset          = STATUS_CONNECTION_RESET,
	SystemErrorCode_IoTimeout                = STATUS_IO_TIMEOUT,
	SystemErrorCode_GracefulDisconnect       = STATUS_GRACEFUL_DISCONNECT,
	SystemErrorCode_DataNotAccepted          = STATUS_DATA_NOT_ACCEPTED,
	SystemErrorCode_MoreProcessingRequired   = STATUS_MORE_PROCESSING_REQUIRED,
	SystemErrorCode_InvalidDeviceState       = STATUS_INVALID_DEVICE_STATE,
	SystemErrorCode_NetworkUnreachable       = STATUS_NETWORK_UNREACHABLE,
	SystemErrorCode_HostUnreachable          = STATUS_HOST_UNREACHABLE,
	SystemErrorCode_ProtocolUnreachable      = STATUS_PROTOCOL_UNREACHABLE,
	SystemErrorCode_PortUnreachable          = STATUS_PORT_UNREACHABLE,
	SystemErrorCode_InvalidDeviceRequest     = STATUS_INVALID_DEVICE_REQUEST,
	SystemErrorCode_RequestAborted           = STATUS_REQUEST_ABORTED,
	SystemErrorCode_BufferTooSmall           = STATUS_BUFFER_TOO_SMALL,
	SystemErrorCode_InvalidBufferSize        = STATUS_INVALID_BUFFER_SIZE,
	SystemErrorCode_ObjectNameNotFound       = STATUS_OBJECT_NAME_NOT_FOUND,
	SystemErrorCode_AccessDenied             = STATUS_ACCESS_DENIED,
	SystemErrorCode_SharingViolation         = STATUS_SHARING_VIOLATION,
	SystemErrorCode_NoMoreEntries            = STATUS_NO_MORE_ENTRIES,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#elif (_AXL_ENV == AXL_ENV_POSIX)

extern const rtl::Guid GUID_Errno;

#define GUID_SystemError GUID_Errno

enum SystemErrorCode
{
	SystemErrorCode_Success                  = 0,
	SystemErrorCode_Cancelled                = EINTR,
	SystemErrorCode_Unsuccessful             = EIO,
	SystemErrorCode_InsufficientResources    = ENOMEM,
	SystemErrorCode_NotImplemented           = ENOSYS,
	SystemErrorCode_InvalidHandle            = EBADF,
	SystemErrorCode_AddressAlreadyExists     = EEXIST,
/*	SystemErrorCode_InvalidAddressComponent  = ERROR_INVALID_NETNAME,
	SystemErrorCode_TooManyAddresses         = ERROR_TOO_MANY_NAMES, */
	SystemErrorCode_InvalidAddress           = EINVAL,
/*	SystemErrorCode_AddressClosed            = ERROR_NETNAME_DELETED,
	SystemErrorCode_BufferOverflow           = ERROR_MORE_DATA,*/
	SystemErrorCode_InvalidParameter         = EINVAL,
/*	SystemErrorCode_ConnectionRefused        = ERROR_CONNECTION_REFUSED,
	SystemErrorCode_ConnectionInvalid        = ERROR_CONNECTION_INVALID,
	SystemErrorCode_AddressAlreadyAssociated = ERROR_ADDRESS_ALREADY_ASSOCIATED,
	SystemErrorCode_AddressNotAssociated     = ERROR_ADDRESS_NOT_ASSOCIATED,
	SystemErrorCode_ConnectionActive         = ERROR_CONNECTION_ACTIVE,
	SystemErrorCode_ConnectionAborted        = ERROR_CONNECTION_ABORTED,
	SystemErrorCode_ConnectionReset          = ERROR_NETNAME_DELETED,
	SystemErrorCode_IoTimeout                = ERROR_SEM_TIMEOUT,
	SystemErrorCode_GracefulDisconnect       = ERROR_GRACEFUL_DISCONNECT,
	SystemErrorCode_DataNotAccepted          = ERROR_INVALID_DATA,
	SystemErrorCode_MoreProcessingRequired   = ERROR_MORE_DATA, */
	SystemErrorCode_InvalidDeviceState       = EBUSY,
/*	SystemErrorCode_NetworkUnreachable       = ERROR_NETWORK_UNREACHABLE,
	SystemErrorCode_HostUnreachable          = ERROR_HOST_UNREACHABLE,
	SystemErrorCode_ProtocolUnreachable      = ERROR_PROTOCOL_UNREACHABLE,
	SystemErrorCode_PortUnreachable          = ERROR_PORT_UNREACHABLE,*/
	SystemErrorCode_InvalidDeviceRequest     = ENOSYS,
//	SystemErrorCode_RequestAborted           = ERROR_REQUEST_ABORTED,
	SystemErrorCode_BufferTooSmall           = ENOBUFS,
/*	SystemErrorCode_InvalidBufferSize        = ERROR_INVALID_USER_BUFFER,
	SystemErrorCode_ObjectNameNotFound       = ERROR_FILE_NOT_FOUND,
	SystemErrorCode_AccessDenied             = ERROR_ACCESS_DENIED,
	SystemErrorCode_SharingViolation         = ERROR_SHARING_VIOLATION,
	SystemErrorCode_NoMoreEntries            = ERROR_NO_MORE_ITEMS, */
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
	SystemErrorCode_Success,
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

	Error (
		const char* string,
		size_t length = -1
		)
	{
		createStringError (string, length);
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
getLastError ();

rtl::String
getLastErrorDescription ();

Error
setError (const Error& error);

inline
Error
pushError (const Error& error)
{
	Error stack = getLastError ();
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

