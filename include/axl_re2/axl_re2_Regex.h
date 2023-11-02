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

#define _AXL_RE2_REGEX_H

namespace axl {
namespace re2 {

//..............................................................................

enum CompileFlag {
	CompileFlag_CaseInsensitive = 0x01,
	CompileFlag_OneLine         = 0x02,
	CompileFlag_Latin1          = 0x04,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum RegexKind {
	RegexKind_Undefined,
	RegexKind_Single,
	RegexKind_Switch,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Regex {
protected:
	class Impl; // RE2::SM

protected:
	Impl* m_impl;
	RegexKind m_regexKind;

public:
	Regex();
	~Regex();

	RegexKind
	getRegexKind() const {
		return m_regexKind;
	}

	void
	clear();

	// compilation

	bool
	compile(
		uint_t flags,
		const sl::StringRef& source
	);

	bool
	compile(const sl::StringRef& source) {
		return compile(0, source);
	}

	void
	createSwitch() {
		clear();
		m_regexKind = RegexKind_Switch;
	}

	bool
	compileSwitchCase(
		uint_t flags,
		const sl::StringRef& source
	);

	bool
	compileSwitchCase(const sl::StringRef& source) {
		return compileSwitchCase(0, source);
	}

	bool
	finalizeSwitch();
};

//..............................................................................

} // namespace re2
} // namespace axl
