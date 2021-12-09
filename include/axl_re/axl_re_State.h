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

#define _AXL_RE_REGEXSTATE_H

#include "axl_re_Nfa.h"
#include "axl_re_Exec.h"

namespace axl {
namespace re {

class Regex;
class State;
class RegexEngine;

//..............................................................................

enum StreamState {
	StreamState_Idle,
	StreamState_Searching,
	StreamState_RequestBackData, // reverse DFA (on match)
	StreamState_MatchReady,
};

//..............................................................................

struct StateImpl: public rc::RefCount {
	friend class State;

	enum CharFlag {
		CharFlag_Cr    = 0x01,
		CharFlag_Lf    = 0x02,
		CharFlag_Nl    = CharFlag_Lf,
		CharFlag_Word  = 0x04,
		CharFlag_Other = 0x08, // non-zero
	};

	Regex* m_regex;
	ExecEngine* m_engine;
	enc::CharCodecKind m_codecKind;
	uint32_t m_decoderState;
	StreamState m_streamState;
	const void* m_lastExecBuffer;
	size_t m_lastExecOffset;
	size_t m_lastExecSize;
	size_t m_maxRollbackLimit; // don't rollback further than this
	uint_t m_execFlags;
	utf32_t m_prevChar;
	uint_t m_prevCharFlags;
	size_t m_offset;
	Match m_match;
	size_t m_matchAcceptId;
	sl::BoxList<Match> m_subMatchList;
	sl::Array<Match*> m_subMatchArray;

protected:
	StateImpl(); // only creatable by State

public:
	~StateImpl() {
		freeEngine();
	}

	void
	freeEngine();

	void
	initialize(
		uint_t execFlags,
		enc::CharCodecKind codecKind
	);

	StateImpl*
	clone();

	void
	postInitialize(Regex* regex);

	void
	reset();

	void
	resetMatchOffset() {
		m_match.m_offset = m_offset;
	}

	static
	uint_t
	calcCharFlags(utf32_t c);

	static
	uint_t
	calcAnchors(
		uint_t prevCharFlags,
		uint_t charFlags
	);

	uint_t
	calcAnchorsUpdateCharFlags(utf32_t c);

	void
	createMatch(
		size_t acceptId,
		const MatchPos& matchPos,
		const sl::ArrayRef<MatchPos>& capturePosArray = sl::ArrayRef<MatchPos>()
	);

	bool
	exec(
		const void* p,
		size_t size
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
uint_t
StateImpl::calcCharFlags(utf32_t c) {
	return
		c == '\r' ? CharFlag_Cr :
		c == '\n' ? CharFlag_Lf :
		c == '_' || enc::isLetterOrDigit(c) ? CharFlag_Word :
		CharFlag_Other;
}

inline
uint_t
StateImpl::calcAnchors(
	uint_t prevCharFlags,
	uint_t charFlags
) {
	uint_t anchors = 0;

	if (prevCharFlags & CharFlag_Lf)
		anchors |= Anchor_BeginLine;

	if (charFlags & (CharFlag_Cr | CharFlag_Lf))
		anchors |= Anchor_EndLine;

	if ((prevCharFlags & CharFlag_Word) ^ (charFlags & CharFlag_Word))
		anchors |= Anchor_WordBoundary;
	else
		anchors |= Anchor_NotWordBoundary;

	return anchors;
}

inline
uint_t
StateImpl::calcAnchorsUpdateCharFlags(utf32_t c) {
	uint_t charFlags = calcCharFlags(c);
	uint_t anchors = m_prevCharFlags ?
		calcAnchors(m_prevCharFlags, charFlags) :
		calcAnchors(calcCharFlags(m_prevChar), calcCharFlags(c));

	m_prevCharFlags = charFlags;
	return anchors;
}

//..............................................................................

class State {
	friend class Regex;
	friend class Match;

protected:
	rc::Ptr<StateImpl> m_p;

public:
	State() {}

#if (_AXL_CPP_HAS_RVALUE_REF)
	State(State&& src) {
		m_p.move(std::move(src.m_p));
	}
#endif

	State(const State& src) {
		m_p.copy(src.m_p);
	}

	State(
		uint32_t flags,
		enc::CharCodecKind codecKind = enc::CharCodecKind_Utf8
	) {
		initialize(flags, codecKind);
	}

	operator bool () const {
		return m_p;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	State&
	operator = (State&& src) {
		m_p.move(std::move(src.m_p));
		return *this;
	}
#endif

	State&
	operator = (const State& src) {
		m_p.copy(src.m_p);
		return *this;
	}

	const Regex*
	getRegex() const {
		ASSERT(m_p);
		return m_p->m_regex;
	}

	enc::CharCodecKind
	getCodecKind() const {
		ASSERT(m_p);
		return m_p->m_codecKind;
	}

	uint_t
	getExecFlags() const {
		ASSERT(m_p);
		return m_p->m_execFlags;
	}

	StreamState
	getStreamState() const {
		ASSERT(m_p);
		return m_p->m_streamState;
	}

	bool
	isMatch() const {
		ASSERT(m_p);
		return m_p->m_matchAcceptId != -1;
	}

	size_t
	getMatchSwitchCaseId() const {
		ASSERT(m_p);
		return m_p->m_matchAcceptId;
	}

	const Match*
	getMatch() const {
		ASSERT(m_p);
		return isMatch() ? &m_p->m_match : NULL;
	}

	size_t
	getSubMatchCount() const {
		ASSERT(m_p);
		return m_p->m_subMatchArray.getCount();
	}

	const Match*
	getSubMatch(size_t i) const {
		ASSERT(m_p);
		return i < m_p->m_subMatchArray.getCount() ? m_p->m_subMatchArray[i] : NULL;
	}

	void
	initialize(
		uint_t execFlags = 0,
		enc::CharCodecKind codecKind = enc::CharCodecKind_Utf8
	);

	void
	reset(size_t offset = 0);

protected:
	void
	ensureExclusive() {
		ASSERT(m_p);
		if (!m_p.isExclusive())
			m_p = m_p->clone();
	}


	void
	initialize(
		uint_t execFlags,
		enc::CharCodecKind codecKind,
		Regex* regex
	);

	void
	postInitialize(Regex* regex) {
		ensureExclusive();
		m_p->postInitialize(regex);
	}

	bool
	exec(
		const void* p,
		size_t size
	) {
		ensureExclusive();
		return m_p->exec(p, size);
	}

	bool
	eof() {
		ensureExclusive();
		return m_p->m_engine->eof();
	}
};

//..............................................................................

} // namespace re
} // namespace axl
