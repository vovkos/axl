//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_err_Error.h"
#include "axl_err_ErrorMgr.h"
#include "axl_sl_Packer.h"

namespace axl {
namespace err {

//..............................................................................

// we can't use any of our error facilities from axl_mem_Allocate.h
// this is a dedicated function for marking out-of-memory incidents

void
setOutOfMemoryError()
{
	setError(SystemErrorCode_InsufficientResources);
}

//..............................................................................

sl::StringRef
ErrorHdr::getDescription() const
{
	return getErrorMgr()->getErrorDescription(this);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

sl::String
ErrorRef::getDescription() const
{
	return !isEmpty() ? m_p->getDescription() : g_noError.getDescription();
}

//..............................................................................

size_t
Error::push(const ErrorRef& error)
{
	if (!m_p)
		return copy(error);

	size_t base = 0;
	size_t baseSize = m_p->m_size;

	if (isKindOf(g_stdErrorGuid, StdErrorCode_Stack))
	{
		base += sizeof(ErrorHdr);
		baseSize -= sizeof(ErrorHdr);
	}

	size_t size = sizeof(ErrorHdr) + error->m_size + baseSize;

	createBuffer(size, true);
	if (!m_p)
		return -1;

	memmove(
		(uchar_t*)m_p + sizeof(ErrorHdr) + error->m_size,
		(uchar_t*)m_p + base,
		baseSize
		);

	m_p->m_size = (uint32_t)size;
	m_p->m_guid = g_stdErrorGuid;
	m_p->m_code = StdErrorCode_Stack;

	memcpy(m_p + 1, error, error->m_size);
	return size;
}

size_t
Error::createSimpleError(
	const sl::Guid& guid,
	uint_t code
	)
{
	createBuffer(sizeof(ErrorHdr));
	if (!m_p)
		return -1;

	m_p->m_size = sizeof(ErrorHdr);
	m_p->m_guid = guid;
	m_p->m_code = code;
	return sizeof(ErrorHdr);
}

size_t
Error::format_va(
	const sl::Guid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	)
{
	sl::PackerSeq packer;
	packer.format(formatString);

	size_t packSize;
	packer.pack_va(NULL, &packSize, va);

	size_t size = sizeof(ErrorHdr) + packSize;

	createBuffer(size);
	if (!m_p)
		return -1;

	m_p->m_size = (uint32_t)size;
	m_p->m_guid = guid;
	m_p->m_code = code;

	packer.pack_va(m_p + 1, &packSize, va);
	return size;
}

size_t
Error::createStringError(const sl::StringRef& string)
{
	size_t length = string.getLength();
	size_t size = sizeof(ErrorHdr) + length + 1;

	ErrorHdr* error = createBuffer(size);
	if (!error)
		return -1;

	error->m_size = (uint32_t)size;
	error->m_guid = g_stdErrorGuid;
	error->m_code = StdErrorCode_String;

	char* dst = (char*)(error + 1);

	memcpy(dst, string.cp(), length);
	dst[length] = 0;

	return size;
}

size_t
Error::formatStringError_va(
	const char* formatString,
	axl_va_list va
	)
{
	char buffer[256];
	sl::String string(rc::BufKind_Stack, buffer, sizeof(buffer));
	string.format_va(formatString, va);
	return createStringError(string);
}

//..............................................................................

ErrorRef
getLastError()
{
	return getErrorMgr()->getLastError();
}

size_t
setError(const ErrorRef& error)
{
	getErrorMgr()->setError(error);
	return error.getSize();
}

size_t
pushError(const ErrorRef& error)
{
	Error stack = getLastError();
	ASSERT(!stack.isKindOf(g_stdErrorGuid, StdErrorCode_NoError));
	size_t result = stack.push(error);
	return result != -1 ? setError(stack) : -1;
}

//..............................................................................

size_t
setFormatError_va(
	const sl::Guid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	)
{
	Error error;
	size_t result = error.format_va(guid, code, formatString, va);
	return result != -1 ? setError(error) : -1;
}

size_t
pushFormatError_va(
	const sl::Guid& guid,
	uint_t code,
	const char* formatString,
	axl_va_list va
	)
{
	Error error;
	size_t result = error.format_va(guid, code, formatString, va);
	return result != -1 ? pushError(error) : -1;
}

size_t
setError(const sl::StringRef& string)
{
	Error error;
	size_t result = error.createStringError(string);
	return result != -1 ? setError(error) : -1;
}

size_t
pushError(const sl::StringRef& string)
{
	Error error;
	size_t result = error.createStringError(string);
	return result != -1 ? pushError(error) : -1;
}

size_t
setFormatStringError_va(
	const char* formatString,
	axl_va_list va
	)
{
	Error error;
	size_t result = error.formatStringError_va(formatString, va);
	return result != -1 ? setError(error) : -1;
}

size_t
pushFormatStringError_va(
	const char* formatString,
	axl_va_list va
	)
{
	Error error;
	size_t result = error.formatStringError_va(formatString, va);
	return result != -1 ? pushError(error) : -1;
}

//..............................................................................

sl::StringRef
StdErrorProvider::getErrorDescription(const ErrorRef& error)
{
	if (error->m_size < sizeof(ErrorHdr))
		return sl::String();

	const char* p;
	size_t stringSize;

	switch (error->m_code)
	{
	case StdErrorCode_NoError:
		return "no error";

	case StdErrorCode_String:
		p = (const char*) (error + 1);
		stringSize = error->m_size - sizeof(ErrorHdr);

		return stringSize && !p[stringSize - 1] ?
			sl::StringRef(error.getHdr(), p, stringSize - 1, true) :
			sl::StringRef(error.getHdr(), p, stringSize, false);

	case StdErrorCode_Stack:
		return getStackErrorDescription(error);

	default:
		return sl::formatString("error #%d");
	}
}

sl::String
StdErrorProvider::getStackErrorDescription(const ErrorRef& error)
{
	sl::String string;

	const ErrorHdr* p = error + 1;
	const void* end = error.getEnd();

	while (p < end)
	{
		ASSERT(p->m_size >= sizeof(ErrorHdr));

		if (!string.isEmpty())
			string += ": ";

		string += ErrorRef(p).getDescription();
		p = (ErrorHdr*)((uchar_t*)p + p->m_size);
	}

	return string;
}

//..............................................................................

} // namespace err
} // namespace axl
