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
#include "axl_re_RegexExec.h"

namespace axl {
namespace re {

class Regex;
class RegexState;
class RegexEngine;

//..............................................................................

struct RegexStateImpl: public rc::RefCount {
	friend class RegexState;

	enum CharFlag {
		CharFlag_Cr   = 0x01,
		CharFlag_Lf   = 0x02,
		CharFlag_Nl   = CharFlag_Lf,
		CharFlag_Word = 0x04,
	};

	Regex* m_regex;
	RegexExecEngine* m_engine;
	enc::CodePointDecoder m_decoder;
	const void* m_lastExecBuffer;
	size_t m_lastExecOffset;
	size_t m_lastExecSize;
	uint_t m_execFlags;
	uint_t m_prevCharFlags;
	size_t m_offset;
	RegexMatch m_match;
	size_t m_matchAcceptId;
	sl::BoxList<RegexMatch> m_subMatchList;
	sl::Array<RegexMatch*> m_subMatchArray;

protected:
	RegexStateImpl(); // only creatable by RegexState

public:
	~RegexStateImpl() {
		freeEngine();
	}

	void
	freeEngine();

	void
	initialize(
		uint_t execFlags,
		enc::CharCodec* codec
	);

	void
	postInitialize(Regex* regex);

	void
	reset();

	void
	resetMatchOffset() {
		m_match.m_offset = m_offset;
	}

	static
	uint_t
	calcCharFlags(utf32_t c);

	uint_t
	calcAnchors(uint_t charFlags);

	uint_t
	calcAnchorsUpdateCharFlags(utf32_t c);

	void
	createMatch(
		size_t acceptId,
		const RegexMatchPos& matchPos,
		const sl::ArrayRef<RegexMatchPos>& capturePosArray = sl::ArrayRef<RegexMatchPos>()
	);

	bool
	exec(
		const void* p,
		size_t size
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
uint_t
RegexStateImpl::calcCharFlags(utf32_t c) {
	uint_t charFlags = 0;

	if (c == '\r')
		charFlags |= CharFlag_Cr;
	else if (c == '\n')
		charFlags |= CharFlag_Lf;
	else if (c == '_' || enc::utfIsLetterOrDigit(c))
		charFlags |= CharFlag_Word;

	return charFlags;
}

inline
uint_t
RegexStateImpl::calcAnchors(uint_t charFlags) {
	uint_t anchors = 0;

	if (m_prevCharFlags & CharFlag_Lf)
		anchors |= Anchor_BeginLine;

	if (charFlags & (CharFlag_Cr | CharFlag_Lf))
		anchors |= Anchor_EndLine;

	if ((charFlags & m_prevCharFlags) & CharFlag_Word)
		anchors |= Anchor_WordBoundary;
	else
		anchors |= Anchor_NotWordBoundary;

	return anchors;
}

inline
uint_t
RegexStateImpl::calcAnchorsUpdateCharFlags(utf32_t c) {
	uint_t charFlags = calcCharFlags(c);
	uint_t anchors = calcAnchors(charFlags);
	m_prevCharFlags = charFlags;
}

//..............................................................................

class RegexState {
	friend class Regex;
	friend class RegexMatch;

protected:
	rc::Ptr<RegexStateImpl> m_p;

public:
	RegexState() {}

#if (_AXL_CPP_HAS_RVALUE_REF)
	RegexState(RegexState&& src) {
		m_p.move(std::move(src.m_p));
	}
#endif

	RegexState(const RegexState& src) {
		m_p.copy(src.m_p);
	}

	RegexState(
		uint32_t flags,
		enc::CharCodec* codec
	) {
		initialize(flags, codec);
	}

	RegexState(
		uint32_t flags,
		enc::CharCodecKind codecKind = enc::CharCodecKind_Utf8
	) {
		initialize(flags, codecKind);
	}

	operator bool () const {
		return m_p;
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	RegexState&
	operator = (RegexState&& src) {
		m_p.move(std::move(src.m_p));
		return *this;
	}
#endif

	RegexState&
	operator = (const RegexState& src) {
		m_p.copy(src.m_p);
		return *this;
	}

	const Regex*
	getRegex() const {
		ASSERT(m_p);
		return m_p->m_regex;
	}

	enc::CharCodec*
	getCodec() const {
		ASSERT(m_p);
		return m_p->m_decoder.getCharCodec();
	}

	uint_t
	getExecFlags() const {
		ASSERT(m_p);
		return m_p->m_execFlags;
	}

	bool
	isMatch() const {
		ASSERT(m_p);
		return m_p->m_matchAcceptId != -1;
	}

	bool
	getLeftmostPotentialMatchOffset() const;

	size_t
	getMatchSwitchCaseId() const {
		ASSERT(m_p);
		return m_p->m_matchAcceptId;
	}

	const RegexMatch*
	getMatch() const {
		ASSERT(m_p);
		return isMatch() ? &m_p->m_match : NULL;
	}

	size_t
	getSubMatchCount() const {
		ASSERT(m_p);
		return m_p->m_subMatchArray.getCount();
	}

	const RegexMatch*
	getSubMatch(size_t i) const {
		ASSERT(m_p);
		return i < m_p->m_subMatchArray.getCount() ? m_p->m_subMatchArray[i] : NULL;
	}

	void
	initialize(
		uint_t execFlags,
		enc::CharCodec* codec
	);

	void
	initialize(
		uint_t execFlags = 0,
		enc::CharCodecKind codecKind = enc::CharCodecKind_Utf8
	) {
		initialize(execFlags, enc::getCharCodec(codecKind));
	}

	void
	reset(size_t offset = 0);

protected:
	void
	initialize(Regex* regex);

	void
	postInitialize(Regex* regex) {
		ASSERT(m_p && m_p.isExclusive());
		return m_p->postInitialize(regex);
	}

	bool
	exec(
		const void* p,
		size_t size
	) {
		ASSERT(m_p && m_p.isExclusive());
		return m_p->exec(p, size);
	}

	bool
	eof() {
		ASSERT(m_p && m_p.isExclusive());
		return m_p->m_engine->eof();
	}
};

//..............................................................................

} // namespace re
} // namespace axl
