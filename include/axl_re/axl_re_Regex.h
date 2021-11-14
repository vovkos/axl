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

#define _AXL_RE_REGEX_H

#include "axl_re_RegexState.h"

namespace axl {
namespace re {

class RegexState;
struct RegexStorageHdr;

//..............................................................................

enum RegexKind {
	RegexKind_Undefined,
	RegexKind_Single,
	RegexKind_Switch,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum RegexCompileFlag {
	RegexCompileFlag_SparseSyntax    = 0x01,
	RegexCompileFlag_CaseInsensitive = 0x02,
	RegexCompileFlag_NoCapture       = 0x10, // same as RegexExecFlag_NoCapture
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Regex {
	friend class RegexCompiler;
	friend class DfaBuilder;

protected:
	struct SwitchCaseContext: sl::ListLink {
		size_t m_baseCaptureId;
		size_t m_captureCount;
		NfaState* m_nfaState;
		DfaState* m_dfaState;
	};

protected:
	RegexKind m_regexKind;
	sl::List<NfaState> m_nfaStateList;
	sl::List<DfaState> m_dfaStateList;
	sl::List<DfaState> m_preDfaStateList;
	NfaStateSetMap<DfaState*> m_dfaStateMap;
	sl::List<SwitchCaseContext> m_switchCaseContextList;
	sl::Array<SwitchCaseContext*> m_switchCaseContextArray;
	size_t m_captureCount;

public:
	Regex();

	bool
	isEmpty() const {
		return m_nfaStateList.isEmpty();
	}

	RegexKind
	getRegexKind() {
		return m_regexKind;
	}

	size_t
	getCaptureCount() const {
		return m_captureCount;
	}

	size_t
	getNfaStateCount() const {
		return m_nfaStateList.getCount();
	}

	const NfaState*
	getNfaStartState() const {
		return *m_nfaStateList.getHead();
	}

	const DfaState*
	getDfaStartState() {
		return !m_dfaStateList.isEmpty() ?
			*m_dfaStateList.getHead() :
			createDfaStartState(*m_nfaStateList.getHead());
	}

	size_t
	getSwitchCaseCount() {
		ASSERT(m_regexKind == RegexKind_Switch);
		return m_switchCaseContextArray.getCount();
	}

	size_t
	getSwitchCaseBaseCaptureId(size_t id) {
		ASSERT(m_regexKind == RegexKind_Switch);
		return m_switchCaseContextArray[id]->m_baseCaptureId;
	}

	size_t
	getSwitchCaseCaptureCount(size_t id) {
		ASSERT(m_regexKind == RegexKind_Switch);
		return m_switchCaseContextArray[id]->m_captureCount;
	}

	const NfaState*
	getSwitchCaseNfaState(size_t id) {
		ASSERT(m_regexKind == RegexKind_Switch);
		return m_switchCaseContextArray[id]->m_nfaState;
	}

	const DfaState*
	getSwitchCaseDfaState(size_t id);

	void
	clear();

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
	saveNfa(sl::Array<char>* buffer);

	size_t
	saveDfa(sl::Array<char>* buffer);

	sl::Array<char>
	saveNfa() {
		sl::Array<char> buffer;
		saveNfa(&buffer);
		return buffer;
	}

	sl::Array<char>
	saveDfa() {
		sl::Array<char> buffer;
		saveDfa(&buffer);
		return buffer;
	}

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
	createSwitch();

	size_t
	compileSwitchCase(
		uint_t flags,
		const sl::StringRef& source
	);

	size_t
	compileSwitchCase(const sl::StringRef& source) {
		return compileSwitchCase(0, source);
	}

	void
	finalizeSwitch() {
		ASSERT(m_regexKind = RegexKind_Switch);
		finalize();
	}

	void
	buildFullDfa();

	bool
	eof(RegexState* state) {
		ASSERT(state->getRegex() == this && (state->getExecFlags() & RegexExecFlag_Stream));
		return state->eof();
	}

	bool
	match(
		RegexState* state,
		const void* p,
		size_t size
	);

	RegexState
	match(
		uint_t flags,
		enc::CharCodec* codec,
		const void* p,
		size_t size
	) {
		RegexState state(flags, codec);
		return match(&state, p, size) ? state : RegexState();
	}

	RegexState
	match(
		uint_t flags,
		enc::CharCodecKind codecKind,
		const void* p,
		size_t size
	) {
		return match(flags, enc::getCharCodec(codecKind), p, size);
	}

	RegexState
	match(
		uint_t flags,
		const void* p,
		size_t size
	) {
		return match(flags, enc::CharCodecKind_Utf8, p, size);
	}

	RegexState
	match(
		const void* p,
		size_t size
	) {
		return match(0, enc::CharCodecKind_Utf8, p, size);
	}

	RegexState
	match(
		uint_t flags,
		const sl::StringRef& string
	) {
		return match(flags, enc::CharCodecKind_Utf8, string.cp(), string.getLength());
	}

	RegexState
	match(const sl::StringRef& string) {
		return match(0, enc::CharCodecKind_Utf8, string.cp(), string.getLength());
	}

#if (_AXL_DEBUG)
	void
	printNfa() const;

	void
	printDfa() const;
#endif

protected:
	void
	finalize();

	DfaState*
	createDfaStartState(NfaState* state);

	DfaState*
	addDfaState(DfaState* state);

	bool
	loadNfa(const RegexStorageHdr* hdr);

	bool
	loadDfa(const RegexStorageHdr* hdr);
};

//..............................................................................

} // namespace re
} // namespace axl
