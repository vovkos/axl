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
#include "axl_re_RegexState.h"
#include "axl_re_Regex.h"
#include "axl_re_RegexExecNfaBt.h"
#include "axl_re_RegexExecNfaSp.h"
#include "axl_re_RegexExecNfaVm.h"
#include "axl_re_RegexExecDfa.h"

namespace axl {
namespace re {

//..............................................................................

RegexStateImpl::RegexStateImpl() {
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
RegexStateImpl::freeEngine() {
	if (m_engine) {
		AXL_MEM_DELETE(m_engine);
		m_engine = NULL;
	}
}

void
RegexStateImpl::initialize(
	uint_t execFlags,
	enc::CharCodec* codec
) {
	freeEngine();

	m_regex = NULL;
	m_decoder.setup(codec);
	m_execFlags = execFlags;
	m_prevCharFlags = RegexStateImpl::CharFlag_NewLine;
}

void
RegexStateImpl::postInitialize(Regex* regex) {
	ASSERT(!m_regex && !m_engine);

	m_regex = regex;

	switch (m_execFlags & RegexExecFlag_EngineMask) {
	case 0: // test default
	case RegexExecFlag_Dfa:
		m_engine = AXL_MEM_NEW_ARGS(RegexExecDfa, (this));
		break;

	case RegexExecFlag_NfaBt:
		m_engine = AXL_MEM_NEW_ARGS(RegexExecNfaBt, (this));
		break;

	case RegexExecFlag_NfaSp:
		m_engine = AXL_MEM_NEW_ARGS(RegexExecNfaSp, (this));
		break;

	case RegexExecFlag_NfaVm:
		m_engine = AXL_MEM_NEW_ARGS(RegexExecNfaVm, (this));
		break;

	default:
		ASSERT(false);
		m_engine = AXL_MEM_NEW_ARGS(RegexExecNfaVm, (this)); // fall back to threading NFA
	}

	reset();
}

void
RegexStateImpl::reset() {
	ASSERT(m_engine);

	m_matchAcceptId = -1;
	m_match.m_offset = m_offset;
	m_match.m_endOffset = -1;
	m_subMatchList.clear();
	m_subMatchArray.clear();
	m_engine->reset();
}

uint32_t
RegexStateImpl::calcAnchors(utf32_t c) {
	uint_t anchors = 0;
	uint_t charFlags = 0;

	if (m_prevCharFlags & CharFlag_NewLine)
		anchors |= Anchor_Begin;

	if (c == '\r') {
		anchors |= Anchor_End;
	} else if (c == '\n') {
		anchors |= Anchor_End;
		charFlags |= CharFlag_NewLine;
	}

	if (enc::utfIsLetterOrDigit(c)) {
		if (!(m_prevCharFlags & CharFlag_AlphaNum))
			anchors |= Anchor_Word;
		charFlags |= CharFlag_AlphaNum;
	} else if (m_prevCharFlags & CharFlag_AlphaNum) {
		anchors |= Anchor_Word;
	}

	m_prevCharFlags = charFlags;
	return anchors;
}

void
RegexStateImpl::accept(size_t acceptId) {
	AXL_RE_TRACE_CAPTURE("%p: accept(%d)\n", m_offset, acceptId);
	m_matchAcceptId = acceptId;
	m_match.m_endOffset = m_offset;
}

bool
RegexStateImpl::finalize(bool isEof) {
	if (m_matchAcceptId == -1)
		return false;

	if (!m_subMatchArray.isEmpty()) { // already finalized
		ASSERT(m_subMatchArray.getFront() == &m_match);
		return true;
	}

	if (m_execFlags & RegexExecFlag_AnchorDataEnd) {
		if (!isEof)
			return m_lastExecOffset + m_lastExecSize == m_match.m_endOffset + m_decoder.getAccumulatorCount(); // account for the incomplete codepoint

		if (m_lastExecOffset + m_lastExecSize != m_match.m_endOffset)
			return false;
	}

	if (!(m_execFlags & RegexExecFlag_Stream)) {
		m_match.m_charCodec = m_decoder.getCharCodec();
		m_match.m_p = (char*)m_lastExecBuffer + m_match.m_offset - m_lastExecOffset;
	}

	if (m_execFlags & RegexExecFlag_DisableCapture)
		m_subMatchArray.copy(&m_match);
	else if (m_regex->getRegexKind() != RegexKind_Switch)
		createSubMatches(
			0,
			m_regex->getCaptureCount(),
			m_engine->getCapturePosArray()
		);
	else
		createSubMatches(
			m_regex->getSwitchCaseBaseCaptureId(m_matchAcceptId),
			m_regex->getSwitchCaseCaptureCount(m_matchAcceptId),
			m_engine->getCapturePosArray()
		);

	return true;
}

void
RegexStateImpl::createSubMatches(
	size_t baseCaptureId,
	size_t captureCount,
	const sl::ArrayRef<RegexMatchPos>& capturePosArray
) {
	m_subMatchArray.setCountZeroConstruct(captureCount + 1);
	m_subMatchArray[0] = &m_match;

	size_t count = capturePosArray.getCount();
	if (count > captureCount)
		count = captureCount;

	for (size_t i = baseCaptureId; i < count; i++) {
		const RegexMatchPos& pos = capturePosArray[i];
		if (pos.m_endOffset == -1)
			continue;

		ASSERT(pos.m_offset != -1 && pos.m_offset <= pos.m_endOffset);

		RegexMatch* match = m_subMatchList.insertTail().p();
		match->m_offset = pos.m_offset;
		match->m_endOffset = pos.m_endOffset;

		if (!(m_execFlags & RegexExecFlag_Stream)) {
			match->m_charCodec = m_decoder.getCharCodec();
			match->m_p = (char*)m_lastExecBuffer + pos.m_offset - m_lastExecOffset;
		}

		m_subMatchArray[i - baseCaptureId + 1] = match;
	}
}

bool
RegexStateImpl::exec(
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
RegexState::initialize(
	uint_t execFlags,
	enc::CharCodec* codec
) {
	if (!m_p)
		m_p = AXL_RC_NEW(RegexStateImpl);
	else
		ASSERT(m_p.isExclusive());

	m_p->initialize(execFlags, codec);
}

void
RegexState::initialize(Regex* regex) {
	ASSERT(!m_p);
	m_p = AXL_RC_NEW(RegexStateImpl);
	m_p->initialize(0, enc::getCharCodec(enc::CharCodecKind_Utf8));
	m_p->postInitialize(regex);
}

void
RegexState::reset(size_t offset) {
	ASSERT(m_p);

	if (m_p.isExclusive()) {
		m_p->m_offset = offset;
		m_p->reset();
	} else {
		rc::Ptr<RegexStateImpl> p;
		sl::takeOver(&p, &m_p);

		m_p = AXL_RC_NEW(RegexStateImpl);
		m_p->initialize(p->m_execFlags, p->m_decoder.getCharCodec());
		m_p->m_offset = offset;
		m_p->postInitialize(p->m_regex);
	}
}

//..............................................................................

} // namespace re
} // namespace axl
