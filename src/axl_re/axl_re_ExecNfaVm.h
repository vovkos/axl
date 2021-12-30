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

class ExecNfaVmBase: public ExecNfaEngine {
protected:
	struct Thread: sl::ListLink {
		const NfaState* m_state;
		sl::Array<MatchPos> m_capturePosArray;

		bool
		isConsuming() const {
			return m_state->m_stateKind >= NfaStateKind_FirstConsuming;
		}

		void
		openCapture(
			size_t offset,
			size_t captureId
		);

		void
		closeCapture(
			size_t offset,
			size_t captureId
		);
	};

protected:
	sl::List<Thread> m_consumingThreadList;
	sl::List<Thread> m_nonConsumingThreadList;
	const NfaState* m_matchState;
	MatchPos m_matchPos;
	sl::Array<MatchPos> m_capturePosArray;

public:
	ExecNfaVmBase(StateImpl* parent);

	virtual
	void
	reset(
		uint_t prevCharFlags,
		size_t offset,
		const NfaState* state
	);

	virtual
	void
	eof() {
		advanceNonConsumingThreads(m_prevCharFlags | Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary);
		finalize(true);
	}

protected:
	void
	copy(const ExecNfaVmBase* src);

	void
	continueConsumingThread(Thread* thread);

	void
	advanceNonConsumingThreads(uint32_t anchors);

	void
	advanceConsumingThreads(utf32_t c);

	void
	finalize(bool isEof);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecNfaEngine*
createExecNfaVm(StateImpl* parent);

//..............................................................................

} // namespace re
} // namespace axl
