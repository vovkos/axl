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

struct StateImpl: public rc::RefCount {
	friend class State;

	enum ExecFlag {
		ExecFlag_ReverseStream = 0x100, // don't overlap with RegexExecFlag
	};

	Regex* m_regex;
	ExecEngine* m_engine;
	enc::CharCodecKind m_codecKind;
	uint_t m_execFlags;

	size_t m_matchAcceptId;
	Match m_match;
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
	reverseStream() {
		m_execFlags |= ExecFlag_ReverseStream;
	}

	void
	initialize(
		uint_t execFlags,
		enc::CharCodecKind codecKind
	);

	StateImpl*
	clone();

	void
	postInitialize(
		Regex* regex,
		size_t offset
	);

	void
	reset(size_t offset);

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

	bool
	isReverseStream() const {
		ASSERT(m_p);
		return (m_p->m_execFlags & (StateImpl::ExecFlag_ReverseStream)) != 0;
	}

	bool
	getLastExecResult() const {
		return m_p->m_engine->getExecResult();
	}

	bool
	isFinal() const {
		ASSERT(m_p);
		return m_p->m_engine->isFinalized();
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
		m_p->postInitialize(regex, 0);
	}

	bool
	exec(
		const void* p,
		size_t size
	);

	bool
	eof();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
State::exec(
	const void* p,
	size_t size
) {
	ensureExclusive();
	m_p->m_engine->preExec(p, size);
	m_p->m_engine->exec(p, size);
	return m_p->m_engine->getExecResult();
}

inline
bool
State::eof() {
	ASSERT(!m_p->m_engine->isFinalized());
	ensureExclusive();
	return m_p->m_engine->eof();
}

//..............................................................................

// need a definition of StateImpl

inline
ExecNfaEngine::ExecNfaEngine(StateImpl* parent):
	ExecEngine(parent) {
	m_execFlags = parent->m_execFlags;
}

//..............................................................................

} // namespace re
} // namespace axl
