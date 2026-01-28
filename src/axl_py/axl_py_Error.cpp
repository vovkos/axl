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
#include "axl_py_Module.h"

namespace axl {
namespace py {

//..............................................................................

err::Error
createPyStatusError(const PyStatus& status) {
	return err::Error(status.err_msg);
}

err::Error
createPyErrError(
    PyObject* type,
	PyObject* value,
	PyObject* traceback
) {
	if (!type)
		return &err::g_noError;

	;

	ExceptionClass exceptionClass = type;
	sl::String string = exceptionClass.check() ?
		exceptionClass.getName() :
		exceptionClass.getStr().getUtf8();

	if (!value)
		return string;

	Exception exception = value;
	ASSERT(exception.check());
	if (Unicode unicode = exception.getStr()) {
		string += ": ";
		string += unicode.getUtf8();
	}

	if (!traceback)
		return string;

	Module tbm = importModule("traceback");
	Object format = tbm.getAttr("format_tb");
	Object list = format.call(traceback);
	if (list)
		if (Unicode joinedList = Unicode::join(list)) {
			string += ":\n";
			string += joinedList.getUtf8();
		}

	return string;
}

size_t
setLastPyErr() {
    Object type;
	Object value;
	Object traceback;

    ::PyErr_Fetch(&type, &value, &traceback);
    ::PyErr_NormalizeException(type.p(), value.p(), traceback.p());
	err::Error error = createPyErrError(type, value, traceback);
	return err::setError(error);
}

//..............................................................................

} // namespace py
} // namespace axl
