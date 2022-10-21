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

#pragma once

#define _AXL_SYS_WIN_REGISTRY_H

#include "axl_sl_Handle.h"
#include "axl_sl_Array.h"
#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class CloseRegKey {
public:
	void
	operator () (HKEY h) {
		long result = ::RegCloseKey(h);
		ASSERT(result == ERROR_SUCCESS);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef sl::Handle<HKEY, CloseRegKey, sl::MinusOne<HKEY> > RegKeyHandle;

//..............................................................................

class RegKey: public RegKeyHandle {
public:
	bool
	open(
		HKEY hParentKey,
		const sl::String_w& name,
		REGSAM access = KEY_ALL_ACCESS
	);

	bool
	queryValue(
		sl::Array<char>* buffer,
		const sl::String_w& name,
		dword_t* type = NULL
	);

	bool
	queryValue(
		sl::String_w* string,
		const sl::String_w& name
	);

	bool
	queryValue(
		dword_t* value,
		const sl::String_w& name
	);

	bool
	queryValue(
		qword_t* value,
		const sl::String_w& name
	);

	sl::String_w
	queryStringValue(const sl::String_w& name);

	dword_t
	queryDwordValue(const sl::String_w& name);

	qword_t
	queryQwordValue(const sl::String_w& name);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
sl::String_w
RegKey::queryStringValue(const sl::String_w& name) {
	sl::String_w value;
	queryValue(&value, name);
	return value;
}

inline
dword_t
RegKey::queryDwordValue(const sl::String_w& name) {
	dword_t value;
	queryValue(&value, name);
	return value;
}

inline
qword_t
RegKey::queryQwordValue(const sl::String_w& name) {
	qword_t value;
	queryValue(&value, name);
	return value;
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
