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

#define _AXL_PY_LONG_H

#include "axl_py_Object.h"

namespace axl {
namespace py {

//..............................................................................

class LongBase: public ObjectBase {
public:
	bool
	check() const {
		return m_p && PyLong_Check(m_p);
	}

	bool
	checkExact() const {
		return m_p && PyLong_CheckExact(m_p);
	}

	bool
	isZero() const {
		ASSERT(m_p);
		return ::PyLong_IsZero(m_p);
	}

	bool
	isPositive() const {
		ASSERT(m_p);
		return ::PyLong_IsPositive(m_p);
	}

	bool
	isNegative() const {
		ASSERT(m_p);
		return ::PyLong_IsNegative(m_p);
	}

	bool
	createFromLong(long value) {
		return finalizeCreate(::PyLong_FromLong(value));
	}

	bool
	createFromUnsignedLong(ulong_t value) {
		return finalizeCreate(::PyLong_FromUnsignedLong(value));
	}

	bool
	createFromSizeT(size_t value) {
		return finalizeCreate(::PyLong_FromSize_t(value));
	}

	bool
	createFromLongLong(long long value) {
		return finalizeCreate(::PyLong_FromLongLong(value));
	}

	bool
	createFromUnsignedLongLong(unsigned long long value) {
		return finalizeCreate(::PyLong_FromUnsignedLongLong(value));
	}

	bool
	createFromInt32(int32_t value) {
		return finalizeCreate(::PyLong_FromInt32(value));
	}

	bool
	createFromUInt32(uint32_t value) {
		return finalizeCreate(::PyLong_FromUInt32(value));
	}

	bool
	createFromInt64(int64_t value) {
		return finalizeCreate(::PyLong_FromInt64(value));
	}

	bool
	createFromUInt64(uint64_t value) {
		return finalizeCreate(::PyLong_FromUInt64(value));
	}

	bool
	createFromDouble(double value) {
		return finalizeCreate(::PyLong_FromDouble(value));
	}

	bool
	createFromString(
		const char* string,
		const char** end = NULL,
		int base = 0
	) {
		return finalizeCreate(::PyLong_FromString(string, (char**)end, base));
	}

	bool
	createFromUnicode(
		PyObject* unicode,
		int base = 0
	) {
		return finalizeCreate(::PyLong_FromUnicodeObject(unicode, base));
	}

	bool
	createFromVoidPtr(void* p) {
		return finalizeCreate(::PyLong_FromVoidPtr(p));
	}

	bool
	createFromNativeBytes(
		const void* p,
		size_t size,
		int flags = Py_ASNATIVEBYTES_DEFAULTS
	) {
		return finalizeCreate(::PyLong_FromNativeBytes(p, size, flags));
	}

	bool
	createFromUnsignedNativeBytes(
		const void* p,
		size_t size,
		int flags = Py_ASNATIVEBYTES_DEFAULTS
	) {
		return finalizeCreate(::PyLong_FromUnsignedNativeBytes(p, size, flags));
	}

	static
	ObjectImpl<LongBase>
	fromLong(long value);

	static
	ObjectImpl<LongBase>
	fromUnsignedLong(ulong_t value);

	static
	ObjectImpl<LongBase>
	fromSizeT(size_t value);

	static
	ObjectImpl<LongBase>
	fromLongLong(long long value);

	static
	ObjectImpl<LongBase>
	fromUnsignedLongLong(unsigned long long value);

	static
	ObjectImpl<LongBase>
	fromInt32(int32_t value);

	static
	ObjectImpl<LongBase>
	fromUInt32(uint32_t value);

	static
	ObjectImpl<LongBase>
	fromInt64(int64_t value);

	static
	ObjectImpl<LongBase>
	fromUInt64(uint64_t value);

	static
	ObjectImpl<LongBase>
	fromDouble(double value);

	static
	ObjectImpl<LongBase>
	fromString(
		const char* string,
		const char** end = NULL,
		int base = 0
	);

	static
	ObjectImpl<LongBase>
	fromUnicode(
		PyObject* unicode,
		int base = 0
	);

	static
	ObjectImpl<LongBase>
	fromVoidPtr(void* p);

	static
	ObjectImpl<LongBase>
	fromNativeBytes(
		const void* p,
		size_t size,
		int flags = Py_ASNATIVEBYTES_DEFAULTS
	);

	static
	ObjectImpl<LongBase>
	fromUnsignedNativeBytes(
		const void* p,
		size_t size,
		int flags = Py_ASNATIVEBYTES_DEFAULTS
	);

	long
	getLong() const {
		ASSERT(m_p);
		return ::PyLong_AsLong(m_p);
	}

	ulong_t
	getUnsignedLong() const {
		ASSERT(m_p);
		return ::PyLong_AsUnsignedLong(m_p);
	}

	int
	getInt() const {
		ASSERT(m_p);
		return ::PyLong_AsInt(m_p);
	}

