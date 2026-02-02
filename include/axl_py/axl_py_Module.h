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

#define _AXL_PY_MODULE_H

#include "axl_py_Unicode.h"

namespace axl {
namespace py {

//..............................................................................

class ModuleBase: public ObjectBase {
public:
	bool
	check() const {
		return m_p && PyModule_Check(m_p);
	}

	bool
	checkExact() const {
		return m_p && ::PyModule_CheckExact(m_p);
	}

	static
	bool
	check(PyObject* p) {
		return PyModule_Check(p);
	}

	static
	bool
	checkExact(PyObject* p) {
		return PyModule_CheckExact(p);
	}

	bool
	create(sl::StringRef& name) {
		return finalizeCreate(::PyModule_New(name.sz()));
	}

	bool
	create(PyObject* name) {
		return finalizeCreate(::PyModule_NewObject(name));
	}

	bool
	create(PyModuleDef* moduleDef) {
		return finalizeCreate(::PyModule_Create(moduleDef));
	}

	bool
	create(
		PyModuleDef* moduleDef,
		int apiVersion
	) {
		return finalizeCreate(::PyModule_Create2(moduleDef, apiVersion));
	}

	static
	ObjectImpl<ModuleBase>
	fromModuleDef(PyModuleDef* moduleDef);

	static
	ObjectImpl<ModuleBase>
	fromModuleDef(
		PyModuleDef* moduleDef,
		int apiVersion
	);

	PyObject*
	getDict() const {
		ASSERT(m_p);
		return ::PyModule_GetDict(m_p); // borrowed reference
	}

	sl::StringRef
	getName() const {
		ASSERT(m_p);
		return ::PyModule_GetName(m_p);
	}

	bool
	getNameObject(PyObject** result) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = ::PyModule_GetNameObject(m_p)) != NULL);
	}

	ObjectImpl<UnicodeBase>
	getNameObject() const;

	sl::StringRef
	getFileName() const {
		ASSERT(m_p);
		return ::PyModule_GetFilename(m_p);
	}

	bool
	getFileNameObject(PyObject** result) const {
		ASSERT(m_p);
		return completeWithLastPyErr((*result = ::PyModule_GetFilenameObject(m_p)) != NULL);
	}

	ObjectImpl<UnicodeBase>
	getFileNameObject() const;

	bool
	add(
		const sl::StringRef& name,
		PyObject* item
	) const;

	bool
	addIntConstant(
		const sl::StringRef& name,
		long value
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyModule_AddIntConstant(m_p, name.sz(), value) != -1);
	}

	bool
	addStringConstant(
		const sl::StringRef& name,
		const sl::StringRef& value
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyModule_AddStringConstant(m_p, name.sz(), value.sz()) != -1);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ObjectImpl<ModuleBase>
ModuleBase::fromModuleDef(PyModuleDef* moduleDef) {
	ObjectImpl<ModuleBase> result;
	result.create(moduleDef);
	return result;
}

inline
ObjectImpl<ModuleBase>
ModuleBase::fromModuleDef(
	PyModuleDef* moduleDef,
	int apiVersion
) {
	ObjectImpl<ModuleBase> result;
	result.create(moduleDef, apiVersion);
	return result;
}

inline
ObjectImpl<UnicodeBase>
ModuleBase::getNameObject() const {
	ObjectImpl<UnicodeBase> name;
	getNameObject(&name);
	return name;
}

inline
ObjectImpl<UnicodeBase>
ModuleBase::getFileNameObject() const {
	ObjectImpl<UnicodeBase> name;
	getFileNameObject(&name);
	return name;
}

inline
bool
ModuleBase::add(
	const sl::StringRef& name,
	PyObject* item
) const {
	ASSERT(m_p);
#if (PY_VERSION_HEX >= 0x030a0000)
	return completeWithLastPyErr(::PyModule_AddObjectRef(m_p, name.sz(), item) != -1);
#else
	int result = ::PyModule_AddObject(m_p, name.sz(), item);
	if (result == -1)
		return failWithLastPyErr();

	Py_INCREF(item); // compensate for the reference stolen by PyModule_AddObject
	return true;
#endif
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<ModuleBase> Module;
typedef ObjectBorrowedImpl<ModuleBase> ModuleBorrowed;

//..............................................................................

} // namespace lua
} // namespace axl
