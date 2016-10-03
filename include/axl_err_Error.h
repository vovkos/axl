// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_ERR_ERROR_H

#include "axl_ref_Buf.h"
#include "axl_sl_Guid.h"

namespace axl {
namespace sl {

template <typename T> class StringRefBase;
template <typename T> class StringBase;

typedef StringRefBase <char> StringRef;
typedef StringBase <char> String;

} // namespace sl
} // namespace axl

namespace axl {
namespace err {

//.............................................................................

// axl std errors

extern AXL_SELECT_ANY const sl::Guid g_stdErrorGuid = sl::g_nullGuid;

enum StdErrorCode
{
	StdErrorCode_NoError,
	StdErrorCode_String,
	StdErrorCode_Stack,
};

//.............................................................................

// system errors

#if (_AXL_OS_WIN)

extern const sl::Guid g_winErrorGuid;

#define g_systemErrorGuid g_winErrorGuid

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

#elif (_AXL_OS_POSIX)

extern const sl::Guid g_ErrnoGuid;

#define g_systemErrorGuid g_ErrnoGuid

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
//	SystemErrorCode_InvalidBufferSize        = ERROR_INVALID_USER_BUFFER,
	SystemErrorCode_ObjectNameNotFound       = ENOENT,
/*	SystemErrorCode_AccessDenied             = ERROR_ACCESS_DENIED,
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

struct ErrorHdr
{
	uint32_t m_size;
	sl::Guid m_guid;
	uint32_t m_code;

	bool
	isKindOf (
		const sl::Guid& guid,
		uint_t code
		) const
	{
		return m_guid == guid && m_code == code;
	}

	bool
	isStackTopKindOf (
		const sl::Guid& guid,
		uint_t code
		) const
	{
		return 
			isKindOf (g_stdErrorGuid, StdErrorCode_Stack) &&
			(this + 1)->isKindOf (guid, code);
	}

	sl::String
	getDescription () const;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class SizeOfError
{
public:
	size_t
	operator () (const ErrorHdr* error)
	{
		return AXL_MAX (error->m_size, sizeof (ErrorHdr));
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

extern AXL_SELECT_ANY const ErrorHdr g_noError =
{
	sizeof (ErrorHdr),
	g_stdErrorGuid,
	SystemErrorCode_Success,
};

//.............................................................................

class ErrorRef: public ref::BufRef <ErrorHdr, SizeOfError>
{
public:
	typedef ref::BufRef <ErrorHdr, SizeOfError> BaseType;

public:
	ErrorRef ()
	{
	}

	ErrorRef (const ErrorRef& src):
		BaseType (src)
	{
	}

	ErrorRef (const ErrorHdr* src):
		BaseType (src)
	{
	}

	ErrorRef (uint_t code);

	ErrorRef (
		const sl::Guid& guid,
		uint_t code
		);

	ErrorRef (
		const char* string,
		size_t length = -1
		);

	ErrorRef&
	operator = (const ErrorRef& src)
	{
		attach (src);
		return *this;
	}

	bool
	isKindOf (
		const sl::Guid& guid,
		uint_t code
		) const
	{
		return !isEmpty () && m_p->isKindOf (guid, code);
	}

	sl::String
	getDescription () const;
};

//.............................................................................

class Error: public ref::Buf <ErrorHdr, SizeOfError, ErrorRef>
{
public:
	Error ()
	{
	}

	Error (const Error& src)
	{
		copy (src);
	}

	Error (const ErrorRef& src)
	{
		copy (src);
	}

	Error (const ErrorHdr* src)
	{
		copy (src);
	}

	Error (uint_t code)
	{
		createSystemError (code);
	}

	Error (
		const sl::Guid& guid,
		uint_t code
		)
	{
		createSimpleError (guid, code);
	}

	Error (
		const char* string,
		size_t length = -1
		)
	{
		createStringError (string, length);
	}

	Error (
		ref::BufKind kind,
		void* p,
		size_t size
		)
	{
		setBuffer (kind, p, size);
	}

	size_t
	push (const ErrorRef& error);

	// pack

	template <typename Pack>
	size_t
	pack_va (
		const sl::Guid& guid,
		uint_t code,
		axl_va_list va
		)
	{
		size_t packSize;
		Pack () (NULL, &packSize, va);

		size_t size = sizeof (ErrorHdr) + packSize;
		createBuffer (size);
		if (!m_p)
			return NULL;

		m_p->m_size = size;
		m_p->m_guid = guid;
		m_p->m_code = code;

		Pack () (m_p + 1, &packSize, va);
		return size;
	}

	template <typename Pack>
	size_t
	pack (
		const sl::Guid& guid,
		uint_t code,
		...
		)
	{
		AXL_VA_DECL (va, code);
		return pack_va <Pack> (guid, code, va);
	}

	template <typename Pack>
	size_t
	pushPack_va (
		const sl::Guid& guid,
		uint_t code,
		axl_va_list va
		)
	{
		if (!m_p)
			return pack_va <Pack> (guid, code, va);

		Error error;
		error.pack_va <Pack> (guid, code, va);
		return push (error);
	}

	template <typename Pack>
	size_t
	pushPack (
		const sl::Guid& guid,
		uint_t code,
		...
		)
	{
		AXL_VA_DECL (va, code);
		return pushPack_va <Pack> (guid, code, va);
	}

	// format

	size_t
	format_va (
		const sl::Guid& guid,
		uint_t code,
		const char* formatString,
		axl_va_list va
		);

	size_t
	format (
		const sl::Guid& guid,
		uint_t code,
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return format_va (guid, code, formatString, va);
	}

	size_t
	pushFormat_va (
		const sl::Guid& guid,
		uint_t code,
		const char* formatString,
		axl_va_list va
		)
	{
		if (!m_p)
			return format_va (guid, code, formatString, va);

		Error error;
		error.format_va (guid, code, formatString, va);
		return push (error);
	}

	size_t
	pushFormat (
		const sl::Guid& guid,
		uint_t code,
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return pushFormat_va (guid, code, formatString, va);
	}

	// system error (push is irrelevant for system errors)

	size_t
	createSystemError (uint_t code)
	{
		return createSimpleError (g_systemErrorGuid, code);
	}

	// simple error

	size_t
	createSimpleError (
		const sl::Guid& guid,
		uint_t code
		);

	size_t
	pushSimpleError (
		const sl::Guid& guid,
		uint_t code
		)
	{
		if (!m_p)
			return createSimpleError (guid, code);

		Error error;
		error.createSimpleError (guid, code);
		return push (error);
	}

	// string error

	size_t
	createStringError (
		const char* p,
		size_t length = -1
		);

	size_t
	pushStringError (
		const char* p,
		size_t length = -1
		)
	{
		if (!m_p)
			return createStringError (p, length);

		Error error;
		error.createStringError (p, length);
		return push (error);
	}

	size_t
	formatStringError_va (
		const char* formatString,
		axl_va_list va
		);

	size_t
	formatStringError (
		const char* formatString,
		...
		)
	{
		AXL_VA_DECL (va, formatString);
		return formatStringError_va (formatString, va);
	}

	size_t
	pushFormatStringError_va (
		const char* formatString,
		axl_va_list va
		)
	{
		if (!m_p)
			return formatStringError_va (formatString, va);

		Error error;
		error.formatStringError_va (formatString, va);
		return push (error);
	}

	size_t
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

inline
ErrorRef::ErrorRef (uint_t code):
	BaseType (Error (code))
{
}

inline
ErrorRef::ErrorRef (
	const sl::Guid& guid,
	uint_t code
	):
	BaseType (Error (guid, code))
{
}

inline
ErrorRef::ErrorRef (
	const char* string,
	size_t length
	):
	BaseType (Error (string, length))
{
}

//.............................................................................

// utility functions

ErrorRef
getLastError ();

size_t
setError (const ErrorRef& error);

size_t
pushError (const ErrorRef& error);

sl::String
getLastErrorDescription ();

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// pack

template <typename Pack>
size_t
setPackError_va (
	const sl::Guid& guid,
	uint_t code,
	axl_va_list va
	)
{
	Error error;
	size_t result = error.pack_va <Pack> (guid, code, va);
	return result != -1 ? setError (error) : -1;
}

template <typename Pack>
size_t
setPackError (
	const sl::Guid& guid,
	uint_t code,
	...
	)
{
	AXL_VA_DECL (va, code);
	return setPackError_va <Pack> (guid, code, va);
}

template <typename Pack>
size_t
pushPackError_va (
	const sl::Guid& guid,
	uint_t code,
	axl_va_list va
	)
{
	Error error;
	size_t result = error.pack_va <Pack> (guid, code, va);
	return result != -1 ? pushError (error) : -1;
}

template <typename Pack>
size_t
pushPackError (
	const sl::Guid& guid,
	uint_t code,
	...
	)
{
	AXL_VA_DECL (va, code);
	return pushPackError_va <Pack> (guid, code, va);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// format

size_t
setFormatError_va (
	const sl::Guid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	);

inline
size_t
setFormatError (
	const sl::Guid& guid,
	uint_t code,
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return setFormatError_va (guid, code, formatString, va);
}

size_t
pushFormatError_va (
	const sl::Guid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	);

inline
size_t
pushFormatError (
	const sl::Guid& guid,
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
size_t
setError (
	const sl::Guid& guid,
	uint_t code
	)
{
	return setError (Error (guid, code));
}

inline
size_t
pushError (
	const sl::Guid& guid,
	uint_t code
	)
{
	return pushError (Error (guid, code));
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// string error

size_t
setError (
	const char* p,
	size_t length = -1
	);

size_t
pushError (
	const char* p,
	size_t length = -1
	);

size_t
setFormatStringError_va (
	const char* formatString,
	axl_va_list va
	);

inline
size_t
setFormatStringError (
	const char* formatString,
	...
	)
{
	AXL_VA_DECL (va, formatString);
	return setFormatStringError_va (formatString, va);
}

size_t
pushFormatStringError_va (
	const char* formatString,
	axl_va_list va
	);

inline
size_t
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
	const ErrorRef& error
	)
{
	setError (error);
	return failResult;
}

inline
bool
fail (const ErrorRef& error)
{
	return fail <bool> (false, error);
}

inline
size_t 
setLastSystemError ()
{
	return setError (getLastSystemErrorCode ());
}

template <typename T>
T
failWithLastSystemError (T failResult)
{
	setLastSystemError ();
	return failResult;
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
	sl::String
	getErrorDescription (const ErrorRef& error) = 0;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class StdErrorProvider: public ErrorProvider
{
public:
	virtual
	sl::String
	getErrorDescription (const ErrorRef& error);

protected:
	sl::String
	getStackErrorDescription (const ErrorRef& error);
};

//.............................................................................

} // namespace err
} // namespace axl

