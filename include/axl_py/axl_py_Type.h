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

#define _AXL_PY_TYPE_H

#include "axl_py_Unicode.h"

namespace axl {
namespace py {

//..............................................................................

class TypeObject: public PyTypeObject {
protected:
	static const PyTypeObject m_headInit;

public:
	TypeObject() {
		memcpy(this, &m_headInit, sizeof(PyTypeObject));
	}

	ulong_t
	getFlags() {
		return PyType_GetFlags(this);
	}

	PyObject*
	getDict() {
		return PyType_GetDict(this);
	}

	bool
	getName(PyObject** name) {
		return completeWithLastPyErr((*name = ::PyType_GetName(this)) != NULL);
	}

	Unicode
	getName();

	bool
	getQualName(PyObject** name) {
		return completeWithLastPyErr((*name = ::PyType_GetQualName(this)) != NULL);
	}

	Unicode
	getQualName();

	bool
	getFullyQualifiedName(PyObject** name) {
		return completeWithLastPyErr((*name = ::PyType_GetFullyQualifiedName(this)) != NULL);
	}

	Unicode
	getFullyQualifiedName();

	bool
	getModuleName(PyObject** name) {
		return completeWithLastPyErr((*name = ::PyType_GetModuleName(this)) != NULL);
	}

	Unicode
	getModuleName();

	void
	setModified() {
		::PyType_Modified(this);
	}

	bool
	ready() {
		return completeWithLastPyErr(::PyType_Ready(this) != -1);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const PyTypeObject TypeObject::m_headInit = {
	PyVarObject_HEAD_INIT(NULL, 0)
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
Unicode
TypeObject::getName() {
	Unicode name;
	getName(&name);
	return name;
}

inline
Unicode
TypeObject::getQualName() {
	Unicode name;
	getQualName(&name);
	return name;
}

inline
Unicode
TypeObject::getFullyQualifiedName() {
	Unicode name;
	getFullyQualifiedName(&name);
	return name;
}

inline
Unicode
TypeObject::getModuleName() {
	Unicode name;
	getModuleName(&name);
	return name;
}

//..............................................................................

} // namespace lua
} // namespace axl
