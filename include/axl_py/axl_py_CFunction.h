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

#define _AXL_PY_CFUNCTION_H

#include "axl_py_Object.h"

namespace axl {
namespace py {

//..............................................................................

class CFunctionBase: public ObjectBase {
public:
	bool
	check() const {
		return m_p && ::PyCFunction_Check(m_p);
	}

	bool
	checkExact() const {
		return m_p && ::PyCFunction_CheckExact(m_p);
	}

	int
	getFlags() const {
		ASSERT(m_p);
		return ::PyCFunction_GetFlags(m_p);
	}

	PyCFunction
	getFunction() const {
		ASSERT(m_p);
		return ::PyCFunction_GetFunction(m_p);
	}

	PyObject*
	getSelf() const {
		ASSERT(m_p);
		return ::PyCFunction_GetSelf(m_p); // borrowed reference
	}

	bool
	createNew(
		PyMethodDef* methodDef,
		PyObject* self = NULL,
		PyObject* module = NULL
	) {
		return finalizeCreate(::PyCFunction_NewEx(methodDef, self, module));
	}

	static
	ObjectImpl<CFunctionBase>
	fromMethodDef(
		PyMethodDef* methodDef,
		PyObject* self = NULL,
		PyObject* module = NULL
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ObjectImpl<CFunctionBase>
CFunctionBase::fromMethodDef(
	PyMethodDef* methodDef,
	PyObject* self,
	PyObject* module
) {
	ObjectImpl<CFunctionBase> result;
	result.createNew(methodDef, self, module);
	return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<CFunctionBase> CFunction;

//..............................................................................

} // namespace lua
} // namespace axl
