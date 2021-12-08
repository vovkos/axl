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

class ExecNfaSpBase: public ExecEngine {
protected:
	sl::Array<MatchPos> m_capturePosArray;
	NfaStateSet m_consumingStateSetTable[2];
	NfaStateSet m_nonConsumingStateSetTable[2];
	size_t m_consumingStateSetIdx;
	size_t m_nonConsumingStateSetIdx;
	size_t m_matchAcceptId;
	size_t m_matchEndOffset;
	bool m_isEmpty;

public:
	ExecNfaSpBase(StateImpl* parent);

	virtual
	void
	reset(size_t offset);

	virtual
	bool
	eof();

protected:
	void
	copy(const ExecNfaSpBase* src);

	void
	addState(const NfaState* state);

	void
	openCapture(size_t captureId);

	void
	closeCapture(size_t captureId);

	void
	advanceNonConsumingStates(uint32_t anchors);

	void
	advanceConsumingStates(utf32_t c);

	bool
	finalize(bool isEof);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecEngine*
createExecNfaSp(
	StateImpl* parent,
	enc::CharCodecKind codecKind
);

//..............................................................................

} // namespace re
} // namespace axl
