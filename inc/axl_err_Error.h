// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_ERR_ERROR_H

#include "axl_rtl_String.h"

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

// {00000000-0000-0000-0000-000000000000}

AXL_DEFINE_GUID (
	GUID_StdError,
	0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	);

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum EStdError
{
	EStdError_NoError,
	EStdError_StringA,
	EStdError_StringW,
	EStdError_Stack,

#ifdef _UNICODE
	EStdError_String = EStdError_StringW,
#else
	EStdError_String = EStdError_StringA,
#endif
};

//.............................................................................

// system errors

#if (_AXL_ENV == AXL_ENV_WIN)

extern const GUID GUID_WinError;

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

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

#elif (_AXL_ENV == AXL_ENV_NT)

extern const GUID GUID_NtError;

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

#endif

//.............................................................................

// POD structure

struct TError
{
	ulong_t m_Size;
	GUID m_Guid;	
	long m_Code;

	// possibly followed by error data

	rtl::CString
	GetDescription () const;

	bool
	IsKind (
		const GUID& Guid,
		long Code
		) const
	{
		return InlineIsEqualGUID (m_Guid, Guid) && m_Code == Code;
	}
};

extern const TError NoError;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CGetErrorSize
{
public:
	size_t
	operator () (const err::TError& x)
	{
		ASSERT (x.m_Size >= sizeof (err::TError));
		return max (x.m_Size, sizeof (err::TError));
	}
};

