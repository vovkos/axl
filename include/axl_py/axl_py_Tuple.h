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

#define _AXL_PY_TUPLE_H

#include "axl_py_Object.h"

namespace axl {
namespace py {

//..............................................................................

class TupleBase: public ObjectBase {
public:
	bool
	check() const {
		return m_p && PyTuple_Check(m_p);
	}

	bool
	checkExact() const {
		return m_p && PyTuple_CheckExact(m_p);
	}

	static
	bool
	check(PyObject* p) {
		return PyTuple_Check(p);
	}

	static
	bool
	checkExact(PyObject* p) {
		return PyTuple_CheckExact(p);
	}

	bool
	createNew(size_t size) {
		return finalizeCreate(::PyTuple_New(size));
	}

	bool
	createFromVarArgs_va(
		PyObject* arg,
		axl_va_list va
	);

	bool
	createFromVarArgs(
		PyObject* arg,
		...
	) {
		AXL_VA_DECL(va, arg);
		return createFromVarArgs_va(arg, va);
	}

	static
	ObjectImpl<TupleBase>
	fromVarArgs_va(
		PyObject* arg,
		axl_va_list va
	);

	static
	ObjectImpl<TupleBase>
	fromVarArgs(
		PyObject* arg,
		...
	) {
		AXL_VA_DECL(va, arg);
		return fromVarArgs_va(arg, va);
	}

	size_t
	getSize() const {
		ASSERT(m_p);
		return PyTuple_Size(m_p);
	}

	PyObject*
	getItem(size_t index) const {
		ASSERT(m_p);
		return PyTuple_GetItem(m_p, index); // borrowed reference
	}

	bool
	setItem(
		size_t index,
		PyObject* item
	) const;

	bool
	getSlice(
		PyObject** result,
		intptr_t from,
		intptr_t to
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = PyTuple_GetSlice(m_p, from, to)) != NULL);
	}

	ObjectImpl<TupleBase>
	getSlice(
		intptr_t from,
		intptr_t to
	) const;

	bool
	parseArgs_va(
		const char* format,
		axl_va_list va
	) {
		return completeWithLastPyErr(PyArg_VaParse(m_p, format, va) != 0); // (!) PyArg_VaParse returns 1 or 0
	}

	bool
	parseArgs(
		const char* format,
		...
	) {
		AXL_VA_DECL(va, format);
		return parseArgs_va(format, va);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
TupleBase::createFromVarArgs_va(
	PyObject* arg,
	axl_va_list va
) {
	size_t count = va.count<PyObject*>(NULL);
	bool result = createNew(count + 1);
	if (!result)
		return false;

	size_t i = 0;
	do {
		result = setItem(i++, arg);
		if (!result)
			return false;
	} while (arg = axl_va_arg(va, PyObject*));

	return true;
}

inline
ObjectImpl<TupleBase>
TupleBase::fromVarArgs_va(
	PyObject* arg,
	axl_va_list va
) {
	ObjectImpl<TupleBase> result;
	result.createFromVarArgs_va(arg, va);
	return result;
}

inline
bool
TupleBase::setItem(
	size_t index,
	PyObject* item
) const {
	int result = PyTuple_SetItem(m_p, index, item);
	if (result == -1)
		return failWithLastPyErr();

	Py_XINCREF(item); // PyTuple_SetItem steals a reference
	return true;
}

inline
ObjectImpl<TupleBase>
TupleBase::getSlice(
	intptr_t from,
	intptr_t to
) const {
	ObjectImpl<TupleBase> result;
	getSlice(&result, from, to);
	return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<TupleBase> Tuple;
typedef ObjectBorrowedImpl<TupleBase> TupleBorrowed;

//..............................................................................

inline
bool
ObjectBase::callObjArgs_va(
	PyObject** result,
	PyObject* arg,
	axl_va_list va
) const {
	ASSERT(m_p);
	return completeWithLastPyErr((*result = ::PyObject_Call(m_p, Tuple::fromVarArgs_va(arg, va), NULL)) != NULL);
}

//..............................................................................

} // namespace py
} // namespace axl
