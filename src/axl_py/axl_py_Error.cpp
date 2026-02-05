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

#include "pch.h"
#include "axl_py_Error.h"
#include "axl_py_Exception.h"
#include "axl_py_Unicode.h"
#include "axl_py_Init.h"
#include "axl_py_Long.h"
#include "axl_py_Module.h"
#include "axl_lex_ParseError.h"

// #define _AXL_PY_ERROR_TRACEBACK 1

namespace axl {
namespace py {

//..............................................................................

err::Error
createPyStatusError(const PyStatus& status) {
	if (!status.func)
		return status.err_msg;

	err::Error error;
	error.formatStringError("%s: %s", status.func, status.err_msg);
	return error;
}

err::ErrorRef
extractSrcPosError(PyObject* exception0) {
	static const sl::StringRef DefaultFileName = "untitled-script";

	ObjectBorrowed exception = exception0;
	if (!exception.hasAttr("lineno"))
		return err::ErrorRef();

	Long lineAttr = exception.getAttr("lineno");
	if (!lineAttr.check())
		return err::ErrorRef();

	int line = lineAttr.getLong() - 1; // python lines are one-based
	Long offsetAttr = exception.getAttr("offset");
	int col = offsetAttr.check() ? offsetAttr.getLong() - 1 : 0; // python cols are one-based
	Unicode fileNameAttr = exception.getAttr("filename");
	sl::StringRef fileName = fileNameAttr.check() ? fileNameAttr.getUtf8() : DefaultFileName;
	return lex::createSrcPosError(fileName, line, col);
}

err::Error
createPyErrError(
    PyObject* type,
	PyObject* value,
	PyObject* traceback
) {
	if (!type)
		return &err::g_noError;

	ExceptionClassBorrowed exceptionClass = type;
	sl::String string = exceptionClass.check() ?
		exceptionClass.getName() :
		exceptionClass.getStr().getUtf8();

	if (!value)
		return string;

	ExceptionBorrowed exception = value;
	ASSERT(exception.check());

	Unicode msg = exception.hasAttr("msg") ?
		Unicode(exception.getAttr("msg")) :
		exception.getStr();

	if (msg.check()) {
		string += ": ";
		string += msg.getUtf8();
	}

	err::ErrorRef srcPosError;
	if (!traceback) {
		srcPosError = extractSrcPosError(value);
		if (!srcPosError)
			return string;
	} else if (Traceback::check(traceback)) {
		PyTracebackObject* top = (PyTracebackObject*)traceback;
		while (top->tb_next)
			top = top->tb_next;

		FrameBorrowed frame = (PyObject*)((PyTracebackObject*)top)->tb_frame;
		sl::StringRef fileName = frame.getCode().getFileName().getUtf8();
		int line = frame.getLineNumber() - 1; // python lines are one-based
		srcPosError = lex::createSrcPosError(fileName, line, 0);

#if (_AXL_PY_ERROR_TRACEBACK)
		Module tbm = importModule("traceback");
		Object format = tbm.getAttr("format_tb");
		Object list = format.call(traceback);
		if (list)
			if (Unicode joinedList = Unicode::join(list)) {
				string += ":\n";
				string += joinedList.getUtf8();
			}
#endif
	}

	err::Error error = string;
	error.push(srcPosError);
	return error;
}

size_t
setLastPyErr() {
    Object type;
	Object value;
	Object traceback;

    ::PyErr_Fetch(&type, &value, &traceback);
    ::PyErr_NormalizeException(&type, &value, &traceback);
	err::Error error = createPyErrError(type, value, traceback);
	return err::setError(error);
}

//..............................................................................

} // namespace py
} // namespace axl
