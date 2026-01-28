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

#define _AXL_PY_LIST_H

#include "axl_py_Tuple.h"

namespace axl {
namespace py {

//..............................................................................

class ListBase: public ObjectBase {
public:
	bool
	check() const {
		return m_p && PyList_Check(m_p);
	}

	bool
	checkExact() const {
		return m_p && PyList_CheckExact(m_p);
	}

	bool
	createNew(size_t size) {
		return finalizeCreate(::PyList_New(size));
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
	ObjectImpl<ListBase>
	fromVarArgs_va(
		PyObject* arg,
		axl_va_list va
	);

	static
	ObjectImpl<ListBase>
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
		return PyList_Size(m_p);
	}

	PyObject*
	getItem(size_t index) const {
		ASSERT(m_p);
		return PyList_GetItem(m_p, index); // borrowed reference
	}

	bool
	setItem(
		size_t index,
		PyObject* item
	) const;

	bool
	insert(
		size_t index,
		PyObject* item
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr(PyList_Insert(m_p, index, item) != -1);
	}

	bool
	append(PyObject* item) const {
		ASSERT(m_p);
		return completeWithLastPyErr(PyList_Append(m_p, item) != -1);
	}

	bool
	getSlice(
		PyObject** result,
		intptr_t from,
		intptr_t to
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = PyList_GetSlice(m_p, from, to)) != NULL);
	}

	ObjectImpl<ListBase>
	getSlice(
		intptr_t from,
		intptr_t to
	) const;

	bool
	getTuple(PyObject** result) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = PyList_AsTuple(m_p)) != NULL);
	}

	ObjectImpl<Tuple>
	getTuple() const;

	bool
	clear() const {
		ASSERT(m_p);
		return completeWithLastPyErr(PyList_Clear(m_p) != -1);
	}

	bool
	sort() const {
		ASSERT(m_p);
		return completeWithLastPyErr(PyList_Sort(m_p) != -1);
	}

	bool
	reverse() const {
		ASSERT(m_p);
		return completeWithLastPyErr(PyList_Reverse(m_p) != -1);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
ListBase::createFromVarArgs_va(
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
ObjectImpl<ListBase>
ListBase::fromVarArgs_va(
	PyObject* arg,
	axl_va_list va
) {
	ObjectImpl<ListBase> result;
	result.createFromVarArgs_va(arg, va);
	return result;
}

inline
bool
ListBase::setItem(
	size_t index,
	PyObject* item
) const {
	int result = PyList_SetItem(m_p, index, item);
	if (result == -1)
		return failWithLastPyErr();

	Py_XINCREF(item); // PyList_SetItem steals a reference
	return true;
}

ObjectImpl<ListBase>
ListBase::getSlice(
	intptr_t from,
	intptr_t to
) const {
	ObjectImpl<ListBase> result;
	getSlice(&result, from, to);
	return result;
}

ObjectImpl<Tuple>
ListBase::getTuple() const {
	ObjectImpl<Tuple> result;
	getTuple(&result);
	return result;
}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<ListBase> List;

//..............................................................................

} // namespace lua
} // namespace axl
