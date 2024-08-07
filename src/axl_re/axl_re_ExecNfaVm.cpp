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
	uint64_t offset,
	size_t captureId
) {
	AXL_RE_TRACE_CAPTURE("%p: open capture(%d)\n", offset, captureId);

	if (captureId >= m_capturePosArray.getCount())
		m_capturePosArray.setCount(captureId + 1);

	MatchPos& pos = m_capturePosArray.rwi()[captureId];
	pos.m_offset = offset;
	pos.m_endOffset = offset;
}

void
ExecNfaVmBase::Thread::closeCapture(
	uint64_t offset,
	size_t captureId
) {
	ASSERT(captureId < m_capturePosArray.getCount());
	AXL_RE_TRACE_CAPTURE("%p: close capture(%d)\n", offset, captureId);
	m_capturePosArray.rwi()[captureId].m_endOffset = offset;
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
		Thread* thread = new Thread(**it);
		m_consumingThreadList.insertTail(thread);
	}

	it = src->m_nonConsumingThreadList.getHead();
	for (; it; it++) {
		Thread* thread = new Thread(**it);
		m_nonConsumingThreadList.insertTail(thread);
	}
}

void
ExecNfaVmBase::initialize(
	const StateInit& init,
	const NfaState* state
) {
	ExecEngine::initialize(init);
	m_matchPos.m_offset = init.m_offset;
	m_consumingThreadList.clear();
	m_nonConsumingThreadList.clear();
	m_matchState = NULL;

	Thread* thread = new Thread;
	thread->m_state = state;

	if (thread->isConsuming())
		m_consumingThreadList.insertTail(thread);
	else
		m_nonConsumingThreadList.insertTail(thread);
}

void
ExecNfaVmBase::eof(bool isLastExecDataAvailable) {
	if (!isLastExecDataAvailable)
		resetLastExecData();

	advanceNonConsumingThreads(m_prevCharFlags | Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary);
	finalize(true);
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
				newThread = new Thread(*thread);
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

				delete thread;
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

				delete thread;
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
				delete thread;
			break;

		case NfaStateKind_MatchCharSet:
			if (thread->m_state->m_charSet->isSet(c))
				continueConsumingThread(thread);
			else
				delete thread;
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
	emitCp(
		const char* p,
		utf32_t c
	) {
		uint_t anchors = this->calcAnchorsUpdateCharFlags(c);
		this->advanceNonConsumingThreads(anchors);

		if (this->m_consumingThreadList.isEmpty()) {
			this->finalize(false);
			return;
		}

		this->m_offset = this->m_lastExecOffset + p - (char*)this->m_lastExecData;
		this->advanceConsumingThreads(c);
	}
};

//..............................................................................

template <typename Decoder>
class ExecNfaVmFactory: public ExecEngineFactory {
public:
	virtual
	ExecEngine*
	createExecEngine(StateImpl* parent) {
		return new ExecNfaVm<Decoder>(parent);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecNfaEngine*
createExecNfaVm(StateImpl* parent) {
	static ExecNfaVmFactory<enc::Latin1Decoder>    latin1Factory;
	static ExecNfaVmFactory<enc::Utf8Decoder>      utf8Factory;
	static ExecNfaVmFactory<enc::Utf16sDecoder>    utf16Factory;
	static ExecNfaVmFactory<enc::Utf16sDecoder_be> utf16Factory_be;
	static ExecNfaVmFactory<enc::Utf32sDecoder>    utf32Factory;
	static ExecNfaVmFactory<enc::Utf32sDecoder_be> utf32Factory_be;

	static ExecEngineFactory* factoryTable[enc::CharCodecKind__Count] = {
		&latin1Factory,
		&utf8Factory,
		&utf16Factory,
		&utf16Factory_be,
		&utf32Factory,
		&utf32Factory_be,
	};

	size_t i = parent->m_init.m_codecKind;
	if (i >= countof(factoryTable)) {
		ASSERT(false);
		i = enc::CharCodecKind_Latin1;
	}

	return (ExecNfaEngine*)factoryTable[i]->createExecEngine(parent);
}

//..............................................................................

} // namespace re
} // namespace axl
