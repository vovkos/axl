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

#define _AXL_PY_ERROR_H

#include "axl_py_Pch.h"

namespace axl {
namespace py {

//..............................................................................

err::Error
createPyStatusError(const PyStatus& status);

inline
bool
failWithPyStatus(const PyStatus& status) {
	return err::fail(createPyStatusError(status));
}

inline
bool
completeWithPyStatus(const PyStatus& status) {
	if (::PyStatus_Exception(status))
		return err::fail(createPyStatusError(status));

	return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

err::Error
createPyErrError(
    PyObject* type,
	PyObject* error,
	PyObject* traceback
);

size_t
setLastPyErr();

template <typename T>
T
failWithLastPyErr(T failResult) {
	setLastPyErr();
	return failResult;
}

inline
bool
failWithLastPyErr() {
	return failWithLastPyErr<bool>(false);
}

template <typename T>
T
completeWithLastPyErr(
	T result,
	T failResult
) {
	if (result == failResult)
		setLastPyErr();

	return result;
}

inline
bool
completeWithLastPyErr(int result) {
	return completeWithLastPyErr<bool>(result != 0, false);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
raiseException(
	PyObject* exception,
	const char* description
) {
	::PyErr_SetString(exception, description);
}

inline
void
raiseException(const char* description) {
	::PyErr_SetString(PyExc_Exception, description);
}

inline
void
raiseException(
	PyObject* exception,
	const sl::StringRef& description
) {
	raiseException(exception, description.sz());
}

inline
void
raiseException(const sl::StringRef& description) {
	raiseException(description.sz());
}

// later maybe add a dedicated error object that would get description on demand

inline
void
raiseException(
	PyObject* exception,
	const err::ErrorRef& error = err::getLastError()
) {
	raiseException(exception, error.getDescription().sz());
}

inline
void
raiseException(const err::ErrorRef& error = err::getLastError()) {
	raiseException(error.getDescription().sz());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
PyObject*
failCFunction(
	PyObject* exception,
	const char* description
) {
	raiseException(exception, description);
	return NULL;
}

inline
PyObject*
failCFunction(const char* description) {
	raiseException(description);
	return NULL;
}

inline
PyObject*
failCFunction(
	PyObject* exception,
	const sl::StringRef& description
) {
	raiseException(exception, description);
	return NULL;
}

inline
PyObject*
failCFunction(const sl::StringRef& description) {
	raiseException(description);
	return NULL;
}

inline
PyObject*
failCFunction(
	PyObject* exception,
	const err::ErrorRef& error = err::getLastError()
) {
	raiseException(exception, error);
	return NULL;
}

inline
PyObject*
failCFunction(const err::ErrorRef& error = err::getLastError()) {
	raiseException(error);
	return NULL;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
PyObject*
completeCFunction(PyObject* result) {
	return result ? result : failCFunction();
}

//..............................................................................

} // namespace py
} // namespace axl
