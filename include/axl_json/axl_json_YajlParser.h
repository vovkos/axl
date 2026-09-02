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

#define _AXL_JSON_YAJLPARSER_H

#include "axl_json_YajlError.h"

namespace axl {
namespace json {

//..............................................................................

class FreeYajlParser {
public:
	void
	operator () (yajl_handle h) {
		::yajl_free(h);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class YajlParser: public sl::Handle<yajl_handle, FreeYajlParser> {
protected:
	class Callbacks: public yajl_callbacks {
	public:
		Callbacks() {
			yajl_null = &T::onNull != &YajlParser::onNull ? nullCallback : NULL;
			yajl_boolean = &T::onBoolean != &YajlParser::onBoolean ? booleanCallback : NULL;
			yajl_integer = &T::onInteger != &YajlParser::onInteger ? integerCallback : NULL;
			yajl_double = &T::onDouble != &YajlParser::onDouble ? doubleCallback : NULL;
			yajl_number = &T::onNumber != &YajlParser::onNumber ? numberCallback : NULL;
			yajl_string = &T::onString != &YajlParser::onString ? stringCallback : NULL;
			yajl_start_map  = &T::onStartMap != &YajlParser::onStartMap ? startMapCallback : NULL;
			yajl_map_key = &T::onMapKey != &YajlParser::onMapKey ? mapKeyCallback : NULL;
			yajl_end_map = &T::onEndMap != &YajlParser::onEndMap ? endMapCallback : NULL;
			yajl_start_array = &T::onStartArray != &YajlParser::onStartArray ? startArrayCallback : NULL;
			yajl_end_array = &T::onEndArray != &YajlParser::onEndArray ? endArrayCallback : NULL;
		}
	};

public:
	YajlParser() {
		bool result = create();
		ASSERT(result);
	}

	bool
	create() {
		close();
		m_h = ::yajl_alloc(sl::getSimpleSingleton<Callbacks>(), NULL, static_cast<T*>(this));
		return m_h ? true : err::fail(err::SystemErrorCode_InsufficientResources);
	}

	bool
	setOption(
		yajl_option option,
		bool value = true
	) {
		ASSERT(m_h);
		return ::yajl_config(m_h, option, (int)value) ? true : err::fail("invalid yajl parser option");
	}

	size_t
	getConsumedByteCount() const {
		ASSERT(m_h);
		return ::yajl_get_bytes_consumed(m_h);
	}

	sl::String
	getLastErrorString(
		const void* p = NULL,
		size_t size = 0,
		bool isVerbose = false
	) const {
		ASSERT(m_h);
		return getYajlErrorString(m_h, p, size, isVerbose);
	}

	bool
	parseFile(const sl::StringRef& fileName) {
		io::SimpleMappedFile file;
		return
			file.open(fileName, io::FileFlag_ReadOnly) &&
			parseChunk(file.p(), file.getMappingSize()) &&
			parseEof();
	}

	bool
	parse(const sl::StringRef& source) {
		return parse(source.cp(), source.getLength());
	}

	bool
	parse(
		const void* p,
		size_t size
	) {
		return
			parseChunk(p, size) &&
			parseEof();
	}

	bool
	parseChunk(
		const void* p,
		size_t size
	) {
		ASSERT(m_h);
		yajl_status status = ::yajl_parse(m_h, (const uchar_t*)p, size);
		return complete(status, p, size);
	}

	bool
	parseEof() {
		ASSERT(m_h);
		return complete(::yajl_complete_parse(m_h), NULL, 0);
	}

protected:
	// overridables; set error and return false to cancel the parse

	bool
	onNull() {
		return true;
	}

	bool
	onBoolean(bool value) {
		return true;
	}

	bool
	onInteger(int64_t value) {
		return true;
	}

	bool
	onDouble(double value) {
		return true;
	}

	bool
	onNumber(const sl::StringRef& value) { // onNumber overrides onInteger/onDouble
		return true;
	}

	bool
	onString(const sl::StringRef& value) {
		return true;
	}

	bool
	onStartMap() {
		return true;
	}

	bool
	onMapKey(const sl::StringRef& key) {
		return true;
	}

	bool
	onEndMap() {
		return true;
	}

	bool
	onStartArray() {
		return true;
	}

	bool
	onEndArray() {
		return true;
	}

	// actual yajl callbacks

	static
	int
	nullCallback(void* context) {
		return ((T*)context)->onNull();
	}

	static
	int
	booleanCallback(
		void* context,
		int value
	) {
		return ((T*)context)->onBoolean(value != 0);
	}

	static
	int
	integerCallback(
		void* context,
		long long value
	) {
		return ((T*)context)->onInteger(value);
	}

	static
	int
	doubleCallback(
		void* context,
		double value
	) {
		return ((T*)context)->onDouble(value);
	}

	static
	int
	numberCallback(
		void* context,
		const char* p,
		size_t length
	) {
		return ((T*)context)->onNumber(sl::StringRef(p, length));
	}

	static
	int
	stringCallback(
		void* context,
		const uchar_t* p,
		size_t length
	) {
		return ((T*)context)->onString(sl::StringRef((const char*)p, length));
	}

	static
	int
	startMapCallback(void* context) {
		return ((T*)context)->onStartMap();
	}

	static
	int
	mapKeyCallback(
		void* context,
		const uchar_t* p,
		size_t length
	) {
		return ((T*)context)->onMapKey(sl::StringRef((const char*)p, length));
	}

	static
	int
	endMapCallback(void* context) {
		return ((T*)context)->onEndMap();
	}

	static
	int
	startArrayCallback(void* context) {
		return ((T*)context)->onStartArray();
	}

	static
	int
	endArrayCallback(void* context) {
		return ((T*)context)->onEndArray();
	}

	// simple utility

	bool
	complete(
		yajl_status status,
		const void* p,
		size_t size
	) {
		return
			status == yajl_status_ok ? true :
			status == yajl_status_client_canceled ? false : // the handler should have set error
			failWithYajlError(m_h, p, size);
	}
};

//..............................................................................

} // namespace json
} // namespace axl
