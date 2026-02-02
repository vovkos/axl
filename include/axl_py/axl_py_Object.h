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

template <typename T>
class ObjectImpl;

template <typename T>
class ObjectBorrowedImpl;

//..............................................................................

class ObjectBase {
protected:
	PyObject* m_p;

public:
	operator PyObject* () const {
		return m_p;
	}

	PyObject*
	p() const {
		return m_p;
	}

#if (PY_VERSION_HEX >= 0x030e0000)
	bool
	isImmortal() const {
		ASSERT(m_p);
		return ::PyUnstable_IsImmortal(m_p) != 0;
	}
#endif

	bool
	isCallable() const {
		ASSERT(m_p);
		return ::PyCallable_Check(m_p) != 0;
	}

	PyTypeObject*
	getType() const {
		ASSERT(m_p);
		return Py_TYPE(m_p);
	}

	bool
	typeCheck(PyTypeObject* type) const {
		ASSERT(m_p);
		return ::PyObject_TypeCheck(m_p, type);
	}

	bool
	typeCheck(PyObject* type) const {
		ASSERT(m_p);
		return ::PyType_Check(type) && ::PyObject_TypeCheck(m_p, (PyTypeObject*)type);
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
	getBuffer(
		Py_buffer* buffer,
		int flags
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyObject_GetBuffer(m_p, buffer, flags) != -1);
	}

	bool
	hasAttr(const sl::StringRef& name) const {
		ASSERT(m_p);
		return ::PyObject_HasAttrString(m_p, name.sz()) != 0;
	}

	bool
	hasAttr(PyObject* name) const {
		ASSERT(m_p);
		return ::PyObject_HasAttr(m_p, name) != 0;
	}

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

#if (PY_VERSION_HEX >= 0x030d0000)
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
#endif

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
	) const;

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
	) const;

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
	) const;

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
	) const;

protected:
	// shouldn't be called from ObjectBorrowedImpl<T>

	bool
	finalizeCreate(PyObject* p) {
		if (!p)
			return failWithLastPyErr();

		Py_XDECREF(this->m_p);
		this->m_p = p;
		return true;
	}
};

//..............................................................................

// owning reference

template <typename T>
class ObjectImpl: public T {
	template <typename T2>
	friend class ObjectImpl;

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

	template <typename T2>
	ObjectImpl(const ObjectBorrowedImpl<T2>& src) {
		construct(src.p());
	}

	ObjectImpl(ObjectImpl&& src) {
		moveConstruct(std::move(src));
	}

	template <typename T2>
	ObjectImpl(ObjectImpl<T2>&& src) {
		moveConstruct(std::move(src));
	}

	~ObjectImpl() {
		Py_XDECREF(this->m_p);
	}

	PyObject**
	operator & () {
		return &this->m_p;
	}

	ObjectImpl&
	operator = (PyObject* p) {
		copy(p);
		return *this;
	}

	ObjectImpl&
	operator = (const ObjectImpl& src) {
		return operator = (src.m_p);
	}

	template <typename T2>
	ObjectImpl&
	operator = (const ObjectImpl<T2>& src) {
		return operator = (src.m_p);
	}

	template <typename T2>
	ObjectImpl&
	operator = (const ObjectBorrowedImpl<T2>& src) {
		return operator = (src.p());
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

	PyObject**
	pp() {
		return &this->m_p;
	}

	void
	attach(PyObject* p) {
		ASSERT(p != this->m_p);
		Py_XDECREF(this->m_p);
		this->m_p = p;
	}

	PyObject*
	detach() {
		PyObject* p = this->m_p;
		this->m_p = NULL;
		return p;
	}

	void
	copy(PyObject* p) {
		if (p == this->m_p)
			return;

		Py_XINCREF(p);
		Py_XDECREF(this->m_p);
		this->m_p = p;
	}

	void
	release() {
		Py_XDECREF(this->m_p);
		this->m_p = NULL;
	}

	template <typename T2>
	void
	move(ObjectImpl<T2>&& src) {
		if (&this->m_p == &src.m_p) // operator & is overloaded
			return;

		Py_XDECREF(this->m_p);
		this->m_p = src.m_p;
		src.m_p = NULL;
	}

protected:
	void
	construct(PyObject* p) {
		Py_XINCREF(p);
		this->m_p = p;
	}

	template <typename T2>
	void
	moveConstruct(ObjectImpl<T2>&& src) {
		this->m_p = src.m_p;
		src.m_p = NULL;
	}
};

//..............................................................................

// borrowed reference

template <typename T>
class ObjectBorrowedImpl: public T {
public:
	ObjectBorrowedImpl(PyObject* p = NULL) {
		this->m_p = p;
	}

	template <typename T2>
	ObjectBorrowedImpl(const ObjectImpl<T2>& src) {
		this->m_p = src.p();
	}

	template <typename T2>
	ObjectBorrowedImpl(const ObjectBorrowedImpl<T2>& src) {
		this->m_p = src.p();
	}

	ObjectBorrowedImpl&
	operator = (PyObject* p) {
		this->m_p = p;
		return *this;
	}

	template <typename T2>
	ObjectBorrowedImpl&
	operator = (const ObjectImpl<T2>& src) {
		return operator = (src.p());
	}

	template <typename T2>
	ObjectBorrowedImpl&
	operator = (const ObjectBorrowedImpl<T2>& src) {
		return operator = (src.p());
	}

private:
	void
	operator & () {}
};

//..............................................................................

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

#if (PY_VERSION_HEX >= 0x030d0000)

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

#endif

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
ObjectBase::callObjArgs(
	PyObject* arg,
	...
) const {
	AXL_VA_DECL(va, arg);
	return callObjArgs_va(arg, va);
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
bool
ObjectBase::callMethodObjArgs_va(
	PyObject** result,
	PyObject* name,
	PyObject* arg,
	axl_va_list va
) const {
	ObjectImpl<ObjectBase> method = getAttr(name);
	return method ? method.callObjArgs_va(result, arg, va) : false;
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

inline
ObjectImpl<ObjectBase>
ObjectBase::callMethodObjArgs(
	PyObject* name,
	PyObject* arg,
	...
) const {
	AXL_VA_DECL(va, arg);
	return callMethodObjArgs_va(name, arg, va);
}

inline
ObjectImpl<ObjectBase>
ObjectBase::callMethodObjArgs(
	const sl::StringRef& name,
	PyObject* arg,
	...
) const {
	AXL_VA_DECL(va, arg);
	return callMethodObjArgs_va(name, arg, va);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<ObjectBase> Object;
typedef ObjectBorrowedImpl<ObjectBase> ObjectBorrowed;

//..............................................................................

} // namespace lua
} // namespace axl
