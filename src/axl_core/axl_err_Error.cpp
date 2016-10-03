#include "pch.h"
#include "axl_err_Error.h"
#include "axl_err_ErrorMgr.h"
#include "axl_sl_Packer.h"

namespace axl {
namespace err {

//.............................................................................

sl::String
ErrorHdr::getDescription () const
{
	ErrorProvider* provider = getErrorMgr ()->findProvider (m_guid);

	return provider ?
		provider->getErrorDescription (this) :
		sl::String::format_s ("%s::%d", m_guid.getGuidString ().cc (), m_code);
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::String
ErrorRef::getDescription () const
{
	return !isEmpty () ? m_p->getDescription () : g_noError.getDescription ();
}

//.............................................................................

size_t
Error::push (const ErrorRef& error)
{
	if (!m_p)
		return copy (error);

	size_t base = 0;
	size_t baseSize = m_p->m_size;

	if (isKindOf (g_stdErrorGuid, StdErrorCode_Stack))
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
	return size;
}

size_t
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
	return sizeof (ErrorHdr);
}

size_t
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
	return size;
}

size_t
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

	return size;
}

size_t
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

ErrorRef
getLastError ()
{
	return getErrorMgr ()->getLastError ();
}

size_t
setError (const ErrorRef& error)
{
	getErrorMgr ()->setError (error);
	return error.getSize ();
}

size_t
pushError (const ErrorRef& error)
{
	Error stack = getLastError ();
	ASSERT (!stack.isKindOf (g_stdErrorGuid, StdErrorCode_NoError));
	size_t result = stack.push (error);
	return result != -1 ? setError (stack) : -1;
}

sl::String
getLastErrorDescription ()
{
	return getErrorMgr ()->getLastError ().getDescription ();
}

//.............................................................................

size_t
setFormatError_va (
	const sl::Guid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	)
{
	Error error;
	size_t result = error.format_va (guid, code, formatString, va);
	return result != -1 ? setError (error) : -1;
}

size_t
pushFormatError_va (
	const sl::Guid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	)
{
	Error error;
	size_t result = error.format_va (guid, code, formatString, va);
	return result != -1 ? pushError (error) : -1;
}

size_t
setError (
	const char* p,
	size_t length
	)
{
	Error error;
	size_t result = error.createStringError (p, length);
	return result != -1 ? setError (error) : -1;
}

size_t
pushError (
	const char* p,
	size_t length
	)
{
	Error error;
	size_t result = error.createStringError (p, length);
	return result != -1 ? pushError (error) : -1;
}

size_t
setFormatStringError_va (
	const char* formatString,
	axl_va_list va
	)
{
	Error error;
	size_t result = error.formatStringError_va (formatString, va);
	return result != -1 ? setError (error) : -1;
}

size_t
pushFormatStringError_va (
	const char* formatString,
	axl_va_list va
	)
{
	Error error;
	size_t result = error.formatStringError_va (formatString, va);
	return result != -1 ? pushError (error) : -1;
}

//.............................................................................

sl::String
StdErrorProvider::getErrorDescription (const ErrorRef& error)
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
StdErrorProvider::getStackErrorDescription (const ErrorRef& error)
{
	sl::String string;

	const ErrorHdr* p = error + 1;
	const void* end = error.getEnd ();

	while (p < end)
	{
		ASSERT (p->m_size >= sizeof (ErrorHdr));

		if (!string.isEmpty ())
			string += ": ";

		string += ErrorRef (p).getDescription ();
		p = (ErrorHdr*) ((uchar_t*) p + p->m_size);
	}

	return string;
}

//.............................................................................

} // namespace err
} // namespace axl
