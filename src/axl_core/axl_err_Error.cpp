#include "pch.h"
#include "axl_err_Error.h"
#include "axl_err_ErrorMgr.h"
#include "axl_rtl_Packer.h"

namespace axl {
namespace err {

//.............................................................................

bool
ErrorData::isKind (
	const rtl::Guid& guid,
	uint_t code
	) const
{
	const ErrorData* error = this;

	if (m_guid == rtl::GUID_Null && m_code == StdErrorCode_Stack)
		error++;

	return error->m_guid == guid && error->m_code == code;
}

rtl::String
ErrorData::getDescription () const
{
	ErrorProvider* provider = getErrorMgr ()->findProvider (m_guid);

	return provider ?
		provider->getErrorDescription (this) :
		rtl::String::format_s ("%s::%d", m_guid.getGuidString ().cc (), m_code);
}

//.............................................................................

rtl::String
Error::getDescription () const
{
	return m_p ?
		m_p->getDescription () :
		g_noError.getDescription ();
}

ErrorData*
Error::copy (const ErrorData& src)
{
	ErrorData* error = getBuffer (src.m_size);
	if (!error)
		return NULL;

	memcpy (error, &src, src.m_size);
	return error;
}

ErrorData*
Error::push (const ErrorData& error)
{
	if (!m_p)
		return copy (error);

	size_t base = 0;
	size_t baseSize = m_p->m_size;

	if (m_p->isKind (GUID_StdError, StdErrorCode_Stack))
	{
		base += sizeof (ErrorData);
		baseSize -= sizeof (ErrorData);
	}

	size_t size = sizeof (ErrorData) + error.m_size + baseSize;

	getBuffer (size, true);
	if (!m_p)
		return NULL;

	memmove (
		(uchar_t*) m_p + sizeof (ErrorData) + error.m_size,
		(uchar_t*) m_p + base,
		baseSize
		);

	m_p->m_size = (uint32_t) size;
	m_p->m_guid = GUID_StdError;
	m_p->m_code = StdErrorCode_Stack;

	memcpy (m_p + 1, &error, error.m_size);
	return m_p;
}

ErrorData*
Error::createSimpleError (
	const rtl::Guid& guid,
	uint_t code
	)
{
	getBuffer (sizeof (ErrorData));
	if (!m_p)
		return NULL;

	m_p->m_size = sizeof (ErrorData);
	m_p->m_guid = guid;
	m_p->m_code = code;
	return m_p;
}

ErrorData*
Error::format_va (
	const rtl::Guid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	)
{
	rtl::PackerSeq packer;
	packer.format (formatString);

	size_t packSize;
	packer.pack_va (NULL, &packSize, va);

	size_t size = sizeof (ErrorData) + packSize;

	getBuffer (size);
	if (!m_p)
		return NULL;

	m_p->m_size = (uint32_t) size;
	m_p->m_guid = guid;
	m_p->m_code = code;

	packer.pack_va (m_p + 1, &packSize, va);
	return m_p;
}

ErrorData*
Error::createStringError (
	const char* p,
	size_t length
	)
{
	if (length == -1)
		length = strlen (p);

	size_t size = sizeof (ErrorData) + length + 1;

	ErrorData* error = getBuffer (size);
	if (!error)
		return NULL;

	error->m_size = (uint32_t) size;
	error->m_guid = GUID_StdError;
	error->m_code = StdErrorCode_String;

	char* dst = (char*) (error + 1);

	memcpy (dst, p, length);
	dst [length] = 0;

	return error;
}

ErrorData*
Error::formatStringError_va (
	const char* formatString,
	axl_va_list va
	)
{
	char buffer [256];
	rtl::String string (ref::BufKind_Stack, buffer, sizeof (buffer));
	string.format_va (formatString, va);
	return createStringError (string, string.getLength ());
}

//.............................................................................

ErrorMode
setErrorMode (ErrorMode mode)
{
	return getErrorMgr ()->setErrorMode (mode);
}

Error
getError ()
{
	return getErrorMgr ()->getError ();
}

Error
setError (const Error& error)
{
	getErrorMgr ()->setError (error);
	return error;
}

//.............................................................................

rtl::String
StdErrorProvider::getErrorDescription (const ErrorData* error)
{
	if (error->m_size < sizeof (ErrorData))
		return rtl::String ();

	size_t length;

	switch (error->m_code)
	{
	case StdErrorCode_NoError:
		return "no error";

	case StdErrorCode_String:
		length = (error->m_size - sizeof (ErrorData)) / sizeof (char);
		return rtl::String ((char*) (error + 1), length);

	case StdErrorCode_Stack:
		return getStackErrorDescription (error);

	default:
		return rtl::String::format_s ("error #%d");
	}
}

rtl::String
StdErrorProvider::getStackErrorDescription (const ErrorData* error)
{
	rtl::String string;

	void* end = (uchar_t*) error + (error->m_size);
	const ErrorData* p = error + 1;

	while (p < end)
	{
		ASSERT (p->m_size >= sizeof (ErrorData));

		if (!string.isEmpty ())
			string += ": ";

		string += p->getDescription ();
		p = (ErrorData*) ((uchar_t*) p + p->m_size);
	}

	return string;
}

//.............................................................................

} // namespace err
} // namespace axl
