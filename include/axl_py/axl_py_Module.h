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
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef ObjectImpl<ModuleBase> Module;

//..............................................................................

} // namespace lua
} // namespace axl
