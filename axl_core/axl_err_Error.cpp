#include "stdafx.h"
#include "axl_err_Error.h"
#include "axl_err_ErrorMgr.h"
#include "axl_rtl_Packer.h"
#include "axl_err_WinError.h"
#include "axl_err_NtError.h"

namespace axl {
namespace err {

//.............................................................................

const TError NoError = 
{
	sizeof (TError),
	GUID_NULL,
	0
};

//.............................................................................

void
RegisterStdProviders ()
{
	class CRegister
	{
	public:
		void
		operator () ()
		{
			CStdErrorProvider::Register ();
			CWinErrorProvider::Register ();
			CNtErrorProvider::Register ();
		}
	};

	exe::CallOnce (CRegister());
}

//.............................................................................

rtl::CString
TError::GetDescription () const
{
	RegisterStdProviders ();

	IErrorProvider* pProvider = GetErrorMgr ()->FindProvider (m_Guid);
	
	return pProvider ? 
		pProvider->GetErrorDescription (this) : 
		rtl::CString::Format_s (
			_T("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x::%d"),
			m_Guid.Data1, m_Guid.Data2, 
			m_Guid.Data3, m_Guid.Data4[0], 
			m_Guid.Data4[0], m_Guid.Data4[1], 
			m_Guid.Data4[2], m_Guid.Data4[3], 
			m_Guid.Data4[4], m_Guid.Data4[5], 
			m_Guid.Data4[6], m_Guid.Data4[7],
			m_Code
			);
}

//.............................................................................

TError*
CError::Copy (const TError& Src)
{
	TError* pError = GetBuffer (Src.m_Size);
	if (!pError)
		return NULL;

	memcpy (pError, &Src, Src.m_Size);
	return pError;
}

TError*
CError::Push (const TError& Error)
{
	if (!m_p)
		return Copy (Error);

	size_t Base = 0;
	size_t BaseSize = m_p->m_Size;

	if (m_p->IsKind (GUID_StdError, EStdError_Stack))
	{
		Base += sizeof (TError);
		BaseSize -= sizeof (TError);
	}

	size_t Size = sizeof (TError) + Error.m_Size + BaseSize;

	GetBuffer (Size, true);
	if (!m_p)
		return NULL;

	memmove (
		(uchar_t*) m_p + sizeof (TError) + Error.m_Size,
		(uchar_t*) m_p + Base,
		BaseSize
		);

	m_p->m_Size = Size;
	m_p->m_Guid = GUID_StdError;
	m_p->m_Code = EStdError_Stack;

	memcpy (m_p + 1, &Error, Error.m_Size);
	return m_p;
}

TError*
CError::CreateSimpleError (
	const GUID& Guid,
	long Code
	)
{
	GetBuffer (sizeof (TError));
	if (!m_p)
		return NULL;

	m_p->m_Size = sizeof (TError);
	m_p->m_Guid = Guid;
	m_p->m_Code = Code;

	return m_p;
}

TError*
CError::FormatV (
	const GUID& Guid,
	long Code,
	const char* pFormat,
	va_list va
	)
{
	rtl::CPackerSeq Pack;
	Pack.Format (pFormat);

	size_t PackSize;
	Pack.PackV (NULL, &PackSize, va);
	
	size_t Size = sizeof (TError) + PackSize;

	GetBuffer (Size);
	if (!m_p)
		return NULL;

	m_p->m_Size = Size;
	m_p->m_Guid = Guid;
	m_p->m_Code = Code;

	Pack.PackV (m_p + 1, &PackSize, va);
	return m_p;
}

TError*
CError::CreateStringError (
	const tchar_t* p,
	size_t Length
	)
{
	if (Length == -1)
		Length = _tcslen (p);

	size_t Size = sizeof (TError) + (Length + 1) * sizeof (tchar_t);

	TError* pError = GetBuffer (Size);
	if (!pError)
		return NULL;

	pError->m_Size = Size;
	pError->m_Guid = GUID_StdError;
	pError->m_Code = EStdError_String;

	tchar_t* pDst = (tchar_t*) (pError + 1);

	memcpy (pDst, p, Length * sizeof (tchar_t));
	pDst [Length] = 0;

	return pError;
}

TError*
CError::FormatStringErrorV (
	const tchar_t* pFormat,
	va_list va
	)
{
	tchar_t Buffer [256];
	rtl::CString String (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String.FormatV (pFormat, va);
	return CreateStringError (String, String.GetLength ());
}

//.............................................................................

EErrorMode
SetErrorMode (EErrorMode Mode)
{
	return GetErrorMgr ()->SetErrorMode (Mode);
}

CError
GetError ()
{
	return GetErrorMgr ()->GetError ();
}

CError
SetError (const CError& Error)
{
	GetErrorMgr ()->SetError (Error);
	return Error;
}

//.............................................................................

void 
CStdErrorProvider::Register ()
{
	GetErrorMgr ()->RegisterProvider (GUID_StdError, rtl::GetSimpleSingleton <CStdErrorProvider> ());
}

rtl::CString 
CStdErrorProvider::GetErrorDescription (const TError* pError)
{
	if (pError->m_Size <= sizeof (TError))
		return rtl::CString ();

	size_t Length;

	switch (pError->m_Code)
	{
	case EStdError_NoError:
		return _T("no error");

	case EStdError_StringA:
		Length = (pError->m_Size - sizeof (TError)) / sizeof (char);
		return rtl::CString ((char*) (pError + 1), Length);

	case EStdError_StringW:
		Length = (pError->m_Size - sizeof (TError)) / sizeof (wchar_t);
		return rtl::CString ((wchar_t*) (pError + 1), Length);

	case EStdError_Stack:
		return GetStackErrorDescription (pError);

	default:
		return rtl::CString::Format_s (_T("error #%d"));
	}
}

rtl::CString 
CStdErrorProvider::GetStackErrorDescription (const TError* pError)
{
	rtl::CString String;

	void* pEnd = (uchar_t*) pError + (pError->m_Size);
	const TError* p = pError + 1;
	
	while (p < pEnd)
	{
		ASSERT (p->m_Size >= sizeof (TError));

		if (!String.IsEmpty ())
			String += _T(": ");

		String += p->GetDescription ();
		p = (TError*) ((uchar_t*) p + p->m_Size);
	}

	return String;
}

//.............................................................................

} // namespace err
} // namespace axl
