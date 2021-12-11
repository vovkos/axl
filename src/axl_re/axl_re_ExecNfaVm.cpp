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
	if (captureId >= m_capturePosArray.getCount())
		m_capturePosArray.setCount(captureId + 1);

	m_capturePosArray[captureId].m_offset = offset;
	m_capturePosArray[captureId].m_endOffset = offset;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecNfaVmBase::ExecNfaVmBase(StateImpl* parent):
	ExecEngine(parent) {
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
	ExecEngine::reset(offset);
	m_matchPos.m_offset = offset;
	m_consumingThreadList.clear();
	m_nonConsumingThreadList.clear();
	m_matchState = NULL;

	Thread* thread = AXL_MEM_NEW(Thread);
	thread->m_state = m_parent->m_regex->getNfaMatchStartState();

	if (thread->isConsuming())
		m_consumingThreadList.insertTail(thread);
	else
		m_nonConsumingThreadList.insertTail(thread);

	advanceNonConsumingThreads(Anchor_BeginLine | Anchor_BeginText | Anchor_WordBoundary);
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
					m_matchPos.m_endOffset = m_parent->m_offset;
					m_capturePosArray = thread->m_capturePosArray;
				}
				AXL_MEM_DELETE(thread);
				goto Break2;

			case NfaStateKind_OpenCapture:
				thread->openCapture(m_parent->m_offset, state->m_captureId);
				state = state->m_nextState;
				break;

			case NfaStateKind_CloseCapture:
				thread->closeCapture(m_parent->m_offset, state->m_captureId);
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
			ASSERT(false); // shouldn't be on consuming thread list
		}
	}
}

bool
ExecNfaVmBase::finalize(bool isEof) {
	if (m_parent->m_matchAcceptId != -1) // already finalized
		return true;

	if (!m_matchState)
		return false;

	if (m_parent->m_execFlags & RegexExecFlag_AnchorDataEnd) {
		if (!isEof)
			return true; // can't verify until we see EOF

		if (m_matchPos.m_endOffset != m_parent->m_lastExecOffset + m_parent->m_lastExecSize)
			return false;
	}

	m_parent->createMatch(m_matchState->m_acceptId, m_matchPos, m_capturePosArray);
	// TODO: create sub-matches
	return true;
}

//..............................................................................

template <typename Encoding>
class ExecNfaVm: public ExecNfaVmBase {
public:
	ExecNfaVm(StateImpl* parent):
		ExecNfaVmBase(parent) {}

	// ExecEngine

	virtual
	ExecEngine*
	clone(StateImpl* parent) {
		ExecNfaVm* exec = AXL_MEM_NEW_ARGS(ExecNfaVm, (parent));
		exec->copy(this);
		return exec;
	}

	virtual
	void
	exec(
		const void* p,
		size_t size
	) {
		Encoding::Decoder::decode(&m_decoderState, *this, (char*)p, (char*)p + size);
	}

	// DecodeEmitter

	bool
	canEmit() const {
		return m_execResult == ExecResult_Undefined;
	}

	void
	emitReplacement(const char* p) {
		emitCodePoint(p, enc::StdChar_Replacement);
	}

	void
	emitCodePoint(
		const char* p,
		utf32_t c
	) {
		uint_t anchors = m_parent->calcAnchorsUpdateCharFlags(c);
		advanceNonConsumingThreads(anchors);

		if (m_consumingThreadList.isEmpty()) {
			m_execResult = (ExecResult)finalize(false);
			return;
		}

		m_parent->m_offset = m_parent->m_lastExecOffset + p - (char*)m_parent->m_lastExecBuffer;
		advanceConsumingThreads(c);
	}
};

//..............................................................................

ExecEngine*
createExecNfaVm(
	StateImpl* parent,
	enc::CharCodecKind codecKind
) {
	switch (codecKind) {
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
