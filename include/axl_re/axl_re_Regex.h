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
	RegexCompileFlag_MatchOnly       = 0x10, // don't demux NFA and no search start states
	RegexCompileFlag_DisableCapture  = 0x20, // same as RegexExecFlag_NoCapture
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Regex {
	friend class RegexCompiler;
	friend class NfaDemuxer;
	friend class DfaBuilder;

protected:
	struct SwitchCase {
		size_t m_captureCount;
		const NfaState* m_nfaMatchStartState;
		const DfaState* m_dfaMatchStartState;

		SwitchCase();
	};

protected:
	RegexKind m_regexKind;
	NfaProgram m_nfaProgram;
	DfaProgram m_dfaProgram;
	DfaProgram m_dfaReverseProgram;
	sl::Array<SwitchCase> m_switchCaseArray;

public:
	Regex();

	bool
	isEmpty() const {
		return m_nfaProgram.m_stateList.isEmpty();
	}

	RegexKind
	getRegexKind() {
		return m_regexKind;
	}

	size_t
	getCaptureCount() const {
		return m_nfaProgram.m_captureCount;
	}

	size_t
	getNfaStateCount() const {
		return m_nfaProgram.m_stateList.getCount();
	}

	const NfaState*
	getNfaMatchStartState() const {
		return m_nfaProgram.m_matchStartState;
	}

	const NfaState*
	getNfaSearchStartState() const {
		return m_nfaProgram.m_searchStartState;
	}

	const DfaState*
	getDfaMatchStartState() {
		return
			m_dfaProgram.m_matchStartState ? m_dfaProgram.m_matchStartState :
			m_nfaProgram.m_matchStartState ? m_dfaProgram.m_matchStartState = m_dfaProgram.createStartState(m_nfaProgram.m_matchStartState) :
			NULL;
	}

	const DfaState*
	getDfaSearchStartState() {
		return
			m_dfaProgram.m_searchStartState ? m_dfaProgram.m_searchStartState :
			m_nfaProgram.m_searchStartState ? m_dfaProgram.m_searchStartState = m_dfaProgram.createStartState(m_nfaProgram.m_searchStartState) :
			NULL;
	}

	const DfaState*
	getDfaReverseMatchStartState() {
		return
			m_dfaReverseProgram.m_matchStartState ? m_dfaReverseProgram.m_matchStartState :
			m_nfaProgram.m_matchStartState ? m_dfaReverseProgram.m_matchStartState = m_dfaReverseProgram.createStartState(m_nfaProgram.m_matchStartState) :
			NULL;
	}

	size_t
	getSwitchCaseCount() {
		ASSERT(m_regexKind == RegexKind_Switch);
		return m_switchCaseArray.getCount();
	}

	const NfaState*
	getSwitchCaseNfaMatchStartState(size_t id) {
		ASSERT(m_regexKind == RegexKind_Switch);
		return m_switchCaseArray[id].m_nfaMatchStartState;
	}

	const DfaState*
	getSwitchCaseDfaMatchStartState(size_t id);

	void
	clear();

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
	save(sl::Array<char>* buffer);

	sl::Array<char>
	save() {
		sl::Array<char> buffer;
		save(&buffer);
		return buffer;
	}

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
	createSwitch(size_t caseCountHint = 4);

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
	finalizeSwitch(uint_t flags = 0) { // only RegexCompileFlag_MatchOnly makes sense here
		ASSERT(m_regexKind = RegexKind_Switch);
		m_nfaProgram.finalize((flags & RegexCompileFlag_MatchOnly) != 0);
	}

	void
	buildFullDfa();

	void
	buildFullReverseDfa();

	// execution (match/search)

	bool
	exec(
		RegexState* state,
		const void* p,
		size_t size
	);

	RegexState
	exec(
		uint_t flags,
		enc::CharCodec* codec,
		const void* p,
		size_t size
	) {
		RegexState state(flags, codec);
		return exec(&state, p, size) ? state : RegexState();
	}

	RegexState
	exec(
		uint_t flags,
		enc::CharCodecKind codecKind,
		const void* p,
		size_t size
	) {
		return exec(flags, enc::getCharCodec(codecKind), p, size);
	}

	RegexState
	exec(
		uint_t flags,
		const void* p,
		size_t size
	) {
		return exec(flags, enc::CharCodecKind_Utf8, p, size);
	}

	RegexState
	exec(
		const void* p,
		size_t size
	) {
		return exec(0, enc::CharCodecKind_Utf8, p, size);
	}

	RegexState
	exec(
		uint_t flags,
		const sl::StringRef& string
	) {
		return exec(flags, enc::CharCodecKind_Utf8, string.cp(), string.getLength());
	}

	RegexState
	exec(const sl::StringRef& string) {
		return exec(0, enc::CharCodecKind_Utf8, string.cp(), string.getLength());
	}

	bool
	eof(RegexState* state) {
		ASSERT(state->getRegex() == this && (state->getExecFlags() & RegexExecFlag_Stream));
		return state->eof();
	}

#if (_AXL_DEBUG)
	// debug printing

	void
	printNfa(FILE* file = stdout) const {
		m_nfaProgram.print(file);
	}

	void
	printDfa(FILE* file = stdout) const {
		m_dfaProgram.print(file);
	}

	void
	printReverseDfa(FILE* file = stdout) const {
		m_dfaReverseProgram.print(file);
	}
#endif
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const DfaState*
Regex::getSwitchCaseDfaMatchStartState(size_t id) {
	ASSERT(m_regexKind == RegexKind_Switch);

	SwitchCase& scase = m_switchCaseArray[id];
	ASSERT(scase.m_nfaMatchStartState);

	return
		scase.m_dfaMatchStartState ? scase.m_dfaMatchStartState :
		scase.m_dfaMatchStartState = m_dfaProgram.createStartState(scase.m_nfaMatchStartState);
}

//..............................................................................

inline
DfaBuilder::DfaBuilder(Regex* regex) {
	m_program = &regex->m_dfaProgram;
}

//..............................................................................

} // namespace re
} // namespace axl