	long long
	getLongLong() const {
		ASSERT(m_p);
		return ::PyLong_AsLongLong(m_p);
	}

	unsigned long long
	getUnsignedLongLong() const {
		ASSERT(m_p);
		return ::PyLong_AsUnsignedLongLong(m_p);
	}

	size_t
	getSizeT() const {
		ASSERT(m_p);
		return ::PyLong_AsSize_t(m_p);
	}

	double
	getDouble() const {
		ASSERT(m_p);
		return ::PyLong_AsDouble(m_p);
	}

	void*
	getVoidPtr() const {
		ASSERT(m_p);
		return ::PyLong_AsVoidPtr(m_p);
	}

	bool
	getInt32(int32_t* value) const;

	int32_t
	getInt32() const;

	bool
	getUInt32(uint32_t* value) const;

	uint32_t
	getUInt32() const;

	bool
	getInt64(int64_t* value) const;

	int64_t
	getInt64() const;

	bool
	getUInt64(uint64_t* value) const;

	uint64_t
	getUInt64() const;

	size_t
	getNativeBytes(
		void* p,
		size_t size,
		int flags = Py_ASNATIVEBYTES_DEFAULTS
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ObjectImpl<LongBase>
LongBase::fromLong(long value) {
	ObjectImpl<LongBase> result;
	result.createFromLong(value);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromUnsignedLong(ulong_t value) {
	ObjectImpl<LongBase> result;
	result.createFromUnsignedLong(value);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromSizeT(size_t value) {
	ObjectImpl<LongBase> result;
	result.createFromSizeT(value);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromLongLong(long long value) {
	ObjectImpl<LongBase> result;
	result.createFromLongLong(value);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromUnsignedLongLong(unsigned long long value) {
	ObjectImpl<LongBase> result;
	result.createFromUnsignedLongLong(value);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromInt32(int32_t value) {
	ObjectImpl<LongBase> result;
	result.createFromInt32(value);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromUInt32(uint32_t value) {
	ObjectImpl<LongBase> result;
	result.createFromUInt32(value);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromInt64(int64_t value) {
	ObjectImpl<LongBase> result;
	result.createFromInt64(value);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromUInt64(uint64_t value) {
	ObjectImpl<LongBase> result;
	result.createFromUInt64(value);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromDouble(double value) {
	ObjectImpl<LongBase> result;
	result.createFromDouble(value);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromString(
	const char* string,
	const char** end,
	int base
) {
	ObjectImpl<LongBase> result;
	result.createFromString(string, end, base);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromUnicode(
	PyObject* unicode,
	int base
) {
	ObjectImpl<LongBase> result;
	result.createFromUnicode(unicode, base);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromVoidPtr(void* p) {
	ObjectImpl<LongBase> result;
	result.createFromVoidPtr(p);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromNativeBytes(
	const void* p,
	size_t size,
	int flags
) {
	ObjectImpl<LongBase> result;
	result.createFromNativeBytes(p, size, flags);
	return result;
}

inline
ObjectImpl<LongBase>
LongBase::fromUnsignedNativeBytes(
	const void* p,
	size_t size,
	int flags
) {
	ObjectImpl<LongBase> result;
	result.createFromUnsignedNativeBytes(p, size, flags);
	return result;
}

inline
bool
LongBase::getInt32(int32_t* value) const {
	ASSERT(m_p);
	int result = ::PyLong_AsInt32(m_p, value);
	return completeWithLastPyErr(result != -1);
}

inline
int32_t
LongBase::getInt32() const {
	int32_t value;
	getInt32(&value);
	return value;
}

inline
bool
LongBase::getUInt32(uint32_t* value) const {
	ASSERT(m_p);
	int result = ::PyLong_AsUInt32(m_p, value);
	return completeWithLastPyErr(result != -1);
}

inline
uint32_t
LongBase::getUInt32() const {
	uint32_t value;
	getUInt32(&value);
	return value;
}

inline
bool
LongBase::getInt64(int64_t* value) const {
	ASSERT(m_p);
	int result = ::PyLong_AsInt64(m_p, value);
	return completeWithLastPyErr(result != -1);
}

inline
int64_t
LongBase::getInt64() const {
	int64_t value;
	getInt64(&value);
	return value;
}

inline
bool
LongBase::getUInt64(uint64_t* value) const {
	ASSERT(m_p);
	int result = ::PyLong_AsUInt64(m_p, value);
	return completeWithLastPyErr(result != -1);
}

inline
uint64_t
LongBase::getUInt64() const {
	uint64_t value;
	getUInt64(&value);
	return value;
}

inline
size_t
LongBase::getNativeBytes(
	void* p,
	size_t size,
	int flags
) {
	ASSERT(m_p);
	Py_ssize_t result = ::PyLong_AsNativeBytes(m_p, p, size, flags);
	return completeWithLastPyErr<size_t>(result, (Py_ssize_t)-1);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<LongBase> Long;

//..............................................................................

} // namespace lua
} // namespace axl
