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

	bool
	check() {
		return ::PyType_Check(this);
	}

	bool
	checkExact() {
		return ::PyType_CheckExact(this);
	}

	static
	bool
	check(PyObject* p) {
		return ::PyType_Check(p);
	}

	static
	bool
	checkExact(PyObject* p) {
		return ::PyType_CheckExact(p);
	}

	operator PyObject* () {
		return (PyObject*)this;
	}

	ulong_t
	getFlags() {
		return ::PyType_GetFlags(this);
	}

#if (PY_VERSION_HEX >= 0x030c0000)
	PyObject*
	getDict() {
		return ::PyType_GetDict(this);
	}
#endif

#if (PY_VERSION_HEX >= 0x030b0000)
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
#endif

#if (PY_VERSION_HEX >= 0x030d0000)
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
#endif

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

#if (PY_VERSION_HEX >= 0x030b0000)

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

#endif

#if (PY_VERSION_HEX >= 0x030d0000)

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

#endif

//..............................................................................

} // namespace lua
} // namespace axl