//.............................................................................

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
		const GUID& Guid,
		long Code,
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
		long Code,
		ref::EBuf Kind = ref::EBuf_Stack,
		TError* p = (TError*) _alloca (MinBufSize),
		size_t Size = MinBufSize
		)
	{ 
		if (p)
			SetBuffer (Kind, p, Size);

		CreateSystemError (Code);
	}

	CError (
		const tchar_t* pFormat,
		...
		)
	{
		FormatStringErrorV (pFormat, va_start_e (pFormat));
	}

	bool
	IsKind (
		const GUID& Guid,
		long Code
		) const
	{
		return m_p && m_p->IsKind (Guid, Code);
	}

	rtl::CString
	GetDescription () const
	{
		return m_p ? 
			m_p->GetDescription () :
			NoError.GetDescription ();
	}

	TError*
	Copy (const TError& Src);

	TError*
	Push (const TError& Error); 

	// pack

	template <typename TPack>
	TError*
	PackV (	
		const GUID& Guid,
		long Code, 
		va_list va
		)
	{
		size_t PackSize;
		TPack () (NULL, &PackSize, va);
	
		size_t Size = sizeof (TError) + PackSize;
		GetBuffer (Size);
		if (!m_p)
			return false;

		m_p->m_Size = Size;
		m_p->m_Guid = Guid;
		m_p->m_Code = Code;

		TPack () (m_p + 1, &PackSize, va);
		return m_p;
	}

	template <typename TPack>
	TError*
	Pack (
		const GUID& Guid,
		long Code, 
		...
		)
	{
		return PackV <TPack> (Guid, Code, va_start_e (Code));
	}

	template <typename TPack>
	TError*
	PushPackV (	
		const GUID& Guid,
		long Code, 
		va_list va
		)
	{
		if (!m_p)
			return PackV (Guid, Code, va);

		CError Error;
		Error.PackV <TPack> (Guid, Code, va);
		return Push (*Error);
	}

	template <typename TPack>
	TError*
	PushPack (
		const GUID& Guid,
		long Code, 
		...
		)
	{
		return PushPackV <TPack> (Guid, Code, va_start_e (Code));
	}

	// format

	TError*
	FormatV (	
		const GUID& Guid,
		long Code, 
		const char* pFormat,
		va_list va
		);

	TError*
	Format (
		const GUID& Guid,
		long Code,
		const char* pFormat,
		...
		)
	{
		return FormatV (Guid, Code, pFormat, va_start_e (pFormat));
	}

	TError*
	PushFormatV (	
		const GUID& Guid,
		long Code, 
		const char* pFormat,
		va_list va
		)
	{
		if (!m_p)
			return FormatV (Guid, Code, pFormat, va);

		CError Error;
		Error.FormatV (Guid, Code, pFormat, va);
		return Push (*Error);
	}

	TError*
	PushFormat (
		const GUID& Guid,
		long Code,
		const char* pFormat,
		...
		)
	{
		return PushFormatV (Guid, Code, pFormat, va_start_e (pFormat));
	}

	// simple error

	TError*
	CreateSimpleError (
		const GUID& Guid,
		long Code
		);	

	TError*
	PushSimpleError (
		const GUID& Guid,
		long Code
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
	CreateSystemError (long Code)
	{
		return CreateSimpleError (GUID_SystemError, Code);
	}

	// string error

	TError* 
	CreateStringError (
		const tchar_t* p,
		size_t Length = -1
		); 

	TError* 
	PushStringError (
		const tchar_t* p,
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
	FormatStringErrorV (
		const tchar_t* pFormat,
		va_list va
		);

	TError*
	FormatStringError (
		const tchar_t* pFormat,
		...
		)
	{
		return FormatStringErrorV (pFormat, va_start_e (pFormat));
	}

	TError*
	PushFormatStringErrorV (
		const tchar_t* pFormat,
		va_list va
		)
	{
		if (!m_p)
			return FormatStringErrorV (pFormat, va);

		CError Error;
		Error.FormatStringErrorV (pFormat, va);
		return Push (*Error);
	}


	TError*
	PushFormatStringError (
		const tchar_t* pFormat,
		...
		)
	{
		return PushFormatStringErrorV (pFormat, va_start_e (pFormat));
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
SetPackErrorV (	
	const GUID& Guid,
	long Code, 
	va_list va
	)
{
	CError Error;
	Error.PackV <TPack> (Guid, Code, va);
	return SetError (Error);
}

template <typename TPack>
CError
SetPackError (
	const GUID& Guid,
	long Code, 
	...
	)
{
	return SetPackErrorV <TPack> (Guid, Code, va_start_e (Code));
}

template <typename TPack>
CError
PushPackErrorV (	
	const GUID& Guid,
	long Code, 
	va_list va
	)
{
	CError Error;
	Error.PackV <TPack> (Guid, Code, va);
	return PushError (Error);
}

template <typename TPack>
CError
PushPackError (
	const GUID& Guid,
	long Code, 
	...
	)
{
	return PushPackErrorV <TPack> (Guid, Code, va_start_e (Code));
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// format

inline
CError
SetFormatErrorV (	
	const GUID& Guid,
	long Code, 
	const char* pFormat,
	va_list va
	)
{
	CError Error;
	Error.FormatV (Guid, Code, pFormat, va);
	return SetError (Error);
}

inline
CError
SetFormatError (
	const GUID& Guid,
	long Code,
	const char* pFormat,
	...
	)
{
	return SetFormatErrorV (Guid, Code, pFormat, va_start_e (pFormat));
}

inline
CError
PushFormatErrorV (	
	const GUID& Guid,
	long Code, 
	const char* pFormat,
	va_list va
	)
{
	CError Error;
	Error.FormatV (Guid, Code, pFormat, va);
	return PushError (Error);
}

inline
CError
PushFormatError (
	const GUID& Guid,
	long Code,
	const char* pFormat,
	...
	)
{
	return PushFormatErrorV (Guid, Code, pFormat, va_start_e (pFormat));
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


// simple error

inline
CError
SetError (
	const GUID& Guid,
	long Code
	)
{
	return SetError (CError (Guid, Code));
}

inline
CError
PushError (
	const GUID& Guid,
	long Code
	)
{
	return PushError (CError (Guid, Code));
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// string error

inline
CError
SetStringError (
	const tchar_t* p,
	size_t Length = -1
	)
{
	CError Error;
	Error.CreateStringError (p, -1);
	return SetError (Error);
}

inline
CError
PushStringError (
	const tchar_t* p,
	size_t Length = -1
	)
{
	CError Error;
	Error.CreateStringError (p, -1);
	return SetError (Error);
}

inline
CError
SetFormatStringErrorV (
	const tchar_t* pFormat,
	va_list va
	)
{
	CError Error;
	Error.FormatStringErrorV (pFormat, va);
	return SetError (Error);
}

inline
CError
SetFormatStringError (
	const tchar_t* pFormat,
	...
	)
{
	return SetFormatStringErrorV (pFormat, va_start_e (pFormat));
}

inline
CError
PushFormatStringErrorV (
	const tchar_t* pFormat,
	va_list va
	)
{
	CError Error;
	Error.FormatStringErrorV (pFormat, va);
	return PushError (Error);
}

inline
CError
PushFormatStringError (
	const tchar_t* pFormat,
	...
	)
{
	return PushFormatStringErrorV (pFormat, va_start_e (pFormat));
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// system error

inline 
CError
SetLastSystemError ()
{
	return SetError (::GetLastError ());
}

inline
bool
FailWithSystemError (long Code)
{
	SetError (Code);
	return false;	
}

inline
bool
FailWithLastSystemError ()
{
	return FailWithSystemError (::GetLastError ());
}

inline 
bool 
Complete (int Result)
{
	if (Result)
		return true;

	SetLastSystemError ();
	return false;
}

//.............................................................................

// providers

[uuid ("ea112d53-04b0-468e-83ec-417acafad3b3")]
struct IErrorProvider: obj::IRoot
{
	virtual 
	rtl::CString 
	GetErrorDescription (const TError* pError) = 0;
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CStdErrorProvider: public IErrorProvider
{
public:
	AXL_OBJ_SIMPLE_CLASS (CStdErrorProvider, IErrorProvider)

public:
	static 
	void 
	Register ();

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

