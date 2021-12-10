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

#include "axl_re_State.h"

namespace axl {
namespace re {

//..............................................................................

class ExecNfaVmBase: public ExecEngine {
protected:
	struct Thread: sl::ListLink {
		const NfaState* m_state;
		size_t m_matchEndOffset;
		sl::Array<MatchPos> m_capturePosArray;

		void
		openCapture(
			size_t offset,
			size_t captureId
		);

		void
		closeCapture(
			size_t offset,
			size_t captureId
		) {
			ASSERT(captureId < m_capturePosArray.getCount());
			m_capturePosArray[captureId].m_endOffset = offset;
		}

	};

protected:
	sl::List<Thread> m_consumingThreadList;
	sl::List<Thread> m_nonConsumingThreadList;
	Thread* m_acceptingThread;
	size_t m_matchOffset;

public:
	ExecNfaVmBase(StateImpl* parent);

	virtual
	~ExecNfaVmBase() {
		freeAcceptingThread();
	}

	virtual
	void
	reset(size_t offset);

	virtual
	bool
	eof() {
		advanceNonConsumingThreads(Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary);
		return finalize(true);
	}

protected:
	void
	copy(const ExecNfaVmBase* src);

	void
	freeAcceptingThread() {
		if (m_acceptingThread) {
			AXL_MEM_DELETE(m_acceptingThread);
			m_acceptingThread = NULL;
		}
	}

	void
	continueConsumingThread(Thread* thread);

	void
	advanceNonConsumingThreads(uint32_t anchors);

	void
	advanceConsumingThreads(utf32_t c);

	bool
	finalize(bool isEof);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecEngine*
createExecNfaVm(
	StateImpl* parent,
	enc::CharCodecKind codecKind
);

//..............................................................................

} // namespace re
} // namespace axl
