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

#define _AXL_PY_INIT_H

#include "axl_py_Module.h"

namespace axl {
namespace py {

//..............................................................................

inline
bool
preInitialize(const PyPreConfig* config) {
	return completeWithPyStatus(::Py_PreInitialize(config));
}

bool
initialize(PyInitConfig* config);

bool
initialize(const PyConfig* config);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
PyObject*
addModule(const sl::StringRef& name) {
	PyObject* module = ::PyImport_AddModule(name.sz()); // borrowed reference
	return completeWithLastPyErr<PyObject*>(module, NULL);
}

inline
PyObject*
addModule(PyObject* name) {
	PyObject* module = ::PyImport_AddModuleObject(name); // borrowed reference
	return completeWithLastPyErr<PyObject*>(module, NULL);
}

inline
bool
importModule(
	PyObject** result,
	const sl::StringRef& name
) {
	*result = ::PyImport_ImportModule(name.sz()); // new reference
	return completeWithLastPyErr(*result != NULL);
}

inline
Module
importModule(const sl::StringRef& name) {
	Module module;
	importModule(&module, name);
	return module;
}

inline
bool
importModule(
	PyObject** result,
	PyObject* name
) {
	*result = ::PyImport_Import(name); // new reference
	return completeWithLastPyErr(*result != NULL);
}

inline
Module
importModule(PyObject* name) {
	Module module;
	importModule(&module, name);
	return module;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
compile(
	int startSymbol,
	PyObject** result,
	const sl::StringRef& code,
	const sl::StringRef& fileName,
	PyCompilerFlags* flags = NULL,
	int optimize = -1
);

inline
Object
compile(
	int startSymbol,
	const sl::StringRef& code,
	const sl::StringRef& fileName,
	PyCompilerFlags* flags = NULL,
	int optimize = -1
) {
	Object result;
	compile(startSymbol, &result, code, fileName, flags, optimize);
	return result;
}

inline
bool
compile(
	PyObject** result,
	const sl::StringRef& code,
	const sl::StringRef& fileName,
	PyCompilerFlags* flags = NULL,
	int optimize = -1
) {
	return compile(Py_file_input, result, code, fileName, flags, optimize);
}

inline
Object
compile(
	const sl::StringRef& code,
	const sl::StringRef& fileName,
	PyCompilerFlags* flags = NULL,
	int optimize = -1
) {
	Object result;
	compile(&result, code, fileName, flags, optimize);
	return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
run(
	int startSymbol,
	PyObject** result,
	const sl::StringRef& code,
	PyObject* globals,
	PyObject* locals = NULL,
	PyCompilerFlags* flags = NULL
);

inline
Object
run(
	int startSymbol,
	const sl::StringRef& code,
	PyObject* globals,
	PyObject* locals = NULL,
	PyCompilerFlags* flags = NULL
) {
	Object result;
	run(startSymbol, &result, code, globals, locals, flags);
	return result;
}

inline
bool
run(
	PyObject** result,
	const sl::StringRef& code,
	PyObject* globals,
	PyObject* locals = NULL,
	PyCompilerFlags* flags = NULL
) {
	return run(Py_file_input, result, code, globals, locals, flags);
}

inline
Object
run(
	const sl::StringRef& code,
	PyObject* globals,
	PyObject* locals = NULL,
	PyCompilerFlags* flags = NULL
) {
	Object result;
	run(&result, code, globals, locals, flags);
	return result;
}

inline
bool
run(
	const sl::StringRef& code,
	PyCompilerFlags* flags = NULL
) {
	int result = ::PyRun_SimpleStringFlags(code.sz(), flags);
	if (result == -1)
		return failWithLastPyErr();

	return true;
}

//..............................................................................

} // namespace lua
} // namespace axl
