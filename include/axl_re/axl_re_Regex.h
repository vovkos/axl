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

#include "axl_re_State.h"
#include "axl_re_Dfa.h"

namespace axl {
namespace re {

//..............................................................................

enum CompileFlag {
	CompileFlag_CaseInsensitive = 0x01,
	CompileFlag_MatchOnly       = 0x02, // don't demux NFA and no search start states
	CompileFlag_DisableCapture  = 0x04, // same as ExecFlag_NoCapture
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum RegexKind {
	RegexKind_Undefined,
	RegexKind_Single,
	RegexKind_Switch,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Regex {
	friend class Compiler;
	friend class ExecDfaBase;
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
	sl::Array<SwitchCase> m_switchCaseArray;
	mutable sys::Lock m_dfaLock; // for when DFA is built on the fly
	mutable DfaProgram m_dfaProgram;
	mutable DfaProgram m_dfaReverseProgram;

public:
	Regex();

	bool
	isEmpty() const {
		return m_nfaProgram.m_stateList.isEmpty();
	}

	RegexKind
	getRegexKind() const {
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
	getNfaStartState() const {
		return m_nfaProgram.m_matchStartState;
	}

	const DfaState*
	getDfaState(size_t id) const {
		return id < m_dfaProgram.m_stateArray.getCount() ? m_dfaProgram.m_stateArray[id] : NULL;
	}

	const DfaState*
	getDfaMatchStartState() const {
		return
			m_dfaProgram.m_matchStartState ? m_dfaProgram.m_matchStartState :
			m_nfaProgram.m_matchStartState ? m_dfaProgram.m_matchStartState = m_dfaProgram.createStartState(m_nfaProgram.m_matchStartState) :
			NULL;
	}

	const DfaState*
	getDfaSearchStartState() const {
		return
			m_dfaProgram.m_searchStartState ? m_dfaProgram.m_searchStartState :
			m_nfaProgram.m_searchStartState ? m_dfaProgram.m_searchStartState = m_dfaProgram.createStartState(m_nfaProgram.m_searchStartState) :
			NULL;
	}

	const DfaState*
	getDfaReverseStartState() const {
		return
			m_dfaReverseProgram.m_matchStartState ? m_dfaReverseProgram.m_matchStartState :
			m_nfaProgram.m_matchStartState ? m_dfaReverseProgram.m_matchStartState = m_dfaReverseProgram.createStartState(m_nfaProgram.m_matchStartState) :
			NULL;
	}

	const DfaState*
	getDfaRollbackState(const DfaState* state) const {
		return
			state->m_rollbackState ? state->m_rollbackState :
			((DfaState*)state)->m_rollbackState = m_dfaReverseProgram.createRollbackState(state);
	}

	size_t
	getSwitchCaseCount() const {
		ASSERT(m_regexKind == RegexKind_Switch);
		return m_switchCaseArray.getCount();
	}

	const NfaState*
	getSwitchCaseNfaStartState(size_t id) const {
		ASSERT(m_regexKind == RegexKind_Switch);
		return m_switchCaseArray[id].m_nfaMatchStartState;
	}

	size_t
	getSwitchCaseCaptureCount(size_t id) const {
		ASSERT(m_regexKind == RegexKind_Switch);
		return m_switchCaseArray[id].m_captureCount;
	}

	const NfaState*
	getMatchNfaStartState(size_t acceptId) const {
		ASSERT(m_regexKind == RegexKind_Switch || acceptId == 0);
		return m_regexKind == RegexKind_Switch ?
			m_switchCaseArray[acceptId].m_nfaMatchStartState :
			m_nfaProgram.m_matchStartState;
	}

	size_t
	getMatchCaptureCount(size_t acceptId) const {
		ASSERT(m_regexKind == RegexKind_Switch || acceptId == 0);
		return m_regexKind == RegexKind_Switch ?
			m_switchCaseArray[acceptId].m_captureCount :
			m_nfaProgram.m_captureCount;
	}

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
	save(sl::Array<char>* buffer) const;

	sl::Array<char>
	save() const {
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

	void
	finalizeSwitch(uint_t flags = 0) { // only CompileFlag_MatchOnly makes sense here
		ASSERT(m_regexKind = RegexKind_Switch);
		m_nfaProgram.finalize((flags & CompileFlag_MatchOnly) != 0);
	}

	void
	buildFullDfa();

	void
	buildFullReverseDfa();

	void
	buildFullRollbackDfa();

	// execution (match/search)

	ExecResult
	exec(
		State* state,
		const void* p,
		size_t size
	) const;

	State
	exec(
		const StateInit& stateInit,
		const void* p,
		size_t size
	) const {
		State state(stateInit);
		return exec(&state, p, size) ? state : State();
	}

	State
	exec(
		const void* p,
		size_t size
	) const {
		return exec(StateInit(), p, size);
	}

	ExecResult
	exec(
		State* state,
		const sl::StringRef& string
	) const {
		return exec(state, string.cp(), string.getLength());
	}

	State
	exec(
		const StateInit& stateInit,
		const sl::StringRef& string
	) const {
		State state(stateInit);
		return exec(&state, string.cp(), string.getLength()) ? state : State();
	}

	State
	exec(const sl::StringRef& string) const {
		return exec(StateInit(), string.cp(), string.getLength());
	}

	ExecResult
	eof(State* state) const {
		ASSERT(state->getRegex() == this && (state->getExecFlags() & ExecFlag_Stream) && !state->isFinal());
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

protected:
	void
	prepareDfaState(const DfaState* state) const;
};

//..............................................................................

} // namespace re
} // namespace axl
