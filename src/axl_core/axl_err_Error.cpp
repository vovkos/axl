#include "pch.h"
#include "axl_err_Error.h"
#include "axl_err_ErrorMgr.h"
#include "axl_rtl_Packer.h"

namespace axl {
namespace err {
	
//.............................................................................

bool
TError::IsKind (
	const rtl::TGuid& Guid,
	uint_t Code
	) const
{
	
	int x = ENOMEM;
	
	const TError* pError = this;
	
	if (m_Guid == rtl::GUID_Null && m_Code == EStdError_Stack)
		pError++;

	return pError->m_Guid == Guid && pError->m_Code == Code;
}

rtl::CString
TError::GetDescription () const
{
	IErrorProvider* pProvider = GetErrorMgr ()->FindProvider (m_Guid);
	
	return pProvider ? 
		pProvider->GetErrorDescription (this) : 
		rtl::CString::Format_s ("%s::%d", m_Guid.GetGuidString ().cc (), m_Code); // thanks a lot gcc
}

//.............................................................................

rtl::CString
CError::GetDescription () const
{
	return m_p ? 
		m_p->GetDescription () :
		NoError.GetDescription ();
}

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

	m_p->m_Size = (uint32_t) Size;
	m_p->m_Guid = GUID_StdError;
	m_p->m_Code = EStdError_Stack;

	memcpy (m_p + 1, &Error, Error.m_Size);
	return m_p;
}

TError*
CError::CreateSimpleError (
	const rtl::TGuid& Guid,
	uint_t Code
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
CError::Format_va (
	const rtl::TGuid& Guid,
	uint_t Code,
	const char* pFormat,
	axl_va_list va
	)
{
	rtl::CPackerSeq Pack;
	Pack.Format (pFormat);

	size_t PackSize;
	Pack.Pack_va (NULL, &PackSize, va);
	
	size_t Size = sizeof (TError) + PackSize;

	GetBuffer (Size);
	if (!m_p)
		return NULL;

	m_p->m_Size = (uint32_t) Size;
	m_p->m_Guid = Guid;
	m_p->m_Code = Code;

	Pack.Pack_va (m_p + 1, &PackSize, va);
	return m_p;
}

TError*
CError::CreateStringError (
	const char* p,
	size_t Length
	)
{
	if (Length == -1)
		Length = strlen (p);

	size_t Size = sizeof (TError) + Length + 1;

	TError* pError = GetBuffer (Size);
	if (!pError)
		return NULL;

	pError->m_Size = (uint32_t) Size;
	pError->m_Guid = GUID_StdError;
	pError->m_Code = EStdError_String;

	char* pDst = (char*) (pError + 1);

	memcpy (pDst, p, Length);
	pDst [Length] = 0;

	return pError;
}

TError*
CError::FormatStringError_va (
	const char* pFormat,
	axl_va_list va
	)
{
	char Buffer [256];
	rtl::CString String (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	String.Format_va (pFormat, va);
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

rtl::CString 
CStdErrorProvider::GetErrorDescription (const TError* pError)
{
	if (pError->m_Size <= sizeof (TError))
		return rtl::CString ();

	size_t Length;

	switch (pError->m_Code)
	{
	case EStdError_NoError:
		return "no error";

	case EStdError_String:
		Length = (pError->m_Size - sizeof (TError)) / sizeof (char);
		return rtl::CString ((char*) (pError + 1), Length);

	case EStdError_Stack:
		return GetStackErrorDescription (pError);

	default:
		return rtl::CString::Format_s ("error #%d");
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
			String += ": ";

		String += p->GetDescription ();
		p = (TError*) ((uchar_t*) p + p->m_Size);
	}

	return String;
}

//.............................................................................

} // namespace err
} // namespace axl
