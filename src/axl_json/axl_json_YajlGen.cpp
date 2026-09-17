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
#include "axl_json_YajlGen.h"

namespace axl {
namespace json {

//..............................................................................

bool
YajlGen::getString(sl::String* string) const {
	ASSERT(m_h);

	const uchar_t* p;
	size_t size;
	yajl_gen_status status = ::yajl_gen_get_buf(m_h, &p, &size);
	if (status != yajl_gen_status_ok)
		return failWithYajlGenError(status);

	string->copy((const char*)p, size);
	return true;
}

//..............................................................................

} // namespace json
} // namespace axl
