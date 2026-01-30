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

#define _AXL_PY_DICT_H

#include "axl_py_Object.h"

namespace axl {
namespace py {

//..............................................................................

class DictBase: public ObjectBase {
public:
	bool
	check() const {
		return m_p && ::PyDict_Check(m_p);
	}

	bool
	checkExact() const {
		return m_p && ::PyDict_CheckExact(m_p);
	}

	static
	bool
	check(PyObject* p) {
		return PyDict_Check(p);
	}

	static
	bool
	checkExact(PyObject* p) {
		return PyDict_Check(p);
	}

	bool
	create() {
		return finalizeCreate(::PyDict_New());
	}

	bool
	createProxy(PyObject* src) {
		return finalizeCreate(::PyDictProxy_New(src));
	}

	bool
	createCopy(PyObject* src) {
		return finalizeCreate(::PyDict_Copy(src));
	}

	void
	clear() {
		ASSERT(m_p);
		::PyDict_Clear(m_p);
	}

	bool
	contains(PyObject* key) {
		ASSERT(m_p);
		return ::PyDict_Contains(m_p, key) == 1;
	}

	bool
	contains(const sl::StringRef& key) {
		ASSERT(m_p);
		return ::PyDict_ContainsString(m_p, key.sz()) == 1;
	}

	PyObject*
	getItem(PyObject* key) {
		ASSERT(m_p);
		return ::PyDict_GetItem(m_p, key); // borrowed reference
	}

	PyObject*
	getItem(const sl::StringRef& key) {
		ASSERT(m_p);
		return ::PyDict_GetItemString(m_p, key.sz()); // borrowed reference
	}

	bool
	setItem(
		PyObject* key,
		PyObject* value
	) {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyDict_SetItem(m_p, key, value) != -1);
	}

	bool
	setItem(
		const sl::StringRef& key,
		PyObject* value
	) {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyDict_SetItemString(m_p, key.sz(), value) != -1);
	}

	bool
	delItem(PyObject* key) {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyDict_DelItem(m_p, key) != -1);
	}

	bool
	delItem(const sl::StringRef& key) {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyDict_DelItemString(m_p, key.sz()) != -1);
	}

	bool
	merge(
		PyObject* src,
		bool override
	) {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyDict_Merge(m_p, src, override) != -1);
	}

	bool
	update(PyObject* src) {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyDict_Update(m_p, src) != -1);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<DictBase> Dict;
typedef ObjectBorrowedImpl<DictBase> DictBorrowed;

//..............................................................................

} // namespace lua
} // namespace axl
