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
		return m_p && ::PyModule_Check(m_p);
	}

	bool
	checkExact() const {
		return m_p && ::PyModule_CheckExact(m_p);
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
	getNameObject() const {
		ObjectImpl<UnicodeBase> name;
		getNameObject(&name);
		return name;
	}

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
	getFileNameObject() const {
		ObjectImpl<UnicodeBase> name;
		getFileNameObject(&name);
		return name;
	}

	bool
	add(
		const sl::StringRef& name,
		PyObject* item
	) const {
		ASSERT(m_p);
		return completeWithLastPyErr(::PyModule_AddObjectRef(m_p, name.sz(), item) != -1);
	}

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<ModuleBase> Module;
typedef ObjectBorrowedImpl<ModuleBase> ModuleBorrowed;

//..............................................................................

} // namespace lua
} // namespace axl
