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

#define _AXL_PY_UNICODE_H

#include "axl_py_Bytes.h"

namespace axl {
namespace py {

//..............................................................................

class UnicodeBase: public ObjectBase {
public:
	bool
	check() const {
		return m_p && ::PyUnicode_Check(m_p);
	}

	bool
	checkExact() const {
		return m_p && ::PyUnicode_CheckExact(m_p);
	}

	static
	bool
	check(PyObject* p) {
		return PyUnicode_Check(p);
	}

	static
	bool
	checkExact(PyObject* p) {
		return PyUnicode_CheckExact(p);
	}

	size_t
	getLength() const {
		return ::PyUnicode_GetLength(m_p);
	}

	PyUnicode_Kind
	getUnicodeKind() const {
		return (PyUnicode_Kind)PyUnicode_KIND(m_p);
	}

	Py_UCS1*
	getUcs1() const {
		return PyUnicode_1BYTE_DATA(m_p);
	}

	Py_UCS2*
	getUcs2() const {
		return PyUnicode_2BYTE_DATA(m_p);
	}

	Py_UCS4*
	getUcs4() const {
		return PyUnicode_4BYTE_DATA(m_p);
	}

	void*
	getData() const {
		return ::PyUnicode_DATA(m_p);
	}

	bool
	isIdentifier() const {
		return ::PyUnicode_IsIdentifier(m_p);
	}

	bool
	isAscii() const {
		return ::PyUnicode_IS_ASCII(m_p);
	}

	bool
	createNew(
		size_t size,
		Py_UCS4 maxChar
	) {
		return finalizeCreate(::PyUnicode_New(size, maxChar));
	}

	bool
	createFromKindAndData(
		PyUnicode_Kind kind,
		const void* data,
		size_t size
	) {
		return finalizeCreate(::PyUnicode_FromKindAndData(kind, data, size));
	}

	bool
	createFromString(const char* string) {
		return finalizeCreate(::PyUnicode_FromString(string));
	}

	bool
	createFromString(const sl::StringRef& string) {
		return finalizeCreate(::PyUnicode_FromStringAndSize(string.cp(), string.getLength()));
	}

	bool
	createFromFormat_va(
		const char* format,
		axl_va_list va
	) {
		return finalizeCreate(::PyUnicode_FromFormatV(format, va));
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
		return finalizeCreate(::PyUnicode_FromObject(object));
	}

	bool
	createJoin(
		PyObject* separator,
		PyObject* sequence
	) {
		return finalizeCreate(::PyUnicode_Join(separator, sequence));
	}

	bool
	createJoin(
		const sl::StringRef& separator,
		PyObject* sequence
	) {
		return finalizeCreate(::PyUnicode_Join(fromString(separator), sequence));
	}

	bool
	createJoin(PyObject* sequence) {
		return createJoin(sl::StringRef(), sequence);
	}

	static
	ObjectImpl<UnicodeBase>
	fromKindAndData(
		PyUnicode_Kind kind,
		const void* data,
		size_t size
	);

	static
	ObjectImpl<UnicodeBase>
	fromString(const char* string);

	static
	ObjectImpl<UnicodeBase>
	fromString(const sl::StringRef& string);

	static
	ObjectImpl<UnicodeBase>
	fromFormat_va(
		const char* format,
		axl_va_list va
	);

	static
	ObjectImpl<UnicodeBase>
	fromFormat(
		const char* format,
		...
	) {
		AXL_VA_DECL(va, format);
		return fromFormat_va(format, va);
	}

	static
	ObjectImpl<UnicodeBase>
	fromObject(PyObject* object);

	static
	ObjectImpl<UnicodeBase>
	join(
		PyObject* separator,
		PyObject* sequence
	);

	static
	ObjectImpl<UnicodeBase>
	join(
		const sl::StringRef& separator,
		PyObject* sequence
	) {
		return join(fromString(separator), sequence);
	}

	static
	ObjectImpl<UnicodeBase>
	join(PyObject* sequence) {
		return join(sl::StringRef(), sequence);
	}

	bool
	append(PyObject* object);

	bool
	getSubstring(
		PyObject** result,
		size_t index,
		size_t length
	) const;

	ObjectImpl<UnicodeBase>
	getSubstring(
		size_t index,
		size_t length
	) const;

	size_t
	getUcs4(
		Py_UCS4* buffer,
		size_t length
	) const;

	sl::StringRef
	getUtf8() const;

	bool
	getUtf8String(PyObject** result) const;

	ObjectImpl<BytesBase>
	getUtf8String() const;

	bool
	getUtf16String(PyObject** result) const;

	ObjectImpl<BytesBase>
	getUtf16String() const;

	bool
	getUtf32String(PyObject** result) const;

