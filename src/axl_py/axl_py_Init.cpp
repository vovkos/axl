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
#include "axl_py_Init.h"
#include "axl_py_Config.h"

namespace axl {
namespace py {

//..............................................................................

class PyFinalizer {
public:
	~PyFinalizer() {
		::Py_Finalize();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
initialize(PyInitConfig* config) {
	int result = ::Py_InitializeFromInitConfig(config);
	if (result == -1) {
		err::setError(InitConfig::getError(config));
		return false;
	}

	sl::getSingleton<PyFinalizer>();
	return true;
}

bool
initialize(const PyConfig* config) {
	PyStatus status = ::Py_InitializeFromConfig(config);
	if (PyStatus_Exception(status))
		return failWithPyStatus(status);

	sl::getSingleton<PyFinalizer>();
	return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
compile(
	int startSymbol,
	PyObject** result_o,
	const sl::StringRef& code,
	const sl::StringRef& fileName,
	PyCompilerFlags* flags,
	int optimize
) {
	PyObject* result = ::Py_CompileStringExFlags(
		code.sz(),
		fileName.sz(),
		startSymbol,
		flags,
		optimize
	);

	if (!result)
		return failWithLastPyErr();

	if (result_o)
		*result_o = result;
	else
		Py_DECREF(result);

	return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
run(
	int startSymbol,
	PyObject** result_o,
	const sl::StringRef& code,
	PyObject* globals,
	PyObject* locals,
	PyCompilerFlags* flags
) {
	PyObject* result = ::PyRun_StringFlags(
		code.sz(),
		startSymbol,
		globals,
		locals,
		flags
	);

	if (!result)
		return failWithLastPyErr();

	if (result_o)
		*result_o = result;
	else
		Py_DECREF(result);

	return true;
}

//..............................................................................

} // namespace py
} // namespace axl
