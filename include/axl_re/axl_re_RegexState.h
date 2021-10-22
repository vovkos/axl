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

#define _AXL_RE_REGEXSTATE_H

#include "axl_re_Nfa.h"
#include "axl_re_RegexNameMgr.h"

namespace axl {
namespace re {

class Regex;
class RegexState;

//..............................................................................

class RegexMatch {
	friend class RegexState;

protected:
	RegexState* m_state;
	size_t m_offset;
	size_t m_length;
	const char* m_p;
	mutable sl::StringRef m_text; // cache

public:
	RegexState*
	getState() const {
		return m_state;
	}

	size_t
	getOffset() const {
		return m_offset;
	}

	size_t
	getLength() const {
		return m_length;
	}

	const char*
	p() const {
		return m_p;
	}

	sl::StringRef
	getText() const;
};

//..............................................................................

class RegexState {
	enum {
		DecodeBufferSize = 64
	};

protected:
	Regex* m_regex;
	enc::CharCodec* m_codec;
	sl::BitMap m_consumingStateSetTable[2];
	sl::BitMap m_nonConsumingStateSet;
	size_t m_consumingStateSetIdx;
	bool m_isPrevCharAlphanumeric;
	size_t m_offset;

	NfaState* m_lastAcceptState;
	size_t m_lastAcceptedOffset;

public:
	RegexState();

	RegexState(
		Regex* regex,
		enc::CharCodec* codec
	) {
		initialize(regex, codec);
	}

	RegexState(
		Regex* regex,
		enc::CharCodecKind codecKind = enc::CharCodecKind_Utf8
	) {
		initialize(regex, codecKind);
	}

	bool
	isEmpty() {
		return
			m_consumingStateSetTable[m_consumingStateSetIdx].findBit(0) ||
			m_nonConsumingStateSet.findBit(0);
	}

	void
	reset();

	void
	initialize(
		Regex* regex,
		enc::CharCodec* codec
	);

	void
	initialize(
		Regex* regex,
		enc::CharCodecKind codecKind = enc::CharCodecKind_Utf8
	) {
		initialize(regex, enc::getCharCodec(codecKind));
	}

	NfaState*
	getLastAcceptedState() {
		return m_lastAcceptState;
	}

	bool
	exec(
		const void* p,
		size_t size
	);

	bool
	eof();

protected:
	void
	addState(NfaState* state);

	void
	advanceNonConsumingStates(uint32_t anchors);
};

//..............................................................................

} // namespace re
} // namespace axl
