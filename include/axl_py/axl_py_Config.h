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

#define _AXL_PY_CONFIG_H

#include "axl_py_Error.h"

namespace axl {
namespace py {

//..............................................................................

class PreConfig: public PyPreConfig {
public:
	PreConfig() {
		PyPreConfig_InitPythonConfig(this);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class IsolatedPreConfig: public PyPreConfig {
public:
	IsolatedPreConfig() {
		PyPreConfig_InitIsolatedConfig(this);
	}
};

//..............................................................................

class FreePyInitConfig {
public:
	void
	operator () (PyInitConfig* p) {
		::PyInitConfig_Free(p);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class InitConfig: public sl::Handle<PyInitConfig*, FreePyInitConfig> {
public:
	InitConfig() {
		m_h = ::PyInitConfig_Create();
	}

	static
	bool
	getError(
		PyInitConfig* p,
		sl::StringRef* message
	);

	static
	sl::StringRef
	getError(PyInitConfig* p);

	bool
	getError(sl::StringRef* message) const {
		return getError(m_h, message);
	}

	sl::StringRef
	getError() const {
		return getError(m_h);
	}

	bool
	getExitCode(int* code) const;

	int
	getExitCode() const;

	bool
	hasOption(const sl::StringRef& name) const {
		ASSERT(m_h);
		return ::PyInitConfig_HasOption(m_h, name.sz());
	}

	bool
	getInt(
		const sl::StringRef& name,
		int64_t* value
	) const {
		ASSERT(m_h);
		return complete(::PyInitConfig_GetInt(m_h, name.sz(), value) != -1);
	}

	int64_t
	getInt(const sl::StringRef& name) const;

	bool
	setInt(
		const sl::StringRef& name,
		int64_t value
	) const {
		ASSERT(m_h);
		return complete(::PyInitConfig_SetInt(m_h, name.sz(), value) != -1);
	}

	bool
	getStr(
		const sl::StringRef& name,
		sl::String* value
	) const;

	sl::String
	getStr(const sl::StringRef& name) const;

	bool
	setStr(
		const sl::StringRef& name,
		const sl::StringRef& value
	) const {
		ASSERT(m_h);
		return complete(::PyInitConfig_SetStr(m_h, name.sz(), value.sz()));
	}

protected:
	bool
	complete(bool result) const {
		if (!result)
			err::setError(getError());

		return result;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
InitConfig::getError(
	PyInitConfig* config,
	sl::StringRef* message
) {
	const char* p = NULL;
	int result = ::PyInitConfig_GetError(config, &p);
	if (!result)
		return false;

	*message = p;
	return true;
}

inline
sl::StringRef
InitConfig::getError(PyInitConfig* p) {
	sl::StringRef message;
	getError(p, &message);
	return message;
}

inline
bool
InitConfig::getExitCode(int* code) const {
	ASSERT(m_h);
	int result = ::PyInitConfig_GetExitCode(m_h, code);
	return result != 0;
}

inline
int
InitConfig::getExitCode() const {
	int code = 0;
	getExitCode(&code);
	return code;
}

inline
int64_t
InitConfig::getInt(const sl::StringRef& name) const {
	int64_t value;
	getInt(name, &value);
	return value;
}

inline
bool
InitConfig::getStr(
	const sl::StringRef& name,
	sl::String* value
) const {
	ASSERT(m_h);

	char* p;
	int result = ::PyInitConfig_GetStr(m_h, name.sz(), &p);
	if (result == -1)
		return err::fail(getError());

	*value = p;
	return true;
}

inline
sl::String
InitConfig::getStr(const sl::StringRef& name) const {
	sl::String string;
	getStr(name, &string);
	return string;
}

//..............................................................................

class ConfigBase: public PyConfig {
public:
	~ConfigBase() {
		PyConfig_Clear(this);
	}

	bool
	setString(
		wchar_t** field,
		const sl::StringRef_w& value
	) {
		return completeWithPyStatus(::PyConfig_SetString(this, field, value.cp()));
	}

	bool
	setString(
		wchar_t** field,
		const sl::StringRef& value
	) {
		return completeWithPyStatus(::PyConfig_SetBytesString(this, field, value.cp()));
	}

	bool
	setArgv(
		int argc,
		wchar_t* const* argv
	) {
		return completeWithPyStatus(::PyConfig_SetArgv(this, argc, argv));
	}

	bool
	setArgv(
		int argc,
		char* const* argv
	) {
		return completeWithPyStatus(::PyConfig_SetBytesArgv(this, argc, argv));
	}

	bool
	setWideStringList(
		PyWideStringList* field,
		size_t count,
		wchar_t** items
	) {
		return completeWithPyStatus(::PyConfig_SetWideStringList(this, field, count, items));
	}

	bool
	read() {
		return completeWithPyStatus(::PyConfig_Read(this));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Config: public ConfigBase {
public:
	Config() {
		PyConfig_InitPythonConfig(this);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class IsolatedConfig: public ConfigBase {
public:
	IsolatedConfig() {
		PyConfig_InitIsolatedConfig(this);
	}
};

//..............................................................................

} // namespace lua
} // namespace axl
