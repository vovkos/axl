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

#include "axl_re_RegexState.h"

namespace axl {
namespace re {

//..............................................................................

class RegexExecNfaSp: public RegexExecEngine {
protected:
	sl::Array<RegexMatchPos> m_capturePosArray;
	NfaStateSet m_consumingStateSetTable[2];
	NfaStateSet m_nonConsumingStateSetTable[2];
	size_t m_consumingStateSetIdx;
	size_t m_nonConsumingStateSetIdx;
	bool m_isEmpty;

public:
	RegexExecNfaSp(RegexStateImpl* parent);

	virtual
	sl::ArrayRef<RegexMatchPos>
	getCapturePosArray() {
		return m_capturePosArray;
	}

	virtual
	void
	reset();

	virtual
	bool
	exec(
		const void* p,
		size_t size
	);

	virtual
	bool
	eof();

protected:
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
};

//..............................................................................

} // namespace re
} // namespace axl
