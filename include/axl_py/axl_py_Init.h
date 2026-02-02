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

enum InitializeFlag {
	InitializeFlag_ReleaseGil = 0x01,
};

inline
bool
preInitialize(const PyPreConfig* config) {
	return completeWithPyStatus(::Py_PreInitialize(config));
}

#if (PY_VERSION_HEX >= 0x030e0000)
bool
initialize(
	PyInitConfig* config,
	uint_t flags = 0
);
#endif

bool
initialize(
	const PyConfig* config,
	uint_t flags = 0
);

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
	const sl::StringRef& source,
	const sl::StringRef& fileName,
	PyCompilerFlags* flags = NULL,
	int optimize = -1
);

inline
Object
compile(
	int startSymbol,
	const sl::StringRef& source,
	const sl::StringRef& fileName,
	PyCompilerFlags* flags = NULL,
	int optimize = -1
) {
	Object result;
	compile(startSymbol, &result, source, fileName, flags, optimize);
	return result;
}

inline
bool
compile(
	PyObject** result,
	const sl::StringRef& source,
	const sl::StringRef& fileName,
	PyCompilerFlags* flags = NULL,
	int optimize = -1
) {
	return compile(Py_file_input, result, source, fileName, flags, optimize);
}

inline
Object
compile(
	const sl::StringRef& source,
	const sl::StringRef& fileName,
	PyCompilerFlags* flags = NULL,
	int optimize = -1
) {
	Object result;
	compile(&result, source, fileName, flags, optimize);
	return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
eval(
	PyObject** result,
	PyObject* code,
	PyObject* globals,
	PyObject* locals = NULL
);

inline
Object
eval(
	PyObject* code,
	PyObject* globals,
	PyObject* locals = NULL
) {
	Object result;
	eval(&result, code, globals, locals);
	return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
run(
	int startSymbol,
	PyObject** result,
	const sl::StringRef& source,
	PyObject* globals,
	PyObject* locals = NULL,
	PyCompilerFlags* flags = NULL
);

inline
Object
run(
	int startSymbol,
	const sl::StringRef& source,
	PyObject* globals,
	PyObject* locals = NULL,
	PyCompilerFlags* flags = NULL
) {
	Object result;
	run(startSymbol, &result, source, globals, locals, flags);
	return result;
}

inline
bool
run(
	PyObject** result,
	const sl::StringRef& source,
	PyObject* globals,
	PyObject* locals = NULL,
	PyCompilerFlags* flags = NULL
) {
	return run(Py_file_input, result, source, globals, locals, flags);
}

inline
Object
run(
	const sl::StringRef& source,
	PyObject* globals,
	PyObject* locals = NULL,
	PyCompilerFlags* flags = NULL
) {
	Object result;
	run(&result, source, globals, locals, flags);
	return result;
}

inline
bool
run(
	const sl::StringRef& source,
	PyCompilerFlags* flags = NULL
) {
	int result = ::PyRun_SimpleStringFlags(source.sz(), flags);
	if (result == -1)
		return failWithLastPyErr();

	return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
int
clearTypeCache() {
	return ::PyType_ClearCache();
}

#if (PY_VERSION_HEX >= 0x030c0000)

inline
int
addTypeWatcher(PyType_WatchCallback callback) {
	return completeWithLastPyErr<int>(::PyType_AddWatcher(callback), -1);
}

inline
bool
clearTypeWatcher(int id) {
	return completeWithLastPyErr(::PyType_ClearWatcher(id) != -1);
}

#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ThreadState {
protected:
	PyThreadState* m_state;

public:
	ThreadState() {
		m_state = NULL;
	}

	operator bool () {
		return m_state;
	}

	bool
	isNull() const {
		return m_state == NULL;
	}

	bool
	detach() {
		return completeWithLastPyErr((m_state = ::PyEval_SaveThread()) != NULL);
	}

	void
	attach();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
ThreadState::attach() {
	ASSERT(m_state);
	::PyEval_RestoreThread(m_state);
	m_state = NULL;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class DetachedThreadRegion: public ThreadState {
public:
	DetachedThreadRegion() {
		detach();
		ASSERT(!isNull());
	}

	~DetachedThreadRegion() {
		if (m_state) // we could have reattached manually
			attach();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class GilRegion {
protected:
	PyGILState_STATE m_state;

public:
	GilRegion() {
		m_state = ::PyGILState_Ensure();
	}

	~GilRegion() {
		::PyGILState_Release(m_state);
	}
};

//..............................................................................

} // namespace py
} // namespace axl
