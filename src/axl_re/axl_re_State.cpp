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

#include "pch.h"
#include "axl_re_State.h"
#include "axl_re_Regex.h"
#include "axl_re_ExecNfaSp.h"
#include "axl_re_ExecNfaVm.h"
#include "axl_re_ExecDfa.h"

namespace axl {
namespace re {

//..............................................................................

StateImpl::StateImpl() {
	m_regex = NULL;
	m_engine = NULL;
	m_matchAcceptId = -1;
}

void
StateImpl::freeEngine() {
	if (m_engine) {
		delete m_engine;
		m_engine = NULL;
	}
}

StateImpl*
StateImpl::clone() {
	StateImpl* state = new StateImpl;

	state->m_regex = m_regex;
	state->m_engine = m_engine->clone(state);
	state->m_init = m_init;
	state->m_matchAcceptId = m_matchAcceptId;
	state->m_match = m_match;

	if (m_match.getOffset() == -1) // we are done
		return state;

	size_t count = m_captureArray.getCount();
	state->m_captureArray.setCount(count);
	sl::Array<Match*>::Rwi rwi = state->m_captureArray;
	sl::ConstBoxIterator<Match> it = m_captureList.getHead();
	for (size_t i = 0; it; it++, i++) {
		Match* match = state->m_captureList.insertTail().p();
		*match = *it;
		rwi[i] = match;
	}

	return state;
}

void
StateImpl::initialize(
	const StateInit& init,
	const Regex* regex
) {
	m_init = init;

	if (!regex)
		m_regex = NULL;
	else
		setRegex(regex);
}

void
StateImpl::setRegex(const Regex* regex) {
	ASSERT(regex);

	m_regex = regex;
	m_matchAcceptId = -1;
	m_match.m_offset = -1;
	m_match.m_endOffset = -1;
	m_captureList.clear();
	m_captureArray.clear();

	freeEngine();

	const DfaState* dfaState = NULL;

	if (m_init.m_execFlags & ExecFlag_Reverse) {
		if (m_init.m_dfaStateId != -1) {
			dfaState = m_regex->getDfaReverseState(m_init.m_dfaStateId);
			if (!dfaState)
				AXL_TRACE("invalid reverse DFA state %d\n", m_init.m_dfaStateId);
		}

		if (!dfaState)
			dfaState = m_regex->getDfaReverseStartState();

		ASSERT(dfaState);
		m_engine = createExecDfaReverse(this);
	} else {
		if (m_init.m_dfaStateId != -1) {
			dfaState = m_regex->getDfaState(m_init.m_dfaStateId);
			if (!dfaState)
				AXL_TRACE("invalid DFA state %d\n", m_init.m_dfaStateId);
		}

		if (!dfaState)
			dfaState = (m_init.m_execFlags & ExecFlag_AnchorDataBegin) || m_regex->isMatchOnly() ?
				m_regex->getDfaMatchStartState() :
				m_regex->getDfaSearchStartState();

		ASSERT(dfaState);
		m_engine = createExecDfa(this);
	}

	((ExecDfaBase*)m_engine)->initialize(m_init, dfaState);
}

void
StateImpl::preCreateMatch(
	size_t acceptId,
	uint64_t endOffset
) {
	ASSERT(m_matchAcceptId == -1);

	m_matchAcceptId = acceptId;
	m_match.m_endOffset = endOffset;
}

void
StateImpl::createMatch(
	size_t acceptId,
	uint64_t lastExecOffset,
	const void* lastExecData,
	const MatchPos& matchPos,
	const sl::ArrayRef<MatchPos>& capturePosArray
) {
	ASSERT(m_match.getOffset() == -1);

	m_matchAcceptId = acceptId;
	m_match.m_offset = matchPos.m_offset;
	m_match.m_endOffset = matchPos.m_endOffset;
	m_match.m_text.clear();

	if (lastExecData && matchPos.m_offset >= lastExecOffset) {
		m_match.m_p = (char*)lastExecData + (size_t)(m_match.m_offset - lastExecOffset);
		m_match.m_codec = enc::getCharCodec(m_init.m_codecKind);
	}

	size_t count = capturePosArray.getCount();
	if (!count || (m_init.m_execFlags & ExecFlag_DisableCapture))
		return;

	m_captureArray.setCountZeroConstruct(count);
	sl::Array<Match*>::Rwi rwi = m_captureArray;
	for (size_t i = 0; i < count; i++) {
		const MatchPos& pos = capturePosArray[i];
		if (pos.m_endOffset == -1)
			continue;

		ASSERT(pos.m_offset != -1 && pos.m_offset <= pos.m_endOffset);

		Match* match = m_captureList.insertTail().p();
		match->m_offset = pos.m_offset;
		match->m_endOffset = pos.m_endOffset;

		if (lastExecData && pos.m_offset >= lastExecOffset) {
			match->m_codec = m_match.m_codec;
			match->m_p = (char*)lastExecData + (size_t)(pos.m_offset - lastExecOffset);
		}

		rwi[i] = match;
	}
}

//..............................................................................

size_t
State::getDfaStateId() const {
	ASSERT(m_p);

	if (!m_p->m_engine)
		return m_p->m_init.m_dfaStateId;

	ASSERT(m_p->m_engine->getEngineKind() == ExecEngineKind_Dfa);
	return ((ExecDfaBase*)m_p->m_engine)->getDfaStateId();
}

void
State::initialize(const StateInit& init) {
	if (!m_p) {
		m_p = AXL_RC_NEW(StateImpl);
		m_p->initialize(init, NULL);
	} else {
		ASSERT(m_p.isExclusive());
		m_p->initialize(init, m_p->m_regex);
	}
}

void
State::initialize(
	const StateInit& init,
	const Regex* regex
) {
	ASSERT(!m_p);
	m_p = AXL_RC_NEW(StateImpl);
	m_p->initialize(init, regex);
}

void
State::reset(uint64_t offset) {
	ASSERT(m_p);

	StateInit init(m_p->m_init.m_execFlags, offset, m_p->m_init.m_codecKind);
	m_p->initialize(init, m_p->m_regex);
}

//..............................................................................

} // namespace re
} // namespace axl
