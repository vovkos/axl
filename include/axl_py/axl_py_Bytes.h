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

#define _AXL_PY_BYTES_H

#include "axl_py_Object.h"

namespace axl {
namespace py {

//..............................................................................

class BytesBase: public ObjectBase {
public:
	bool
	check() const {
		return m_p && PyBytes_Check(m_p);
	}

	bool
	checkExact() const {
		return m_p && PyBytes_CheckExact(m_p);
	}

	static
	bool
	check(PyObject* p) {
		return PyBytes_Check(p);
	}

	static
	bool
	checkExact(PyObject* p) {
		return PyBytes_CheckExact(p);
	}

	size_t
	getSize() const {
		return PyBytes_Size(m_p);
	}

	bool
	createFromString(const sl::StringRef& string) {
		return finalizeCreate(::PyBytes_FromStringAndSize(string.cp(), string.getLength()));
	}

	bool
	createFromString(
		const void* p,
		size_t size
	) {
		return finalizeCreate(::PyBytes_FromStringAndSize((char*)p, size));
	}

	bool
	createFromFormat_va(
		const char* format,
		axl_va_list va
	) {
		return finalizeCreate(::PyBytes_FromFormatV(format, va));
	}

	bool
	createFromFormat(
		const char* format,
		...
	) {
		AXL_VA_DECL(va, format);
		return createFromFormat_va(format, va);
	}

	bool
	createFromObject(PyObject* object) {
		return finalizeCreate(::PyBytes_FromObject(object));
	}

	static
	ObjectImpl<BytesBase>
	fromString(const sl::StringRef& string);

	static
	ObjectImpl<BytesBase>
	fromString(
		const void* p,
		size_t size
	);

	static
	ObjectImpl<BytesBase>
	fromFormat_va(
		const char* format,
		axl_va_list va
	);

	static
	ObjectImpl<BytesBase>
	fromFormat(
		const char* format,
		...
	) {
		AXL_VA_DECL(va, format);
		return fromFormat_va(format, va);
	}

	static
	ObjectImpl<BytesBase>
	fromObject(PyObject* object);

	bool
	concat(PyObject* part) {
		::PyBytes_Concat(&m_p, part);
		return completeWithLastPyErr(m_p != NULL);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ObjectImpl<BytesBase>
BytesBase::fromString(const sl::StringRef& string) {
	ObjectImpl<BytesBase> result;
	result.createFromString(string);
	return result;
}

inline
ObjectImpl<BytesBase>
BytesBase::fromString(
	const void* p,
	size_t size
) {
	ObjectImpl<BytesBase> result;
	result.createFromString(p, size);
	return result;
}

inline
ObjectImpl<BytesBase>
BytesBase::fromFormat_va(
	const char* format,
	axl_va_list va
) {
	ObjectImpl<BytesBase> result;
	result.createFromFormat_va(format, va);
	return result;
}

inline
ObjectImpl<BytesBase>
BytesBase::fromObject(PyObject* object) {
	ObjectImpl<BytesBase> result;
	result.createFromObject(object);
	return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<BytesBase> Bytes;
typedef ObjectBorrowedImpl<BytesBase> BytesBorrowed;

//..............................................................................

inline
ObjectImpl<BytesBase>
ObjectBase::getBytes() const {
	ObjectImpl<BytesBase> bytes;
	getBytes(&bytes);
	return bytes;
}

//..............................................................................

} // namespace lua
} // namespace axl
