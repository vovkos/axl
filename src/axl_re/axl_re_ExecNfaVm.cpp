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
#include "axl_re_ExecNfaVm.h"
#include "axl_re_Regex.h"
#include "axl_enc_Utf16s.h"
#include "axl_enc_Utf32s.h"

namespace axl {
namespace re {

//..............................................................................

void
ExecNfaVmBase::Thread::openCapture(
	size_t offset,
	size_t captureId
) {
	AXL_RE_TRACE_CAPTURE("%p: open capture(%d)\n", offset, captureId);

	if (captureId >= m_capturePosArray.getCount())
		m_capturePosArray.setCount(captureId + 1);

	m_capturePosArray[captureId].m_offset = offset;
	m_capturePosArray[captureId].m_endOffset = offset;
}

void
ExecNfaVmBase::Thread::closeCapture(
	size_t offset,
	size_t captureId
) {
	ASSERT(captureId < m_capturePosArray.getCount());
	AXL_RE_TRACE_CAPTURE("%p: close capture(%d)\n", offset, captureId);
	m_capturePosArray[captureId].m_endOffset = offset;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecNfaVmBase::ExecNfaVmBase(StateImpl* parent):
	ExecNfaEngine(parent) {
	m_matchState = NULL;
}

void
ExecNfaVmBase::copy(const ExecNfaVmBase* src) {
	ExecEngine::copy(src);

	m_consumingThreadList.clear();
	m_nonConsumingThreadList.clear();

	sl::ConstIterator<Thread> it = src->m_consumingThreadList.getHead();
	for (; it; it++) {
		Thread* thread = AXL_MEM_NEW_ARGS(Thread, (**it));
		m_consumingThreadList.insertTail(thread);
	}

	it = src->m_nonConsumingThreadList.getHead();
	for (; it; it++) {
		Thread* thread = AXL_MEM_NEW_ARGS(Thread, (**it));
		m_nonConsumingThreadList.insertTail(thread);
	}
}

void
ExecNfaVmBase::reset(size_t offset) {
	reset(offset, m_parent->m_regex->getNfaStartState());
}

void
ExecNfaVmBase::reset(
	size_t offset,
	const NfaState* state
) {
	ExecEngine::reset(offset);
	m_matchPos.m_offset = offset;
	m_consumingThreadList.clear();
	m_nonConsumingThreadList.clear();
	m_matchState = NULL;

	Thread* thread = AXL_MEM_NEW(Thread);
	thread->m_state = state;

	if (thread->isConsuming())
		m_consumingThreadList.insertTail(thread);
	else
		m_nonConsumingThreadList.insertTail(thread);
}

void
ExecNfaVmBase::advanceNonConsumingThreads(uint32_t anchors) {
	while (!m_nonConsumingThreadList.isEmpty()) {
		Thread* thread = m_nonConsumingThreadList.removeHead();
		const NfaState* state = thread->m_state;
		for (;;)
			switch (state->m_stateKind) {
				Thread* newThread;

			case NfaStateKind_Link:
				state = state->m_nextState->m_stateKind == NfaStateKind_Link ?
					state->m_nextState->m_opState :
					state->m_nextState;
				break;

			case NfaStateKind_Sequence:
				state = state->m_nextState->m_opState;
				break;

			case NfaStateKind_Split:
				newThread = AXL_MEM_NEW_ARGS(Thread, (*thread));
				newThread->m_state = state->m_splitState;
				if (newThread->isConsuming())
					m_consumingThreadList.insertTail(newThread);
				else
					m_nonConsumingThreadList.insertTail(newThread);

				state = state->m_nextState;
				break;

			case NfaStateKind_Accept:
				if (!m_matchState || state->m_id <= m_matchState->m_id) {
					m_matchState = state;
					m_matchPos.m_endOffset = m_offset;
					m_capturePosArray = thread->m_capturePosArray;
				}
				AXL_MEM_DELETE(thread);
				goto Break2;

			case NfaStateKind_OpenCapture:
				if (!(m_execFlags & ExecFlag_DisableCapture))
					thread->openCapture(m_offset, state->m_captureId);
				state = state->m_nextState;
				break;

			case NfaStateKind_CloseCapture:
				if (!(m_execFlags & ExecFlag_DisableCapture))
					thread->closeCapture(m_offset, state->m_captureId);
				state = state->m_nextState;
				break;

			case NfaStateKind_MatchAnchor:
				if (state->m_anchor & anchors) {
					state = state->m_nextState;
					break;
				}

				AXL_MEM_DELETE(thread);
				goto Break2;

			case NfaStateKind_MatchChar:
			case NfaStateKind_MatchCharSet:
			case NfaStateKind_MatchAnyChar:
				thread->m_state = state;
				m_consumingThreadList.insertTail(thread);
				goto Break2;

			default:
				ASSERT(false);
			}
	Break2:;
	}
}

inline
void
ExecNfaVmBase::continueConsumingThread(Thread* thread) {
	thread->m_state = thread->m_state->m_nextState;
	ASSERT(!thread->isConsuming()); // must be a link
	m_nonConsumingThreadList.insertTail(thread);
}

void
ExecNfaVmBase::advanceConsumingThreads(utf32_t c) {
	while (!m_consumingThreadList.isEmpty()) {
		Thread* thread = m_consumingThreadList.removeHead();
 		switch (thread->m_state->m_stateKind) {
		case NfaStateKind_MatchChar:
			if (thread->m_state->m_char == c)
				continueConsumingThread(thread);
			else
				AXL_MEM_DELETE(thread);
			break;

		case NfaStateKind_MatchCharSet:
			if (thread->m_state->m_charSet->isSet(c))
				continueConsumingThread(thread);
			else
				AXL_MEM_DELETE(thread);
			break;

		case NfaStateKind_MatchAnyChar:
			continueConsumingThread(thread);
			break;

		default:
			ASSERT(false); // shouldn't be on the consuming thread list
		}
	}
}

void
ExecNfaVmBase::finalize(bool isEof) {
	ASSERT(m_parent->m_match.getOffset() == -1);

	if (!m_matchState) {
		m_execResult = ExecResult_NoMatch;
		return;
	}

	if (m_execFlags & ExecFlag_AnchorDataEnd) {
		if (!isEof)
			return; // can't verify until we see EOF

		if (m_matchPos.m_endOffset != m_lastExecEndOffset) {
			m_execResult = ExecResult_NoMatch;
			return;
		}
	}

	m_parent->createMatch(m_matchState->m_acceptId, m_lastExecOffset, m_lastExecData, m_matchPos, m_capturePosArray);
	m_execResult = ExecResult_Match;
}

//..............................................................................

template <typename Encoding>
class ExecNfaVm: public ExecImpl<
	ExecNfaVm<Encoding>,
	ExecNfaVmBase,
	Encoding
> {
public:
	ExecNfaVm(StateImpl* parent):
		ExecImpl<ExecNfaVm, ExecNfaVmBase, Encoding>(parent) {}

	void
	emitCodePoint(
		const char* p,
		utf32_t c
	) {
		uint_t anchors = calcAnchorsUpdateCharFlags(c);
		advanceNonConsumingThreads(anchors);

		if (m_consumingThreadList.isEmpty()) {
			finalize(false);
			return;
		}

		m_offset = m_lastExecOffset + p - (char*)m_lastExecData;
		advanceConsumingThreads(c);
	}
};

//..............................................................................

ExecNfaEngine*
createExecNfaVm(StateImpl* parent) {
	switch (parent->m_init.m_codecKind) {
	case enc::CharCodecKind_Ascii:
		return AXL_MEM_NEW_ARGS(ExecNfaVm<enc::Ascii>, (parent));
	case enc::CharCodecKind_Utf8:
		return AXL_MEM_NEW_ARGS(ExecNfaVm<enc::Utf8>, (parent));
	case enc::CharCodecKind_Utf16:
		return AXL_MEM_NEW_ARGS(ExecNfaVm<enc::Utf16s>, (parent));
	case enc::CharCodecKind_Utf16_be:
		return AXL_MEM_NEW_ARGS(ExecNfaVm<enc::Utf16s_be>, (parent));
	case enc::CharCodecKind_Utf32:
		return AXL_MEM_NEW_ARGS(ExecNfaVm<enc::Utf32s>, (parent));
	case enc::CharCodecKind_Utf32_be:
		return AXL_MEM_NEW_ARGS(ExecNfaVm<enc::Utf32s_be>, (parent));
	default:
		ASSERT(false);
		return AXL_MEM_NEW_ARGS(ExecNfaVm<enc::Ascii>, (parent));
	}

	return NULL;
}

//..............................................................................

} // namespace re
} // namespace axl
