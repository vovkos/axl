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
			uint64_t offset,
			size_t captureId
		);

		void
		closeCapture(
			uint64_t offset,
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
	initialize(
		const StateInit& init,
		const NfaState* state
	);

	virtual
	void
	eof(bool isLastExecDataAvailable);

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
