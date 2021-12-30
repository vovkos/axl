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
#include "axl_re_Match.h"
#include "axl_re_Exec.h"

namespace axl {
namespace re {

class Regex;
class State;

//..............................................................................

struct StateInit {
	uint_t m_execFlags;
	enc::CharCodecKind m_codecKind;
	enc::DecoderState m_decoderState;
	uint_t m_prevCharFlags;
	size_t m_offset;
	uint_t m_baseCharFlags;
	size_t m_baseOffset;
	size_t m_dfaStateId;

	StateInit() {
		setup(0, 0, enc::CharCodecKind_Utf8);
	}

	StateInit(
		uint_t execFlags,
		size_t offset = 0,
		enc::CharCodecKind codecKind = enc::CharCodecKind_Utf8
	) {
		setup(execFlags, offset, codecKind);
	}

	void
	setup(
		uint_t execFlags,
		size_t offset = 0,
		enc::CharCodecKind codecKind = enc::CharCodecKind_Utf8
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
StateInit::setup(
	uint_t execFlags,
	size_t offset,
	enc::CharCodecKind codecKind
) {
	m_execFlags = execFlags;
	m_codecKind = codecKind;
	m_prevCharFlags = Anchor_BeginLine | Anchor_BeginText | Anchor_WordBoundary;
	m_offset = offset;
	m_baseCharFlags = Anchor_BeginLine | Anchor_BeginText | Anchor_WordBoundary;
	m_baseOffset = offset;
	m_dfaStateId = -1;
}

//..............................................................................

struct StateImpl: public rc::RefCount {
	friend class State;

	const Regex* m_regex;
	ExecEngine* m_engine;
	StateInit m_init;
	size_t m_matchAcceptId;
	Match m_match;
	sl::BoxList<Match> m_captureList;
	sl::Array<Match*> m_captureArray;

protected:
	StateImpl(); // only creatable by State

public:
	~StateImpl() {
		freeEngine();
	}

	void
	freeEngine();

	StateImpl*
	clone();

	void
	initialize(
		const StateInit& init,
		const Regex* regex
	);

	void
	setRegex(const Regex* regex);

	void
	preCreateMatch(
		size_t acceptId,
		size_t endOffset
	);

	void
	createMatch(
		size_t acceptId,
		size_t lastExecOffset,
		const void* lastExecData,
		const MatchPos& matchPos,
		const sl::ArrayRef<MatchPos>& capturePosArray = sl::ArrayRef<MatchPos>()
	);
};

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

	State(const StateInit& init) {
		initialize(init);
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

	bool
	isStream() const {
		ASSERT(m_p);
		return (m_p->m_init.m_execFlags & ExecFlag_Stream) != 0;
	}

	bool
	isFinal() const {
		ASSERT(m_p && m_p->m_engine);
		return m_p->m_engine->isFinalized();
	}

	bool
	isPreMatch() const {
		ASSERT(m_p);
		return m_p->m_matchAcceptId != -1;
	}

	bool
	isMatch() const {
		ASSERT(m_p);
		return m_p->m_match.getOffset() != -1;
	}

	const Regex*
	getRegex() const {
		ASSERT(m_p);
		return m_p->m_regex;
	}

	enc::CharCodecKind
	getCodecKind() const {
		ASSERT(m_p);
		return m_p->m_init.m_codecKind;
	}

	uint_t
	getExecFlags() const {
		ASSERT(m_p);
		return m_p->m_init.m_execFlags;
	}

	ExecResult
	getLastExecResult() const {
		ASSERT(m_p && m_p->m_engine);
		return m_p->m_engine->getExecResult();
	}

	size_t
	getDfaStateId() const;

	uint_t
	getPrevCharFlags() const {
		ASSERT(m_p && m_p->m_engine);
		return m_p->m_engine->getPrevCharFlags();
	}

	enc::DecoderState
	getDecoderState() {
		ASSERT(m_p && m_p->m_engine);
		return m_p->m_engine->getDecoderState();
	}

	size_t
	getMatchAcceptId() const {
		ASSERT(m_p);
		return m_p->m_matchAcceptId;
	}

	const Match*
	getMatch() const {
		ASSERT(m_p);
		return isPreMatch() ? &m_p->m_match : NULL;
	}

	size_t
	getCaptureCount() const {
		ASSERT(m_p);
		return m_p->m_captureArray.getCount();
	}

	const Match*
	getCapture(size_t i) const {
		ASSERT(m_p);
		return i < m_p->m_captureArray.getCount() ? m_p->m_captureArray[i] : NULL;
	}

	const Match*
	getGroup(size_t i) const {
		return i == 0 ? getMatch() : getCapture(i - 1);
	}

	void
	initialize() {
		initialize(StateInit());
	}

	void
	initialize(const StateInit& init);

	void
	reset(size_t offset = 0);

	void
	resume() {
		ASSERT(isPreMatch());
		reset(m_p->m_match.getEndOffset());
	}

protected:
	void
	ensureExclusive() {
		ASSERT(m_p);
		if (!m_p.isExclusive())
			m_p = m_p->clone();
	}

	void
	initialize(
		const StateInit& init,
		const Regex* regex
	);

	void
	setRegex(const Regex* regex) {
		ensureExclusive();
		m_p->setRegex(regex);
	}

	ExecResult
	exec(
		const void* p,
		size_t size
	);

	ExecResult
	eof();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ExecResult
State::exec(
	const void* p,
	size_t size
) {
	ensureExclusive();
	size ?
		m_p->m_engine->exec(p, size) :
		m_p->m_engine->eof();
	return m_p->m_engine->getExecResult();
}

inline
ExecResult
State::eof() {
	ensureExclusive();
	m_p->m_engine->eof();
	return m_p->m_engine->getExecResult();
}

//..............................................................................

} // namespace re
} // namespace axl
