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

// if no streaming is required and we are matching from the beginning of the text
// then  we can start with NFA; it will work faster if there is a match

// #define _AXL_RE_CAN_SKIP_DFA 1
#define _AXL_RE_CAN_SKIP_DFA 0

//..............................................................................

StateImpl::StateImpl() {
	m_regex = NULL;
	m_engine = NULL;
	m_execFlags = 0;
	m_matchAcceptId = -1;
}

void
StateImpl::freeEngine() {
	if (m_engine) {
		AXL_MEM_DELETE(m_engine);
		m_engine = NULL;
	}
}

void
StateImpl::initialize(
	uint_t execFlags,
	enc::CharCodecKind codecKind
) {
	freeEngine();

	m_regex = NULL;
	m_codecKind = codecKind;
	m_execFlags = execFlags;
}

StateImpl*
StateImpl::clone() {
	StateImpl* state = AXL_MEM_NEW(StateImpl);

	state->m_regex = m_regex;
	state->m_engine = m_engine->clone(state);
	state->m_codecKind = m_codecKind;
	state->m_execFlags = m_execFlags;

	if (m_matchAcceptId == -1) // we are done
		return state;

	state->m_match = m_match;
	state->m_matchAcceptId = m_matchAcceptId;

	size_t count = m_subMatchArray.getCount();
	ASSERT(count == state->m_subMatchList.getCount() + 1);
	state->m_subMatchArray.setCount(count);
	state->m_subMatchArray[0] = &state->m_match;

	sl::ConstBoxIterator<Match> it = m_subMatchList.getHead();
	for (size_t i = 1; it; it++, i++) {
		Match* match = state->m_subMatchList.insertTail().p();
		*match = *it;
		state->m_subMatchArray[i] = match;
	}

	return state;
}

void
StateImpl::postInitialize(
	Regex* regex,
	size_t offset
) {
	ASSERT(!m_regex && !m_engine);

	m_regex = regex;

#if (_AXL_RE_CAN_SKIP_DFA)
	if (regex->getRegexKind() != RegexKind_Switch &&
		(m_execFlags & (ExecFlag_Stream | ExecFlag_AnchorDataBegin)) == ExecFlag_AnchorDataBegin)
		m_engine = createExecNfaVm(this);
	else
		m_engine = createExecDfa(this);
#else
	m_engine = createExecDfa(this);
#endif

	reset(offset);
}

void
StateImpl::reset(size_t offset) {
	ASSERT(m_engine);

	m_matchAcceptId = -1;
	m_match.m_offset = -1;
	m_match.m_endOffset = -1;
	m_subMatchList.clear();
	m_subMatchArray.clear();
	m_engine->reset(offset);
}

void
StateImpl::createMatch(
	size_t acceptId,
	size_t lastExecOffset,
	const void* lastExecData,
	const MatchPos& matchPos,
	const sl::ArrayRef<MatchPos>& capturePosArray
) {
	ASSERT(m_matchAcceptId == -1);

	m_matchAcceptId = acceptId;
	m_match.m_offset = matchPos.m_offset;
	m_match.m_endOffset = matchPos.m_endOffset;
	m_match.m_text.clear();

	char* base;

	if (lastExecData) {
		base = (char*)lastExecData - lastExecOffset;
		m_match.m_codec = enc::getCharCodec(m_codecKind);
		m_match.m_p = base + m_match.m_offset;
	}

	size_t count = capturePosArray.getCount();
	if (!count || (m_execFlags & ExecFlag_DisableCapture)) {
		m_subMatchArray.copy(&m_match);
		return;
	}

	m_subMatchArray.setCountZeroConstruct(count + 1);
	m_subMatchArray[0] = &m_match;

	for (size_t i = 0; i < count; i++) {
		const MatchPos& pos = capturePosArray[i];
		if (pos.m_endOffset == -1)
			continue;

		ASSERT(pos.m_offset != -1 && pos.m_offset <= pos.m_endOffset);

		Match* match = m_subMatchList.insertTail().p();
		match->m_offset = pos.m_offset;
		match->m_endOffset = pos.m_endOffset;

		if (lastExecData) {
			match->m_codec = m_match.m_codec;
			match->m_p = base + pos.m_offset;
		}

		m_subMatchArray[i + 1] = match;
	}
}

//..............................................................................

void
State::initialize(
	uint_t execFlags,
	enc::CharCodecKind codecKind
) {
	if (!m_p)
		m_p = AXL_RC_NEW(StateImpl);
	else
		ASSERT(m_p.isExclusive());

	m_p->initialize(execFlags, codecKind);
}

void
State::initialize(
	uint_t execFlags,
	enc::CharCodecKind codecKind,
	Regex* regex
) {
	ASSERT(!m_p);
	m_p = AXL_RC_NEW(StateImpl);
	m_p->initialize(execFlags, codecKind);
	m_p->postInitialize(regex, 0);
}

void
State::reset(size_t offset) {
	ASSERT(m_p);

	if (m_p.isExclusive()) {
		m_p->reset(offset);
	} else {
		rc::Ptr<StateImpl> p;
		sl::takeOver(&p, &m_p);

		m_p = AXL_RC_NEW(StateImpl);
		m_p->initialize(p->m_execFlags, p->m_codecKind);
		m_p->postInitialize(p->m_regex, offset);
	}
}

//..............................................................................

} // namespace re
} // namespace axl
