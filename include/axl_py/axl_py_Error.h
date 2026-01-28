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
raiseException(
	PyObject* exception,
	const sl::StringRef& description
) {
	::PyErr_SetString(exception, description.sz());
}

// later add a dedicated error object that would get description on demand

inline
void
raiseException(
	PyObject* exception,
	const err::ErrorRef& error = err::getLastError()
) {
	::PyErr_SetString(exception, error.getDescription().sz());
}

//..............................................................................

} // namespace lua
} // namespace axl
