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
	RegexFlag_OneLine         = 0x01, // R2::Options.one_line -> true (otherwise, false)
	RegexFlag_Latin1          = 0x02, // R2::Options.encoding -> EncodingLatin1 (otherwise, EncodingUTF8)
	RegexFlag_CaseInsensitive = 0x04, // R2::Options.case_sensitive -> (otherwise, false)
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

	operator bool () const {
		return getRegexKind() != RegexKind_Undefined;
	}

	// properties

	RegexKind
	getRegexKind() const;

	uint_t
	getFlags() const {
		return m_flags;
	}

	sl::StringRef
	getPattern() const;

	size_t
	getCaptureCount() const;

	size_t
	getSwitchCaseCount() const;

	sl::StringRef
	getSwitchCasePattern(uint_t switchCaseId) const;

	size_t
	getSwitchCaseCaptureCount(uint_t switchCaseId) const;

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
	createSwitch(uint_t flags = 0);

	uint_t
	compileSwitchCase(const sl::StringRef& source);

	bool
	finalizeSwitch();

	// serialization

	size_t
	load(
		const void* p,
		size_t size
	);

	size_t
	load(const sl::ArrayRef<char>& buffer) {
		return load(buffer.cp(), buffer.getCount());
	}

	size_t
	save(sl::Array<char>* buffer) const;

	sl::Array<char>
	save() const {
		sl::Array<char> buffer;
		save(&buffer);
		return buffer;
	}

	// execution

	State
	exec(
		Anchor anchor,
		const sl::StringRef& text
	) const;

	State
	exec(const sl::StringRef& text) const {
		return exec(Anchor_None, text);
	}

	ExecResult
	exec(
		State* state,
		const sl::StringRef& chunk
	) const;

	ExecResult
	execEof(
		State* state,
		const sl::StringRef& lastChunk,
		int eofChar = EofChar
	) const;

	ExecResult
	execEof(
		State* state,
		int eofChar = EofChar
	) const {
		return execEof(state, sl::StringRef(), eofChar);
	}

	bool
	captureSubmatches(
		const Match& match,
		Match* submatchArray,
		size_t count
	) const {
		return captureSubmatchesImpl(RegexKind_Single, 0, match, submatchArray, count);
	}

	bool
	captureSwitchCaseSubmatches(
		uint_t switchCaseId,
		const Match& match,
		Match* submatchArray,
		size_t count
	) const {
		return captureSubmatchesImpl(RegexKind_Switch, switchCaseId, match, submatchArray, count);
	}

protected:
	bool
	captureSubmatchesImpl(
		RegexKind kind,
		uint_t switchCaseId,
		const Match& match,
		Match* submatchArray,
		size_t count
	) const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
State
Regex::exec(
	Anchor anchor,
	const sl::StringRef& text
) const {
	State state(anchor);
	execEof(&state, text);
	return state;
}

//..............................................................................

} // namespace re2
} // namespace axl
