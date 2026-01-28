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

struct PyFinalizer {
	PyThreadState* m_mainThreadState;

	PyFinalizer() {
		m_mainThreadState = NULL;
	}

	~PyFinalizer() {
		if (m_mainThreadState)
			::PyEval_RestoreThread(m_mainThreadState);

		::Py_Finalize();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
setupFinalizer(uint_t flags) {
	PyFinalizer* finalizer = sl::getSingleton<PyFinalizer>();
	if (flags & InitializeFlag_ReleaseGil)
		finalizer->m_mainThreadState = ::PyEval_SaveThread();
}

bool
initialize(
	PyInitConfig* config,
	uint_t flags
) {
	int result = ::Py_InitializeFromInitConfig(config);
	if (result == -1) {
		err::setError(InitConfig::getError(config));
		return false;
	}

	setupFinalizer(flags);
	return true;
}

bool
initialize(
	const PyConfig* config,
	uint_t flags
) {
	PyStatus status = ::Py_InitializeFromConfig(config);
	if (PyStatus_Exception(status))
		return failWithPyStatus(status);

	setupFinalizer(flags);
	return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
compile(
	int startSymbol,
	PyObject** result_o,
	const sl::StringRef& source,
	const sl::StringRef& fileName,
	PyCompilerFlags* flags,
	int optimize
) {
	PyObject* result = ::Py_CompileStringExFlags(
		source.sz(),
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
eval(
	PyObject** result_o,
	PyObject* code,
	PyObject* globals,
	PyObject* locals
) {
	PyObject* result = ::PyEval_EvalCode(code, globals, locals);

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
	const sl::StringRef& source,
	PyObject* globals,
	PyObject* locals,
	PyCompilerFlags* flags
) {
	PyObject* result = ::PyRun_StringFlags(
		source.sz(),
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
