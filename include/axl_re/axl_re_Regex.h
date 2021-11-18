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
	struct SwitchCaseContext: sl::ListLink {
		size_t m_baseCaptureId;
		size_t m_captureCount;
		const NfaState* m_nfaMatchStartState;
		const DfaState* m_dfaMatchStartState;
	};

protected:
	RegexKind m_regexKind;
	size_t m_startCount;
	size_t m_captureCount;
	NfaState* m_nfaMatchStartState;
	NfaState* m_nfaSearchStartState;
	DfaState* m_dfaMatchStartState;
	DfaState* m_dfaSearchStartState;
	sl::List<NfaState> m_nfaStateList;
	sl::List<DfaState> m_dfaStateList;
	sl::List<DfaState> m_preDfaStateList;
	NfaStateSetMap<DfaState*> m_dfaStateMap;
	sl::List<SwitchCaseContext> m_switchCaseContextList;
	sl::Array<SwitchCaseContext*> m_switchCaseContextArray;

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
	getNfaMatchStartState() const {
		return m_nfaMatchStartState;
	}

	const NfaState*
	getNfaSearchStartState() const {
		return m_nfaSearchStartState;
	}

	const DfaState*
	getDfaMatchStartState() {
		return
			m_dfaMatchStartState ? m_dfaMatchStartState :
			m_nfaMatchStartState ? m_dfaMatchStartState = createDfaStartState(m_nfaMatchStartState) :
			NULL;
	}

	const DfaState*
	getDfaSearchStartState() {
		return
			m_dfaSearchStartState ? m_dfaSearchStartState :
			m_nfaSearchStartState ? m_dfaSearchStartState = createDfaStartState(m_nfaSearchStartState) :
			NULL;
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
	getSwitchCaseNfaMatchStartState(size_t id) {
		ASSERT(m_regexKind == RegexKind_Switch);
		return m_switchCaseContextArray[id]->m_nfaMatchStartState;
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
	finalizeSwitch(uint_t flags = 0) { // only RegexCompileFlag_MatchOnly makes sense here
		ASSERT(m_regexKind = RegexKind_Switch);
		finalize(flags);
	}

	void
	buildFullDfa();

#if (_AXL_DEBUG)
	void
	printNfa() const;

	void
	printDfa() const;
#endif

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

protected:
	void
	finalize(uint_t flags);

	DfaState*
	createDfaStartState(const NfaState* state);

	DfaState*
	addDfaState(DfaState* state);

	bool
	loadNfa(const RegexStorageHdr* hdr);

	bool
	loadDfa(const RegexStorageHdr* hdr);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const DfaState*
Regex::getSwitchCaseDfaMatchStartState(size_t id) {
	ASSERT(m_regexKind == RegexKind_Switch);

	SwitchCaseContext* context = m_switchCaseContextArray[id];
	ASSERT(context->m_nfaMatchStartState);

	return
		context->m_dfaMatchStartState ? context->m_dfaMatchStartState :
		context->m_dfaMatchStartState = createDfaStartState(context->m_nfaMatchStartState);
}

//..............................................................................

} // namespace re
} // namespace axl
