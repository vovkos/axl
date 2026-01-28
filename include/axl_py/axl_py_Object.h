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

#define _AXL_PY_OBJECT_H

#include "axl_py_Error.h"

namespace axl {
namespace py {

class UnicodeBase;
class BytesBase;

//..............................................................................

template <typename T>
class ObjectImpl: public T {
public:
	ObjectImpl(PyObject* p = NULL) {
		construct(p);
	}

	ObjectImpl(const ObjectImpl& src) {
		construct(src.m_p);
	}

	template <typename T2>
	ObjectImpl(const ObjectImpl<T2>& src) {
		construct(src.m_p);
	}

	ObjectImpl(ObjectImpl&& src) {
		moveConstruct(std::move(src));
	}

	template <typename T2>
	ObjectImpl(ObjectImpl<T2>&& src) {
		moveConstruct(std::move(src));
	}

	~ObjectImpl() {
		Py_XDECREF(m_p);
	}

	PyObject**
	operator & () {
		ASSERT(!m_p);
		return &m_p;
	}

	ObjectImpl&
	operator = (PyObject* p) {
		copy(p);
		return *this;
	}

	ObjectImpl&
	operator = (const ObjectImpl& src) {
		copy(src.m_p);
		return *this;
	}

	template <typename T2>
	ObjectImpl&
	operator = (const ObjectImpl<T2>& src) {
		copy(src.m_p);
		return *this;
	}

	ObjectImpl&
	operator = (ObjectImpl&& src) {
		move(std::move(src));
		return *this;
	}

	template <typename T2>
	ObjectImpl&
	operator = (ObjectImpl<T2>&& src) {
		move(std::move(src));
		return *this;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ObjectBase {
protected:
	PyObject* m_p;

public:
	operator PyObject* () const {
		return m_p;
	}

	PyObject**
	p() {
		return &m_p;
	}

	void
	attach(PyObject* p);

	PyObject*
	detach();

	void
	copy(PyObject* p);

	void
	release() {
		Py_XDECREF(m_p);
		m_p = NULL;
	}

	void
	move(ObjectBase&& src);

	PyTypeObject*
	getType() const {
		ASSERT(m_p);
		return Py_TYPE(m_p);
	}

	bool
	getStr(PyObject** result) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = ::PyObject_Str(m_p)) != NULL);
	}

	ObjectImpl<UnicodeBase>
	getStr() const;

	bool
	getRepr(PyObject** result) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = ::PyObject_Repr(m_p)) != NULL);
	}

	ObjectImpl<UnicodeBase>
	getRepr() const;

	bool
	getBytes(PyObject** result) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = ::PyObject_Bytes(m_p)) != NULL);
	}

	ObjectImpl<BytesBase>
	getBytes() const;

	bool
	getAttr(
		PyObject** result,
		const sl::StringRef& name
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = ::PyObject_GetAttrString(m_p, name.sz())) != NULL);
	}

	ObjectImpl<ObjectBase>
	getAttr(const sl::StringRef& name) const;

	bool
	getAttr(
		PyObject** result,
		PyObject* name
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = ::PyObject_GetAttr(m_p, name)) != NULL);
	}

	ObjectImpl<ObjectBase>
	getAttr(PyObject* name) const;

	bool
	getOptionalAttr(
		PyObject** result,
		const sl::StringRef& name
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyObject_GetOptionalAttrString(m_p, name.sz(), result) != -1);
	}

	ObjectImpl<ObjectBase>
	getOptionalAttr(const sl::StringRef& name) const;

	bool
	getOptionalAttr(
		PyObject** result,
		PyObject* name
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyObject_GetOptionalAttr(m_p, name, result) != -1);
	}

	ObjectImpl<ObjectBase>
	getOptionalAttr(PyObject* name) const;

	bool
	callEx(
		PyObject** result,
		PyObject* args,
		PyObject* kwargs = NULL
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = ::PyObject_Call(m_p, args, kwargs)) != NULL);
	}

	ObjectImpl<ObjectBase>
	callEx(
		PyObject* args,
		PyObject* kwargs = NULL
	) const;

	bool
	call(PyObject** result) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = ::PyObject_CallNoArgs(m_p)) != NULL);
	}

	ObjectImpl<ObjectBase>
	call() const;

	bool
	call(
		PyObject** result,
		PyObject* arg
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = ::PyObject_CallOneArg(m_p, arg)) != NULL);
	}

	ObjectImpl<ObjectBase>
	call(PyObject* arg) const;

	bool
	callObjArgs_va(
		PyObject** result,
		PyObject* arg,
		axl_va_list va
	) const;

	bool
	callObjArgs(
		PyObject** result,
		PyObject* arg,
		...
	) const {
		AXL_VA_DECL(va, arg);
		return callObjArgs_va(result, arg, va);
	}

	ObjectImpl<ObjectBase>
	callObjArgs_va(
		PyObject* arg,
		axl_va_list va
	) const;

	ObjectImpl<ObjectBase>
	callObjArgs(
		PyObject* arg,
		...
	) const {
		AXL_VA_DECL(va, arg);
		return callObjArgs_va(arg, va);
	}

	bool
	callMethod(
		PyObject** result,
		PyObject* name
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = ::PyObject_CallMethodNoArgs(m_p, name)) != NULL);
	}

	bool
	callMethod(
		PyObject** result,
		const sl::StringRef& name
	) const;

	ObjectImpl<ObjectBase>
	callMethod(PyObject* name) const;

	ObjectImpl<ObjectBase>
	callMethod(const sl::StringRef& name) const;

	bool
	callMethod(
		PyObject** result,
		PyObject* name,
		PyObject* arg
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = ::PyObject_CallMethodOneArg(m_p, arg, name)) != NULL);
	}

	bool
	callMethod(
		PyObject** result,
		const sl::StringRef& name,
		PyObject* arg
	) const;

	ObjectImpl<ObjectBase>
	callMethod(
		PyObject* name,
		PyObject* arg
	) const;

	ObjectImpl<ObjectBase>
	callMethod(
		const sl::StringRef& name,
		PyObject* arg
	) const;

	bool
	callMethodObjArgs_va(
		PyObject** result,
		PyObject* name,
		PyObject* arg,
		axl_va_list va
	) const {
		ObjectImpl<ObjectBase> method = getAttr(name);
		return method ? method.callObjArgs_va(result, arg, va) : false;
	}

	bool
	callMethodObjArgs(
		PyObject** result,
		PyObject* name,
		PyObject* arg,
		...
	) const {
		AXL_VA_DECL(va, arg);
		return callMethodObjArgs_va(result, name, arg, va);
	}

	bool
	callMethodObjArgs_va(
		PyObject** result,
		const sl::StringRef& name,
		PyObject* arg,
		axl_va_list va
	) const;

	bool
	callMethodObjArgs(
		PyObject** result,
		const sl::StringRef& name,
		PyObject* arg,
		...
	) const {
		AXL_VA_DECL(va, arg);
		return callMethodObjArgs_va(result, name, arg, va);
	}

	ObjectImpl<ObjectBase>
	callMethodObjArgs_va(
		PyObject* name,
		PyObject* arg,
		axl_va_list va
	) const;

	ObjectImpl<ObjectBase>
	callMethodObjArgs(
		PyObject* name,
		PyObject* arg,
		...
	) const {
		AXL_VA_DECL(va, arg);
		return callMethodObjArgs_va(name, arg, va);
	}

	ObjectImpl<ObjectBase>
	callMethodObjArgs_va(
		const sl::StringRef& name,
		PyObject* arg,
		axl_va_list va
	) const;

	ObjectImpl<ObjectBase>
	callMethodObjArgs(
		const sl::StringRef& name,
		PyObject* arg,
		...
	) const {
		AXL_VA_DECL(va, arg);
		return callMethodObjArgs_va(name, arg, va);
	}