	ObjectImpl<BytesBase>
	getUtf32String() const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ObjectImpl<UnicodeBase>
UnicodeBase::fromKindAndData(
	PyUnicode_Kind kind,
	const void* data,
	size_t size
) {
	ObjectImpl<UnicodeBase> result;
	result.createFromKindAndData(kind, data, size);
	return result;
}

inline
ObjectImpl<UnicodeBase>
UnicodeBase::fromString(const char* string) {
	ObjectImpl<UnicodeBase> result;
	result.createFromString(string);
	return result;
}

inline
ObjectImpl<UnicodeBase>
UnicodeBase::fromString(const sl::StringRef& string) {
	ObjectImpl<UnicodeBase> result;
	result.createFromString(string);
	return result;
}

inline
ObjectImpl<UnicodeBase>
UnicodeBase::fromFormat_va(
	const char* format,
	axl_va_list va
) {
	ObjectImpl<UnicodeBase> result;
	result.createFromFormat_va(format, va);
	return result;
}

inline
ObjectImpl<UnicodeBase>
UnicodeBase::fromObject(PyObject* object) {
	ObjectImpl<UnicodeBase> result;
	result.createFromObject(object);
	return result;
}

inline
ObjectImpl<UnicodeBase>
UnicodeBase::join(
	PyObject* separator,
	PyObject* sequence
) {
	ObjectImpl<UnicodeBase> result;
	result.createJoin(separator, sequence);
	return result;
}

inline
bool
UnicodeBase::append(PyObject* object) {
	ASSERT(m_p);
	::PyUnicode_Append(&m_p, object);
	return completeWithLastPyErr(m_p != NULL);
}

inline
bool
UnicodeBase::getSubstring(
	PyObject** result,
	size_t index,
	size_t length
) const {
	ASSERT(m_p);
	PyObject* slice = ::PyUnicode_Substring(m_p, index, index + length);
	if (!slice)
		return failWithLastPyErr();

	*result = slice;
	return true;
}

inline
ObjectImpl<UnicodeBase>
UnicodeBase::getSubstring(
	size_t index,
	size_t length
) const {
	ObjectImpl<UnicodeBase> slice;
	getSubstring(&slice, index, length);
	return slice;
}

inline
size_t
UnicodeBase::getUcs4(
	Py_UCS4* buffer,
	size_t length
) const {
	ASSERT(m_p);
	Py_UCS4* p = ::PyUnicode_AsUCS4(m_p, buffer, length, 0);
	if (!p)
		return failWithLastPyErr<size_t>(-1);

	return getLength();
}

inline
sl::StringRef
UnicodeBase::getUtf8() const {
	ASSERT(m_p);
	Py_ssize_t length;
	const char* p = ::PyUnicode_AsUTF8AndSize(m_p, &length);
	if (!p)
		return failWithLastPyErr<sl::StringRef>(sl::StringRef());

	return sl::StringRef(p, length);
}

inline
bool
UnicodeBase::getUtf8String(PyObject** result) const {
	ASSERT(m_p);
	PyObject* p = ::PyUnicode_AsUTF8String(m_p);
	if (!p)
		return failWithLastPyErr<size_t>(-1);

	*result = p;
	return true;
}

inline
ObjectImpl<BytesBase>
UnicodeBase::getUtf8String() const {
	ObjectImpl<BytesBase> string;
	getUtf8String(&string);
	return string;
}

inline
bool
UnicodeBase::getUtf16String(PyObject** result) const {
	ASSERT(m_p);
	PyObject* p = ::PyUnicode_AsUTF16String(m_p);
	if (!p)
		return failWithLastPyErr<size_t>(-1);

	*result = p;
	return true;
}

inline
ObjectImpl<BytesBase>
UnicodeBase::getUtf16String() const {
	ObjectImpl<BytesBase> string;
	getUtf16String(&string);
	return string;
}

inline
bool
UnicodeBase::getUtf32String(PyObject** result) const {
	ASSERT(m_p);
	PyObject* p = ::PyUnicode_AsUTF32String(m_p);
	if (!p)
		return failWithLastPyErr<size_t>(-1);

	*result = p;
	return true;
}

inline
ObjectImpl<BytesBase>
UnicodeBase::getUtf32String() const {
	ObjectImpl<BytesBase> string;
	getUtf32String(&string);
	return string;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<UnicodeBase> Unicode;
typedef ObjectBorrowedImpl<UnicodeBase> UnicodeBorrowed;

//..............................................................................

inline
ObjectImpl<UnicodeBase>
ObjectBase::getStr() const {
	ObjectImpl<UnicodeBase> string;
	getStr(&string);
	return string;
}

inline
ObjectImpl<UnicodeBase>
ObjectBase::getRepr() const {
	ObjectImpl<UnicodeBase> string;
	getRepr(&string);
	return string;
}

inline
bool
ObjectBase::callMethod(
	PyObject** result,
	const sl::StringRef& name
) const {
	return callMethod(result, Unicode::fromString(name));
}

inline
ObjectImpl<ObjectBase>
ObjectBase::callMethod(const sl::StringRef& name) const {
	return callMethod(Unicode::fromString(name));
}

inline
bool
ObjectBase::callMethod(
	PyObject** result,
	const sl::StringRef& name,
	PyObject* arg
) const {
	return callMethod(result, Unicode::fromString(name), arg);
}

inline
ObjectImpl<ObjectBase>
ObjectBase::callMethod(
	const sl::StringRef& name,
	PyObject* arg
) const {
	return callMethod(Unicode::fromString(name), arg);
}

inline
bool
ObjectBase::callMethodObjArgs_va(
	PyObject** result,
	const sl::StringRef& name,
	PyObject* arg,
	axl_va_list va
) const {
	return callMethodObjArgs_va(result, Unicode::fromString(name), arg, va);
}

inline
ObjectImpl<ObjectBase>
ObjectBase::callMethodObjArgs_va(
	const sl::StringRef& name,
	PyObject* arg,
	axl_va_list va
) const {
	return callMethodObjArgs_va(Unicode::fromString(name), arg, va);
}

//..............................................................................

} // namespace lua
} // namespace axl
