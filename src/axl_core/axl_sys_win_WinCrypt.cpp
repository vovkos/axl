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
#include "axl_sys_win_WinCrypt.h"
#include "axl_sys_win_Certificate.h"
#include "axl_sys_win_CryptMsg.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
cryptQueryObjectFile(
	const sl::StringRef_w& fileName,
	dword_t expectedContentTypeFlags,
	dword_t expectedFormatTypeFlags,
	dword_t flags,
	dword_t* encodingType,
	dword_t* contentType,
	dword_t* formatType,
	HCERTSTORE* certStore,
	HCRYPTMSG* cryptMsg,
	const void** context
) {
	bool_t result = ::CryptQueryObject(
		CERT_QUERY_OBJECT_FILE,
		fileName.sz(),
		expectedContentTypeFlags,
		expectedFormatTypeFlags,
		flags,
		encodingType,
		contentType,
		formatType,
		certStore,
		cryptMsg,
		context
	);

	return err::complete(result);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
cryptDecodeObject(
	void* buffer,
	size_t bufferSize,
	dword_t encodingType,
	const char* oid,
	const void* p,
	size_t size,
	dword_t flags
) {
	dword_t resultSize = buffer ? bufferSize : 0;

	bool_t result = ::CryptDecodeObject(
		encodingType,
		oid,
		(uchar_t*)p,
		size,
		flags,
		buffer,
		&resultSize
	);

	if (!result)
		return err::failWithLastSystemError<size_t>(-1);

	return resultSize;
}

size_t
cryptDecodeObject(
	sl::Array<char>* buffer,
	dword_t encodingType,
	const char* oid,
	const void* p,
	size_t size,
	dword_t flags
) {
	size_t resultSize = cryptDecodeObject(NULL, 0, encodingType, oid, p, size, flags);
	if (resultSize == -1)
		return -1;

	bool result = buffer->setCount(resultSize);
	if (!result)
		return -1;

	return cryptDecodeObject(buffer->p(), resultSize, encodingType, oid, p, size, flags);
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
