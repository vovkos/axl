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
	getNfaStartState() const {
		return m_nfaProgram.m_matchStartState;
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
	getDfaReverseStartState() {
		return
			m_dfaReverseProgram.m_matchStartState ? m_dfaReverseProgram.m_matchStartState :
			m_nfaProgram.m_matchStartState ? m_dfaReverseProgram.m_matchStartState = m_dfaReverseProgram.createStartState(m_nfaProgram.m_matchStartState) :
			NULL;
	}

	const DfaState*
	getDfaRollbackState(const DfaState* state) {
		return
			state->m_rollbackState ? state->m_rollbackState :
			((DfaState*)state)->m_rollbackState = m_dfaReverseProgram.createRollbackState(state);
	}

	size_t
	getSwitchCaseCount() {
		ASSERT(m_regexKind == RegexKind_Switch);
		return m_switchCaseArray.getCount();
	}

	const NfaState*
	getSwitchCaseNfaStartState(size_t id) {
		ASSERT(m_regexKind == RegexKind_Switch);
		return m_switchCaseArray[id].m_nfaMatchStartState;
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
	createSwitch() {
		clear();
		m_regexKind = RegexKind_Switch;
	}

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
	finalizeSwitch(uint_t flags = 0) { // only CompileFlag_MatchOnly makes sense here
		ASSERT(m_regexKind = RegexKind_Switch);
		m_nfaProgram.finalize((flags & CompileFlag_MatchOnly) != 0);
	}

	void
	prepareDfaState(const DfaState* state);

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
	);

	State
	exec(
		const StateInit& stateInit,
		const void* p,
		size_t size
	) {
		State state(stateInit);
		return exec(&state, p, size) ? state : State();
	}

	State
	exec(
		const void* p,
		size_t size
	) {
		return exec(StateInit(), p, size);
	}

	State
	exec(
		uint_t flags,
		const sl::StringRef& string
	) {
		return exec(StateInit(flags), string.cp(), string.getLength());
	}

	State
	exec(const sl::StringRef& string) {
		return exec(StateInit(), string.cp(), string.getLength());
	}

	ExecResult
	eof(State* state) {
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
};

//..............................................................................

} // namespace re
} // namespace axl
