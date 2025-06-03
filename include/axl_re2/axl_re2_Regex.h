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

// non-conflicting with ExecFlag-s

enum RegexFlag {
	RegexFlag_OneLine         = 0x10, // R2::Options.one_line -> true (otherwise, false)
	RegexFlag_Latin1          = 0x20, // R2::Options.encoding -> EncodingLatin1 (otherwise, EncodingUTF8)
	RegexFlag_CaseInsensitive = 0x40, // R2::Options.case_sensitive -> false (otherwise, true)
	RegexFlag_WholeWord       = 0x80, // R2::Options.word_boundary -> true (otherwise, false)
	RegexFlag__Mask           = 0xf0,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum RegexKind {
	RegexKind_Undefined, // RE2::SM::kUndefined
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
	Regex() {
		init();
	}

	Regex(
		const sl::StringRef& pattern,
		uint_t flags = 0
	) {
		init();
		compile(pattern, flags);
	}

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

	bool
	isNullable() const;

	// compilation

	void
	clear();

	bool
	compile(
		const sl::StringRef& source,
		uint_t flags = 0
	);

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

	Match
	exec(
		const sl::StringRef& text,
		uint_t execFlags = 0
	) const;

	Match
	exec(
		size_t chunkCount,
		const sl::StringRef* chunkTable,
		uint_t execFlags = 0
	) const;

	ExecResult
	exec(
		State* state,
		const sl::StringRef& chunk
	) const;

	ExecResult
	exec(
		State* state,
		size_t chunkOffset,
		size_t chunkCount,
		const sl::StringRef* chunkTable
	) const;

	ExecResult
	exec(
		State* state,
		size_t chunkCount,
		const sl::StringRef* chunkTable
	) const {
		return exec(state, 0, chunkCount, chunkTable);
	}

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

	ExecResult
	execEof(
		State* state,
		size_t chunkOffset,
		size_t chunkCount,
		const sl::StringRef* chunkTable,
		int eofChar = EofChar
	) const;

	ExecResult
	execEof(
		State* state,
		size_t chunkCount,
		const sl::StringRef* chunkTable,
		int eofChar = EofChar
	) const {
		return execEof(state, 0, chunkCount, chunkTable, eofChar);
	}

	size_t
	captureSubmatches(
		const sl::StringRef& matchText,
		Capture* submatchArray,
		size_t count
	) const {
		return captureSubmatchesImpl(
			RegexKind_Single,
			0,
			0,
			matchText,
			submatchArray,
			count
		);
	}

	size_t
	captureSubmatches(
		uint64_t matchOffset,
		const sl::StringRef& matchText,
		Capture* submatchArray,
		size_t count
	) const {
		return captureSubmatchesImpl(
			RegexKind_Single,
			0,
			matchOffset,
			matchText,
			submatchArray,
			count
		);
	}

	size_t
	captureSubmatches(
		const Capture& match,
		Capture* submatchArray,
		size_t count
	) const {
		return captureSubmatchesImpl(
			RegexKind_Single,
			0,
			match.getOffset(),
			match.getText(),
			submatchArray,
			count
		);
	}

	size_t
	captureSwitchCaseSubmatches(
		uint_t switchCaseId,
		const sl::StringRef& matchText,
		Capture* submatchArray,
		size_t count
	) const {
		return captureSubmatchesImpl(
			RegexKind_Switch,
			switchCaseId,
			0,
			matchText,
			submatchArray,
			count
		);
	}

	size_t
	captureSwitchCaseSubmatches(
		uint_t switchCaseId,
		uint64_t matchOffset,
		const sl::StringRef& matchText,
		Capture* submatchArray,
		size_t count
	) const {
		return captureSubmatchesImpl(
			RegexKind_Switch,
			switchCaseId,
			matchOffset,
			matchText,
			submatchArray,
			count
		);
	}

	size_t
	captureSwitchCaseSubmatches(
		uint_t switchCaseId,
		const Capture& match,
		Capture* submatchArray,
		size_t count
	) const {
		return captureSubmatchesImpl(
			RegexKind_Switch,
			switchCaseId,
			match.getOffset(),
			match.getText(),
			submatchArray,
			count
		);
	}

protected:
	void
	init();

	size_t
	captureSubmatchesImpl(
		RegexKind kind,
		uint_t switchCaseId,
		uint64_t matchOffset,
		const sl::StringRef& matchText,
		Capture* submatchArray,
		size_t count
	) const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
Match
Regex::exec(
	const sl::StringRef& text,
	uint_t execFlags
) const {
	State state(execFlags);
	execEof(&state, text);
	return state.isMatch() ? state.getMatch() : Match();
}

inline
Match
Regex::exec(
	size_t chunkCount,
	const sl::StringRef* chunkTable,
	uint_t execFlags
) const {
	State state(execFlags);
	execEof(&state, chunkCount, chunkTable);
	return state.isMatch() ? state.getMatch() : Match();
}

//..............................................................................

} // namespace re2
} // namespace axl
