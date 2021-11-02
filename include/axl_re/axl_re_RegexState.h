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
#include "axl_re_Dfa.h"
#include "axl_re_RegexNameMgr.h"

namespace axl {
namespace re {

class Regex;
class RegexState;

//..............................................................................

class RegexMatch: public sl::ListLink {
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

enum RegexStateFlag {
	RegexStateFlag_Lexer       = 0x02,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class RegexState {
	friend class Regex;

protected:
	enum {
		DecodeBufferSize = 64
	};

	enum PrevCharFlag {
		PrevCharFlag_AlphaNum = 0x01,
		PrevCharFlag_NewLine  = 0x02,
	};

protected:
	Regex* m_regex;
	enc::CharCodec* m_codec;
	uint_t m_flags;
	size_t m_matchLengthLimit;

	sl::BitMap m_consumingStateSetTable[2];
	sl::BitMap m_nonConsumingStateSet;
	size_t m_consumingStateSetIdx;
	uint_t m_prevCharFlags;
	const NfaState* m_lastAcceptState;

	size_t m_offset;
	size_t m_matchOffset;
	size_t m_lastAcceptMatchLength;
	size_t m_consumedLength;

	sl::List<RegexMatch> m_matchList;
	RegexMatch* m_match;
	sl::Array<RegexMatch*> m_subMatchArray;

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
	isEmpty() const {
		return
			!m_consumingStateSetTable[m_consumingStateSetIdx].findBit() &&
			!m_nonConsumingStateSet.findBit();
	}

	const Regex*
	getRegex() const {
		return m_regex;
	}

	enc::CharCodec*
	getCodec() const {
		return m_codec;
	}

	const RegexMatch*
	getMatch() const {
		return m_match;
	}

	const RegexMatch*
	getSubMatch(size_t i) const {
		return i < m_subMatchArray.getCount() ? m_subMatchArray[i] : NULL;
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

	const NfaState*
	getLastAcceptedState() {
		return m_lastAcceptState;
	}

protected: // should be called by Regex::match
	bool
	exec(
		const void* p,
		size_t size
	);

	bool
	eof();

protected:
	void
	addState(const NfaState* state);

	void
	advanceNonConsumingStates(uint32_t anchors);
};

//..............................................................................

inline
sl::StringRef
RegexMatch::getText() const {
	if (m_text.isEmpty())
		m_text = m_state->getCodec()->decode_utf8(m_p, m_length);
	return m_text;
}

//..............................................................................

} // namespace re
} // namespace axl
