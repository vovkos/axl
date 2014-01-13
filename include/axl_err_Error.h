// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
GetErrorMode ();

EErrorMode // returns previous one
SetErrorMode (EErrorMode Mode);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CScopeErrorMode
{
protected:
	EErrorMode m_OldMode;

public:
	CScopeErrorMode (EErrorMode Mode)
	{
		m_OldMode = SetErrorMode (Mode);
	}

	~CScopeErrorMode ()
	{
		SetErrorMode (m_OldMode);
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
GetLastSystemErrorCode ()
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
GetLastSystemErrorCode ()
{
	return errno;
}

#endif

//.............................................................................

// POD structure

struct TError
{
	uint32_t m_Size;
	rtl::TGuid m_Guid;
	uint32_t m_Code;

	// possibly followed by error data

	rtl::CString
	GetDescription () const;

	bool
	IsKind (
		const rtl::TGuid& Guid,
		uint_t Code
		) const;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

extern
AXL_SELECT_ANY
const TError NoError =
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
		ASSERT (x.m_Size >= sizeof (TError));
		return AXL_MAX (x.m_Size, sizeof (TError));
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

	CError (const TError& Src)
	{
		Copy (Src);
	}

	CError (
		ref::EBuf Kind,
		void* p,
		size_t Size
		)
	{
		SetBuffer (Kind, p, Size);
	}

	CError (
		const rtl::TGuid& Guid,
		uint_t Code,
		ref::EBuf Kind = ref::EBuf_Stack,
		TError* p = (TError*) _alloca (MinBufSize),
		size_t Size = MinBufSize
		)
	{
		if (p)
			SetBuffer (Kind, p, Size);

		CreateSimpleError (Guid, Code);
	}

	CError (
		uint_t Code,
		ref::EBuf Kind = ref::EBuf_Stack,
		TError* p = (TError*) _alloca (MinBufSize),
		size_t Size = MinBufSize
		)
	{
		if (p)
			SetBuffer (Kind, p, Size);

		CreateSystemError (Code);
	}

	bool
	IsKind (
		const rtl::TGuid& Guid,
		uint_t Code
		) const
	{
		return m_p && m_p->IsKind (Guid, Code);
	}

	rtl::CString
	GetDescription () const;

	TError*
	Copy (const TError& Src);

	TError*
	Push (const TError& Error);

	// pack

	template <typename TPack>
	TError*
	Pack_va (
		const rtl::TGuid& Guid,
		uint_t Code,
		axl_va_list va
		)
	{
		size_t PackSize;
		TPack () (NULL, &PackSize, va);

		size_t Size = sizeof (TError) + PackSize;
		GetBuffer (Size);
		if (!m_p)
			return NULL;

		m_p->m_Size = Size;
		m_p->m_Guid = Guid;
		m_p->m_Code = Code;

		TPack () (m_p + 1, &PackSize, va);
		return m_p;
	}

	template <typename TPack>
	TError*
	Pack (
		const rtl::TGuid& Guid,
		uint_t Code,
		...
		)
	{
		AXL_VA_DECL (va, Code);
		return Pack_va <TPack> (Guid, Code, va);
	}

	template <typename TPack>
	TError*
	PushPack_va (
		const rtl::TGuid& Guid,
		uint_t Code,
		axl_va_list va
		)
	{
		if (!m_p)
			return Pack_va <TPack> (Guid, Code, va);

		CError Error;
		Error.Pack_va <TPack> (Guid, Code, va);
		return Push (*Error);
	}

	template <typename TPack>
	TError*
	PushPack (
		const rtl::TGuid& Guid,
		uint_t Code,
		...
		)
	{
		AXL_VA_DECL (va, Code);
		return PushPack_va <TPack> (Guid, Code, va);
	}

	// format

	TError*
	Format_va (
		const rtl::TGuid& Guid,
		uint_t Code,
		const char* pFormat,
		axl_va_list va
		);

	TError*
	Format (
		const rtl::TGuid& Guid,
		uint_t Code,
		const char* pFormat,
		...
		)
	{
		AXL_VA_DECL (va, pFormat);
		return Format_va (Guid, Code, pFormat, va);
	}

	TError*
	PushFormat_va (
		const rtl::TGuid& Guid,
		uint_t Code,
		const char* pFormat,
		axl_va_list va
		)
	{
		if (!m_p)
			return Format_va (Guid, Code, pFormat, va);

		CError Error;
		Error.Format_va (Guid, Code, pFormat, va);
		return Push (*Error);
	}

	TError*
	PushFormat (
		const rtl::TGuid& Guid,
		uint_t Code,
		const char* pFormat,
		...
		)
	{
		AXL_VA_DECL (va, pFormat);
		return PushFormat_va (Guid, Code, pFormat, va);
	}

	// simple error

	TError*
	CreateSimpleError (
		const rtl::TGuid& Guid,
		uint_t Code
		);

	TError*
	PushSimpleError (
		const rtl::TGuid& Guid,
		uint_t Code
		)
	{
		if (!m_p)
			return CreateSimpleError (Guid, Code);

		CError Error;
		Error.CreateSimpleError (Guid, Code);
		return Push (*Error);
	}

	// system error (push is irrelevant for system errors)

	TError*
	CreateSystemError (uint_t Code)
	{
		return CreateSimpleError (GUID_SystemError, Code);
	}

	// string error

	TError*
	CreateStringError (
		const char* p,
		size_t Length = -1
		);

	TError*
	PushStringError (
		const char* p,
		size_t Length = -1
		)
	{
		if (!m_p)
			return CreateStringError (p, Length);

		CError Error;
		Error.CreateStringError (p, Length);
		return Push (*Error);
	}

	TError*
	FormatStringError_va (
		const char* pFormat,
		axl_va_list va
		);

	TError*
	FormatStringError (
		const char* pFormat,
		...
		)
	{
		AXL_VA_DECL (va, pFormat);
		return FormatStringError_va (pFormat, va);
	}

	TError*
	PushFormatStringError_va (
		const char* pFormat,
		axl_va_list va
		)
	{
		if (!m_p)
			return FormatStringError_va (pFormat, va);

		CError Error;
		Error.FormatStringError_va (pFormat, va);
		return Push (*Error);
	}


	TError*
	PushFormatStringError (
		const char* pFormat,
		...
		)
	{
		AXL_VA_DECL (va, pFormat);
		return PushFormatStringError_va (pFormat, va);
	}
};

//.............................................................................

// utility functions

CError
GetError ();

CError
SetError (const CError& Error);

inline
CError
PushError (const CError& Error)
{
	CError Stack = GetError ();
	Stack.Push (*Error);
	return SetError (Stack);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// pack

template <typename TPack>
CError
SetPackError_va (
	const rtl::TGuid& Guid,
	uint_t Code,
	axl_va_list va
	)
{
	CError Error;
	Error.Pack_va <TPack> (Guid, Code, va);
	return SetError (Error);
}

template <typename TPack>
CError
SetPackError (
	const rtl::TGuid& Guid,
	uint_t Code,
	...
	)
{
	AXL_VA_DECL (va, Code);
	return SetPackError_va <TPack> (Guid, Code, va);
}

template <typename TPack>
CError
PushPackError_va (
	const rtl::TGuid& Guid,
	uint_t Code,
	axl_va_list va
	)
{
	CError Error;
	Error.Pack_va <TPack> (Guid, Code, va);
	return PushError (Error);
}

template <typename TPack>
CError
PushPackError (
	const rtl::TGuid& Guid,
	uint_t Code,
	...
	)
{
	AXL_VA_DECL (va, Code);
	return PushPackError_va <TPack> (Guid, Code, va);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// format

inline
CError
SetFormatError_va (
	const rtl::TGuid& Guid,
	uint_t Code,
	const char* pFormat,
	axl_va_list va
	)
{
	CError Error;
	Error.Format_va (Guid, Code, pFormat, va);
	return SetError (Error);
}

inline
CError
SetFormatError (
	const rtl::TGuid& Guid,
	uint_t Code,
	const char* pFormat,
	...
	)
{
	AXL_VA_DECL (va, pFormat);
	return SetFormatError_va (Guid, Code, pFormat, va);
}

inline
CError
PushFormatError_va (
	const rtl::TGuid& Guid,
	uint_t Code,
	const char* pFormat,
	axl_va_list va
	)
{
	CError Error;
	Error.Format_va (Guid, Code, pFormat, va);
	return PushError (Error);
}

inline
CError
PushFormatError (
	const rtl::TGuid& Guid,
	uint_t Code,
	const char* pFormat,
	...
	)
{
	AXL_VA_DECL (va, pFormat);
	return PushFormatError_va (Guid, Code, pFormat, va);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


// simple error

inline
CError
SetError (
	const rtl::TGuid& Guid,
	uint_t Code
	)
{
	return SetError (CError (Guid, Code));
}

inline
CError
PushError (
	const rtl::TGuid& Guid,
	uint_t Code
	)
{
	return PushError (CError (Guid, Code));
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// string error

inline
CError
CreateStringError (
	const char* p,
	size_t Length = -1
	)
{
	CError Error;
	Error.CreateStringError (p, Length);
	return Error;
}

inline
CError
SetStringError (
	const char* p,
	size_t Length = -1
	)
{
	return SetError (CreateStringError (p, Length));
}

inline
CError
PushStringError (
	const char* p,
	size_t Length = -1
	)
{
	CError Error;
	Error.CreateStringError (p, -1);
	return SetError (Error);
}

inline
CError
FormatStringError_va (
	const char* pFormat,
	axl_va_list va
	)
{
	CError Error;
	Error.FormatStringError_va (pFormat, va);
	return Error;
}

inline
CError
FormatStringError (
	const char* pFormat,
	...
	)
{
	AXL_VA_DECL (va, pFormat);
	return FormatStringError_va (pFormat, va);
}

inline
CError
SetFormatStringError_va (
	const char* pFormat,
	axl_va_list va
	)
{
	return SetError (FormatStringError_va (pFormat, va));
}

inline
CError
SetFormatStringError (
	const char* pFormat,
	...
	)
{
	AXL_VA_DECL (va, pFormat);
	return SetFormatStringError_va (pFormat, va);
}

inline
CError
PushFormatStringError_va (
	const char* pFormat,
	axl_va_list va
	)
{
	CError Error;
	Error.FormatStringError_va (pFormat, va);
	return PushError (Error);
}

inline
CError
PushFormatStringError (
	const char* pFormat,
	...
	)
{
	AXL_VA_DECL (va, pFormat);
	return PushFormatStringError_va (pFormat, va);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// convenient helpers

template <typename T>
T
Fail (
	T FailResult,
	CError Error
	)
{
	SetError (Error);
	return FailResult;
}

inline
bool
Fail (CError Error)
{
	return Fail <bool> (false, Error);
}

inline
CError
SetLastSystemError ()
{
	return SetError (GetLastSystemErrorCode ());
}

template <typename T>
T
FailWithLastSystemError (T FailResult)
{
	return Fail (FailResult, GetLastSystemErrorCode ());
}

inline
bool
FailWithLastSystemError ()
{
	return FailWithLastSystemError <bool> (false);
}

template <typename T>
T
CompleteWithSystemError (
	T Result,
	T FailResult,
	uint_t ErrorCode
	)
{
	if (Result == FailResult)
		SetError (ErrorCode);

	return Result;
}

template <typename T>
T
Complete (
	T Result,
	T FailResult
	)
{
	if (Result == FailResult)
		SetError (GetLastSystemErrorCode ());

	return Result;
}

inline
bool
Complete (int Result)
{
	return Complete <bool> (Result != 0, false);
}

//.............................................................................

// providers

class CErrorProvider
{
public:
	virtual
	rtl::CString
	GetErrorDescription (const TError* pError) = 0;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CStdErrorProvider: public CErrorProvider
{
public:
	virtual
	rtl::CString
	GetErrorDescription (const TError* pError);

protected:
	rtl::CString
	GetStackErrorDescription (const TError* pError);
};

//.............................................................................

} // namespace err
} // namespace axl

