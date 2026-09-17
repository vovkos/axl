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

#define _AXL_JSON_YAJLGEN_H

#include "axl_json_YajlError.h"

namespace axl {
namespace json {

//..............................................................................

class FreeYajlGen {
public:
	void
	operator () (yajl_gen h) {
		::yajl_gen_free(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class YajlGen: public sl::Handle<yajl_gen, FreeYajlGen> {
public:
	YajlGen() {
		bool result = create();
		ASSERT(result);
	}

	bool
	create();

	bool
	setOption(
		yajl_gen_option option,
		bool value = true
	);

	bool
	setIndentString(const sl::StringRef& string);

	bool
	getString(sl::String* string) const;

	sl::String
	getString() const;

	void
	clearBuffer() {
		ASSERT(m_h);
		::yajl_gen_clear(m_h);
	}

	void
	reset(const sl::StringRef& separator = sl::StringRef()) {
		ASSERT(m_h);
		::yajl_gen_reset(m_h, separator.isEmpty() ? NULL : separator.sz());
	}

	bool
	openMap() {
		return complete(::yajl_gen_map_open(m_h));
	}

	bool
	openMap(const sl::StringRef& key) {
		return addString(key) && openMap();
	}

	bool
	closeMap() {
		return complete(::yajl_gen_map_close(m_h));
	}

	bool
	openArray() {
		return complete(::yajl_gen_array_open(m_h));
	}

	bool
	openArray(const sl::StringRef& key) {
		return addString(key) && openArray();
	}

	bool
	closeArray() {
		return complete(::yajl_gen_array_close(m_h));
	}

	bool
	addNull() {
		return complete(::yajl_gen_null(m_h));
	}

	bool
	addNull(const sl::StringRef& key) {
		return addString(key) && addNull();
	}

	bool
	addBool(bool value) {
		return complete(::yajl_gen_bool(m_h, value));
	}

	bool
	addBool(
		const sl::StringRef& key,
		bool value
	) {
		return addString(key) && addBool(value);
	}

	bool
	addInteger(int64_t value) {
		return complete(::yajl_gen_integer(m_h, value));
	}

	bool
	addInteger(
		const sl::StringRef& key,
		int64_t value
	) {
		return addString(key) && addInteger(value);
	}

	bool
	addDouble(double value) {
		return complete(::yajl_gen_double(m_h, value));
	}

	bool
	addDouble(
		const sl::StringRef& key,
		double value
	) {
		return addString(key) && addDouble(value);
	}

	bool
	addNumber(const sl::StringRef& value) {
		return complete(::yajl_gen_number(m_h, value.cp(), value.getLength()));
	}

	bool
	addNumber(
		const sl::StringRef& key,
		const sl::StringRef& value
	) {
		return addString(key) && addNumber(value);
	}

	bool
	addString(const sl::StringRef& value) {
		return complete(::yajl_gen_string(m_h, (const uchar_t*)value.cp(), value.getLength()));
	}

	bool
	addString(
		const sl::StringRef& key,
		const sl::StringRef& value
	) {
		return addString(key) && addString(value);
	}

protected:
	bool
	complete(yajl_gen_status status) {
		return status == yajl_gen_status_ok ? true : failWithYajlGenError(status);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
YajlGen::create() {
	close();
	m_h = ::yajl_gen_alloc(NULL);
	return m_h ? true : err::fail(err::SystemErrorCode_InsufficientResources);
}

inline
bool
YajlGen::setOption(
	yajl_gen_option option,
	bool value
) {
	ASSERT(m_h && (option == yajl_gen_beautify || option == yajl_gen_validate_utf8 || option == yajl_gen_escape_solidus));
	int result = ::yajl_gen_config(m_h, option, value);
	return result ? true : err::fail("invalid yajl generator option");
}

inline
bool
YajlGen::setIndentString(const sl::StringRef& string) {
	ASSERT(m_h);
	int result = ::yajl_gen_config(m_h, yajl_gen_indent_string, string.sz());
	return result ? true : err::fail("invalid yajl indent string");
}

inline
sl::String
YajlGen::getString() const {
	sl::String string;
	getString(&string);
	return string;
}

//..............................................................................

} // namespace json
} // namespace axl
