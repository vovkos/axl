#include "pch.h"
#include "axl_err_Error.h"
#include "axl_err_ErrorMgr.h"
#include "axl_rtl_Packer.h"

namespace axl {
namespace err {

//.............................................................................

bool
TError::isKind (
	const rtl::TGuid& guid,
	uint_t code
	) const
{
	const TError* error = this;

	if (m_guid == rtl::GUID_Null && m_code == EStdError_Stack)
		error++;

	return error->m_guid == guid && error->m_code == code;
}

rtl::CString
TError::getDescription () const
{
	CErrorProvider* provider = getErrorMgr ()->findProvider (m_guid);

	return provider ?
		provider->getErrorDescription (this) :
		rtl::CString::format_s ("%s::%d", m_guid.getGuidString ().cc (), m_code); // thanks a lot gcc
}

//.............................................................................

rtl::CString
CError::getDescription () const
{
	return m_p ?
		m_p->getDescription () :
		noError.getDescription ();
}

TError*
CError::copy (const TError& src)
{
	TError* error = getBuffer (src.m_size);
	if (!error)
		return NULL;

	memcpy (error, &src, src.m_size);
	return error;
}

TError*
CError::push (const TError& error)
{
	if (!m_p)
		return copy (error);

	size_t base = 0;
	size_t baseSize = m_p->m_size;

	if (m_p->isKind (GUID_StdError, EStdError_Stack))
	{
		base += sizeof (TError);
		baseSize -= sizeof (TError);
	}

	size_t size = sizeof (TError) + error.m_size + baseSize;

	getBuffer (size, true);
	if (!m_p)
		return NULL;

	memmove (
		(uchar_t*) m_p + sizeof (TError) + error.m_size,
		(uchar_t*) m_p + base,
		baseSize
		);

	m_p->m_size = (uint32_t) size;
	m_p->m_guid = GUID_StdError;
	m_p->m_code = EStdError_Stack;

	memcpy (m_p + 1, &error, error.m_size);
	return m_p;
}

TError*
CError::createSimpleError (
	const rtl::TGuid& guid,
	uint_t code
	)
{
	getBuffer (sizeof (TError));
	if (!m_p)
		return NULL;

	m_p->m_size = sizeof (TError);
	m_p->m_guid = guid;
	m_p->m_code = code;

	return m_p;
}

TError*
CError::format_va (
	const rtl::TGuid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	)
{
	rtl::CPackerSeq packer;
	packer.format (formatString);

	size_t packSize;
	packer.pack_va (NULL, &packSize, va);

	size_t size = sizeof (TError) + packSize;

	getBuffer (size);
	if (!m_p)
		return NULL;

	m_p->m_size = (uint32_t) size;
	m_p->m_guid = guid;
	m_p->m_code = code;

	packer.pack_va (m_p + 1, &packSize, va);
	return m_p;
}

TError*
CError::createStringError (
	const char* p,
	size_t length
	)
{
	if (length == -1)
		length = strlen (p);

	size_t size = sizeof (TError) + length + 1;

	TError* error = getBuffer (size);
	if (!error)
		return NULL;

	error->m_size = (uint32_t) size;
	error->m_guid = GUID_StdError;
	error->m_code = EStdError_String;

	char* dst = (char*) (error + 1);

	memcpy (dst, p, length);
	dst [length] = 0;

	return error;
}

TError*
CError::formatStringError_va (
	const char* formatString,
	axl_va_list va
	)
{
	char buffer [256];
	rtl::CString string (ref::EBuf_Stack, buffer, sizeof (buffer));
	string.format_va (formatString, va);
	return createStringError (string, string.getLength ());
}

//.............................................................................

EErrorMode
setErrorMode (EErrorMode mode)
{
	return getErrorMgr ()->setErrorMode (mode);
}

CError
getError ()
{
	return getErrorMgr ()->getError ();
}

CError
setError (const CError& error)
{
	getErrorMgr ()->setError (error);
	return error;
}

//.............................................................................

rtl::CString
CStdErrorProvider::getErrorDescription (const TError* error)
{
	if (error->m_size < sizeof (TError))
		return rtl::CString ();

	size_t length;

	switch (error->m_code)
	{
	case EStdError_NoError:
		return "no error";

	case EStdError_String:
		length = (error->m_size - sizeof (TError)) / sizeof (char);
		return rtl::CString ((char*) (error + 1), length);

	case EStdError_Stack:
		return getStackErrorDescription (error);

	default:
		return rtl::CString::format_s ("error #%d");
	}
}

rtl::CString
CStdErrorProvider::getStackErrorDescription (const TError* error)
{
	rtl::CString string;

	void* end = (uchar_t*) error + (error->m_size);
	const TError* p = error + 1;

	while (p < end)
	{
		ASSERT (p->m_size >= sizeof (TError));

		if (!string.isEmpty ())
			string += ": ";

		string += p->getDescription ();
		p = (TError*) ((uchar_t*) p + p->m_size);
	}

	return string;
}

//.............................................................................

} // namespace err
} // namespace axl
