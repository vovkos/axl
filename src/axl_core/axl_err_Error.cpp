#include "pch.h"
#include "axl_err_Error.h"
#include "axl_err_ErrorMgr.h"
#include "axl_sl_Packer.h"

namespace axl {
namespace err {

//.............................................................................

bool
ErrorHdr::isKind (
	const sl::Guid& guid,
	uint_t code
	) const
{
	const ErrorHdr* error = this;

	if (m_guid == sl::g_nullGuid && m_code == StdErrorCode_Stack)
		error++;

	return error->m_guid == guid && error->m_code == code;
}

sl::String
ErrorHdr::getDescription () const
{
	ErrorProvider* provider = getErrorMgr ()->findProvider (m_guid);

	return provider ?
		provider->getErrorDescription (this) :
		sl::String::format_s ("%s::%d", m_guid.getGuidString ().cc (), m_code);
}

//.............................................................................

sl::String
Error::getDescription () const
{
	return m_p ?
		m_p->getDescription () :
		g_noError.getDescription ();
}

ErrorHdr*
Error::copy (const ErrorHdr* src)
{
	ErrorHdr* error = createBuffer (src->m_size);
	if (!error)
		return NULL;

	memcpy (error, src, src->m_size);
	return error;
}

ErrorHdr*
Error::push (const ErrorHdr* error)
{
	if (!m_p)
		return copy (error);

	size_t base = 0;
	size_t baseSize = m_p->m_size;

	if (m_p->isKind (g_stdErrorGuid, StdErrorCode_Stack))
	{
		base += sizeof (ErrorHdr);
		baseSize -= sizeof (ErrorHdr);
	}

	size_t size = sizeof (ErrorHdr) + error->m_size + baseSize;

	createBuffer (size, true);
	if (!m_p)
		return NULL;

	memmove (
		(uchar_t*) m_p + sizeof (ErrorHdr) + error->m_size,
		(uchar_t*) m_p + base,
		baseSize
		);

	m_p->m_size = (uint32_t) size;
	m_p->m_guid = g_stdErrorGuid;
	m_p->m_code = StdErrorCode_Stack;

	memcpy (m_p + 1, error, error->m_size);
	return m_p;
}

ErrorHdr*
Error::createSimpleError (
	const sl::Guid& guid,
	uint_t code
	)
{
	createBuffer (sizeof (ErrorHdr));
	if (!m_p)
		return NULL;

	m_p->m_size = sizeof (ErrorHdr);
	m_p->m_guid = guid;
	m_p->m_code = code;
	return m_p;
}

ErrorHdr*
Error::format_va (
	const sl::Guid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	)
{
	sl::PackerSeq packer;
	packer.format (formatString);

	size_t packSize;
	packer.pack_va (NULL, &packSize, va);

	size_t size = sizeof (ErrorHdr) + packSize;

	createBuffer (size);
	if (!m_p)
		return NULL;

	m_p->m_size = (uint32_t) size;
	m_p->m_guid = guid;
	m_p->m_code = code;

	packer.pack_va (m_p + 1, &packSize, va);
	return m_p;
}

ErrorHdr*
Error::createStringError (
	const char* p,
	size_t length
	)
{
	if (length == -1)
		length = strlen_s (p);

	size_t size = sizeof (ErrorHdr) + length + 1;

	ErrorHdr* error = createBuffer (size);
	if (!error)
		return NULL;

	error->m_size = (uint32_t) size;
	error->m_guid = g_stdErrorGuid;
	error->m_code = StdErrorCode_String;

	char* dst = (char*) (error + 1);

	memcpy (dst, p, length);
	dst [length] = 0;

	return error;
}

ErrorHdr*
Error::formatStringError_va (
	const char* formatString,
	axl_va_list va
	)
{
	char buffer [256];
	sl::String string (ref::BufKind_Stack, buffer, sizeof (buffer));
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
getLastError ()
{
	return getErrorMgr ()->getLastError ();
}

Error
setError (const Error& error)
{
	getErrorMgr ()->setError (error);
	return error;
}

sl::String
getLastErrorDescription ()
{
	return getErrorMgr ()->getLastError ()->getDescription ();
}

//.............................................................................

sl::String
StdErrorProvider::getErrorDescription (const ErrorHdr* error)
{
	if (error->m_size < sizeof (ErrorHdr))
		return sl::String ();

	size_t length;

	switch (error->m_code)
	{
	case StdErrorCode_NoError:
		return "no error";

	case StdErrorCode_String:
		length = (error->m_size - sizeof (ErrorHdr)) / sizeof (char);
		return sl::String ((char*) (error + 1), length);

	case StdErrorCode_Stack:
		return getStackErrorDescription (error);

	default:
		return sl::String::format_s ("error #%d");
	}
}

sl::String
StdErrorProvider::getStackErrorDescription (const ErrorHdr* error)
{
	sl::String string;

	void* end = (uchar_t*) error + (error->m_size);
	const ErrorHdr* p = error + 1;

	while (p < end)
	{
		ASSERT (p->m_size >= sizeof (ErrorHdr));

		if (!string.isEmpty ())
			string += ": ";

		string += p->getDescription ();
		p = (ErrorHdr*) ((uchar_t*) p + p->m_size);
	}

	return string;
}

//.............................................................................

} // namespace err
} // namespace axl
