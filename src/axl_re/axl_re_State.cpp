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
	m_lastExecBuffer = NULL;
	m_lastExecOffset = 0;
	m_lastExecSize = 0;
	m_execFlags = 0;
	m_prevCharFlags = 0;
	m_offset = 0;
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
	enc::CharCodec* codec
) {
	freeEngine();

	m_regex = NULL;
	m_decoder.setup(codec);
	m_execFlags = execFlags;
	m_prevCharFlags = StateImpl::CharFlag_Lf;
}

void
StateImpl::postInitialize(Regex* regex) {
	ASSERT(!m_regex && !m_engine);

	m_regex = regex;

	switch (m_execFlags & RegexExecFlag_EngineMask) {
	case 0: // test default
	case RegexExecFlag_Dfa:
		m_engine = AXL_MEM_NEW_ARGS(ExecDfa, (this));
		break;

	case RegexExecFlag_NfaSp:
		m_engine = AXL_MEM_NEW_ARGS(ExecNfaSp, (this));
		break;

	case RegexExecFlag_NfaVm:
		m_engine = AXL_MEM_NEW_ARGS(ExecNfaVm, (this));
		break;

	default:
		ASSERT(false);
		m_engine = AXL_MEM_NEW_ARGS(ExecNfaVm, (this)); // fall back to threading NFA
	}

	reset();
}

axl::lex::LineCol lc;

void
StateImpl::reset() {
	ASSERT(m_engine);

	m_matchAcceptId = -1;
	m_match.m_offset = m_offset;
	m_match.m_endOffset = -1;
	m_subMatchList.clear();
	m_subMatchArray.clear();
	m_engine->reset();
}

void
StateImpl::createMatch(
	size_t acceptId,
	const MatchPos& matchPos,
	const sl::ArrayRef<MatchPos>& capturePosArray
) {
	ASSERT(m_matchAcceptId == -1);

	m_matchAcceptId = acceptId;
	m_match.m_offset = matchPos.m_offset;
	m_match.m_endOffset = matchPos.m_endOffset;

	if (!(m_execFlags & RegexExecFlag_Stream)) {
		m_match.m_charCodec = m_decoder.getCharCodec();
		m_match.m_p = (char*)m_lastExecBuffer + m_match.m_offset - m_lastExecOffset;
	}

	size_t count = capturePosArray.getCount();

	if (!count || (m_execFlags & RegexExecFlag_DisableCapture)) {
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

		if (!(m_execFlags & RegexExecFlag_Stream)) {
			match->m_charCodec = m_decoder.getCharCodec();
			match->m_p = (char*)m_lastExecBuffer + pos.m_offset - m_lastExecOffset;
		}

		m_subMatchArray[i + 1] = match;
	}
}

bool
StateImpl::exec(
	const void* p,
	size_t size
) {
	m_lastExecBuffer = p;
	m_lastExecOffset = m_offset;
	m_lastExecSize = size;

	return m_engine->exec(p, size);
}

//..............................................................................

void
State::initialize(
	uint_t execFlags,
	enc::CharCodec* codec
) {
	if (!m_p)
		m_p = AXL_RC_NEW(StateImpl);
	else
		ASSERT(m_p.isExclusive());

	m_p->initialize(execFlags, codec);
}

void
State::initialize(Regex* regex) {
	ASSERT(!m_p);
	m_p = AXL_RC_NEW(StateImpl);
	m_p->initialize(0, enc::getCharCodec(enc::CharCodecKind_Utf8));
	m_p->postInitialize(regex);
}

void
State::reset(size_t offset) {
	ASSERT(m_p);

	if (m_p.isExclusive()) {
		m_p->m_offset = offset;
		m_p->reset();
	} else {
		rc::Ptr<StateImpl> p;
		sl::takeOver(&p, &m_p);

		m_p = AXL_RC_NEW(StateImpl);
		m_p->initialize(p->m_execFlags, p->m_decoder.getCharCodec());
		m_p->m_offset = offset;
		m_p->postInitialize(p->m_regex);
	}
}

//..............................................................................

} // namespace re
} // namespace axl
