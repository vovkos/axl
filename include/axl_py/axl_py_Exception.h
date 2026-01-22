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

#define _AXL_PY_EXCEPTION_H

#include "axl_py_Object.h"

namespace axl {
namespace py {

//..............................................................................

class ExceptionClassBase: public ObjectBase {
public:
	bool
	check() const {
		return m_p && PyExceptionClass_Check(m_p);
	}

	const char*
	getName() const {
		ASSERT(m_p);
		return ::PyExceptionClass_Name(m_p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<ExceptionClassBase> ExceptionClass;

//..............................................................................

class ExceptionBase: public ObjectBase {
public:
	bool
	check() const {
		return m_p && PyExceptionInstance_Check(m_p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<ExceptionBase> Exception;

//..............................................................................

class TraceBackBase: public ObjectBase {
public:
	bool
	check() const {
		return m_p && PyTraceBack_Check(m_p);
	}

	bool
	print(PyObject* file) const {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyTraceBack_Print(m_p, file) != -1);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<TraceBackBase> TraceBack;

//..............................................................................

} // namespace lua
} // namespace axl
