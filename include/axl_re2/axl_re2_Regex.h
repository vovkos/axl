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

	// properties

	RegexKind
	getRegexKind() const;

	uint_t
	getFlags() const {
		return m_flags;
	}

	size_t
	getSwitchCaseCount() const;

	size_t
	getCaptureCount() const;

	size_t
	getCaptureCount(uint_t switchCaseId) const;

	// compilation

	void
	clear();

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
	exec(
		const void* p,
		size_t size
	) const;

	State
	exec(const sl::StringRef& text) const {
		return exec(text.cp(), text.getLength());
	}

	ExecResult
	exec(
		State* state,
		const void* p,
		size_t size
	) const;

	ExecResult
	exec(
		State* state,
		const sl::StringRef& chunk
	) const {
		return exec(state, chunk.cp(), chunk.getLength());
	}

	ExecResult
	eof(
		State* state,
		int eofChar = State::EofChar
	) const;

	bool
	captureSubmatches(
		const void* p,
		size_t size,
		sl::StringRef* submatchArray,
		size_t count
	) const {
		return captureSubmatchesImpl(0, p, size, submatchArray, count);
	}

	bool
	captureSubmatches(
		uint_t switchCaseId,
		const void* p,
		size_t size,
		sl::StringRef* submatchArray,
		size_t count
	) const {
		return captureSubmatchesImpl(switchCaseId, p, size, submatchArray, count);
	}

	bool
	captureSubmatches(
		const sl::StringRef& text,
		sl::StringRef* submatchArray,
		size_t count
	) const {
		return captureSubmatchesImpl(0, text.cp(), text.getLength(), submatchArray, count);
	}

	bool
	captureSubmatches(
		uint_t switchCaseId,
		const sl::StringRef& text,
		sl::StringRef* submatchArray,
		size_t count
	) const {
		return captureSubmatchesImpl(switchCaseId, text.cp(), text.getLength(), submatchArray, count);
	}

protected:
	bool
	captureSubmatchesImpl(
		uint_t switchCaseId,
		const void* p,
		size_t size,
		sl::StringRef* submatchArray,
		size_t count
	) const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
State
Regex::exec(
	const void* p,
	size_t size
) const {
	State state;
	state.setEof(size);
	exec(&state, p, size);
	return state;
}

//..............................................................................

} // namespace re2
} // namespace axl