protected:
	void
	construct(PyObject* p) {
		Py_XINCREF(p);
		m_p = p;
	}

	void
	moveConstruct(ObjectBase&& src) {
		m_p = src.m_p;
		src.m_p = NULL;
	}

	bool
	finalizeCreate(PyObject* p) {
		if (!p)
			return failWithLastPyErr();

		attach(p);
		return true;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
ObjectBase::attach(PyObject* p) {
	ASSERT(p != m_p);
	Py_XDECREF(m_p);
	m_p = p;
}

inline
PyObject*
ObjectBase::detach() {
	PyObject* p = m_p;
	m_p = NULL;
	return p;
}

inline
void
ObjectBase::copy(PyObject* p) {
	if (p == m_p)
		return;

	Py_XINCREF(p);
	Py_XDECREF(m_p);
	m_p = p;
}

inline
void
ObjectBase::move(ObjectBase&& src) {
	if (this == &src)
		return;

	Py_XDECREF(m_p);
	m_p = src.m_p;
	src.m_p = NULL;
}

inline
ObjectImpl<ObjectBase>
ObjectBase::getAttr(const sl::StringRef& name) const {
	ObjectImpl<ObjectBase> value;
	getAttr(&value, name);
	return value;
}

inline
ObjectImpl<ObjectBase>
ObjectBase::getAttr(PyObject* name) const {
	ObjectImpl<ObjectBase> value;
	getAttr(&value, name);
	return value;
}

inline
ObjectImpl<ObjectBase>
ObjectBase::getOptionalAttr(const sl::StringRef& name) const {
	ObjectImpl<ObjectBase> value;
	getOptionalAttr(&value, name);
	return value;
}

inline
ObjectImpl<ObjectBase>
ObjectBase::getOptionalAttr(PyObject* name) const {
	ObjectImpl<ObjectBase> value;
	getOptionalAttr(&value, name);
	return value;
}

inline
ObjectImpl<ObjectBase>
ObjectBase::callEx(
	PyObject* args,
	PyObject* kwargs
) const {
	ObjectImpl<ObjectBase> result;
	callEx(&result, args, kwargs);
	return result;
}

inline
ObjectImpl<ObjectBase>
ObjectBase::call() const {
	ObjectImpl<ObjectBase> result;
	call(&result);
	return result;
}

inline
ObjectImpl<ObjectBase>
ObjectBase::call(PyObject* arg) const {
	ObjectImpl<ObjectBase> result;
	call(&result, arg);
	return result;
}

inline
ObjectImpl<ObjectBase>
ObjectBase::callObjArgs_va(
	PyObject* arg,
	axl_va_list va
) const {
	ObjectImpl<ObjectBase> result;
	callObjArgs_va(&result, arg, va);
	return result;
}

inline
ObjectImpl<ObjectBase>
ObjectBase::callMethod(PyObject* name) const {
	ObjectImpl<ObjectBase> result;
	callMethod(&result, name);
	return result;
}

inline
ObjectImpl<ObjectBase>
ObjectBase::callMethod(
	PyObject* name,
	PyObject* arg
) const {
	ObjectImpl<ObjectBase> result;
	callMethod(&result, name, arg);
	return result;
}

inline
ObjectImpl<ObjectBase>
ObjectBase::callMethodObjArgs_va(
	PyObject* name,
	PyObject* arg,
	axl_va_list va
) const {
	ObjectImpl<ObjectBase> result;
	callMethodObjArgs_va(&result, name, arg, va);
	return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<ObjectBase> Object;

//..............................................................................

} // namespace lua
} // namespace axl
