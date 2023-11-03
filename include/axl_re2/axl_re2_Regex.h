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

#include "axl_re2_State.h"

namespace axl {
namespace re2 {

//..............................................................................

enum RegexFlag {
	RegexFlag_Stream          = 0x01,
	RegexFlag_OneLine         = 0x02,
	RegexFlag_Latin1          = 0x04,
	RegexFlag_CaseInsensitive = 0x08,
	RegexFlag_AnchorStart     = 0x10, // RE2::ANCHOR_START
	RegexFlag_FullMatch       = 0x20, // RE2::ANCHOR_BOTH
	RegexFlag_AnchorMask      = 0x30,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum RegexKind {
	RegexKind_Undefined, // RE2::SM::kUninitialized
	RegexKind_Single,    // RE2::SM::kSingleRegexp
	RegexKind_Switch,    // RE2::SM::kRegexpSwitch
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Regex {
protected:
	class Impl; // RE2::SM

protected:
	Impl* m_impl;
	uint_t m_flags;

public:
	Regex();
	~Regex();

	RegexKind
	getRegexKind() const;

	uint_t
	getFlags() const {
		return m_flags;
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
	createSwitch(uint_t flags = RegexFlag_AnchorStart);

	uint_t
	compileSwitchCase(const sl::StringRef& source);

	bool
	finalizeSwitch();

	// execution

	State
	exec(const sl::StringRef& text) const;

	ExecResult
	exec(
		State* state,
		const sl::StringRef& chunk
	) const;

	ExecResult
	eof(
		State* state,
		int eofChar = State::EofChar
	) const;

	bool
	captureSubmatches(
		const sl::StringRef& match,
		sl::StringRef* submatchArray,
		size_t count
	) const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
State
Regex::exec(const sl::StringRef& text) const {
	State state;
	state.setEof(text.getLength());
	exec(&state, text);
	return state;
}

//..............................................................................

} // namespace re2
} // namespace axl
