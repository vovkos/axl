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
#include "axl_sys_win_Registry.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
RegKey::open(
	HKEY hParentKey,
	const sl::String_w& name,
	REGSAM access
) {
	close();

	long result = ::RegOpenKeyExW(
		hParentKey,
		name.sz(),
		0,
		access,
		&m_h
	);

	if (result != ERROR_SUCCESS) {
		err::setError(result);
		return false;
	}

	return true;
}

bool
RegKey::queryValue(
	sl::Array<char>* buffer,
	const sl::String_w& name,
	dword_t* type
) {
	ASSERT(isOpen());
	dword_t size = buffer->getCount();

	for (;;) {
		long result = ::RegQueryValueExW(m_h, name, NULL, type, (PBYTE)buffer->p(), &size);
		switch (result) {
		case ERROR_MORE_DATA:
			break;

		case ERROR_SUCCESS:
			if (buffer->isEmpty()) // on zero size RegQueryValueExW returns ERROR_SUCCESS, not ERROR_MORE_DATA
				break;

			buffer->setCount(size); // ensure proper result size
			return true;

		default:
			err::setError(result);
			return false;
		}

		result = buffer->setCount(size);
		if (!result)
			return false;
	}
}

bool
RegKey::queryValue(
	sl::String_w* string,
	const sl::String_w& name
) {
	char stackBuffer[512];
	sl::Array<char> buffer(rc::BufKind_Stack, stackBuffer, sizeof(stackBuffer));
	bool result = queryValue(&buffer, name);
	if (!result)
		return false;

	size_t length = buffer.getCount() / sizeof(wchar_t);
	if (!length) {
		string->clear();
		return true;
	}

	const wchar_t* p = (wchar_t*)buffer.cp();
	if (!p[length - 1])
		length--;

	string->copy(p, length);
	return true;
}

bool
RegKey::queryValue(
	dword_t* value,
	const sl::String_w& name
) {
	char stackBuffer[128];
	sl::Array<char> buffer(rc::BufKind_Stack, stackBuffer, sizeof(stackBuffer));
	bool result = queryValue(&buffer, name);
	if (!result)
		return false;

	if (buffer.getCount() < sizeof(dword_t))
		return err::fail(err::SystemErrorCode_BufferTooSmall);

	*value = *(dword_t*)buffer.cp();
	return true;
}

bool
RegKey::queryValue(
	qword_t* value,
	const sl::String_w& name
) {
	char stackBuffer[128];
	sl::Array<char> buffer(rc::BufKind_Stack, stackBuffer, sizeof(stackBuffer));
	bool result = queryValue(&buffer, name);
	if (!result)
		return false;

	if (buffer.getCount() < sizeof(qword_t))
		return err::fail(err::SystemErrorCode_BufferTooSmall);

	*value = *(qword_t*)buffer.cp();
	return true;
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl

