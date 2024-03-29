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

#define _AXL_RE_REGEXMATCH_H

#include "axl_re_Pch.h"

namespace axl {
namespace re {

struct StateImpl;

//..............................................................................

struct MatchPos {
	uint64_t m_offset;
	uint64_t m_endOffset;

	MatchPos() {
		m_offset = -1;
		m_endOffset = -1;
	}

	MatchPos(
		uint64_t offset,
		uint64_t endOffset
	) {
		m_offset = offset;
		m_endOffset = endOffset;
	}

	bool
	operator == (const MatchPos& pos) {
		return isEqual(pos);
	}

	bool
	operator != (const MatchPos& pos) {
		return !isEqual(pos);
	}

	bool
	isEqual(const MatchPos& pos) {
		return m_offset == pos.m_offset && m_endOffset == pos.m_endOffset;
	}

	bool
	isInside(
		uint64_t offset,
		uint64_t endOffset
	) const {
		return m_offset >= offset && m_endOffset <= endOffset;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Match: protected MatchPos {
	friend struct StateImpl;

protected:
	enc::CharCodec* m_codec;
	const void* m_p;
	mutable sl::StringRef m_text; // cache

public:
	Match():
		MatchPos(0, 0) {
		m_codec = NULL;
		m_p = NULL;
	}

	bool
	operator == (const Match& match) {
		return isEqual(match);
	}

	bool
	operator != (const Match& match) {
		return !isEqual(match);
	}

	bool
	isEmpty() const {
		return m_offset == m_endOffset;
	}

	uint64_t
	getOffset() const {
		return m_offset;
	}

	uint64_t
	getEndOffset() const {
		return m_endOffset;
	}

	size_t
	getSize() const {
		return (size_t)(m_endOffset - m_offset);
	}

	uint64_t
	getSize64() const {
		return m_endOffset - m_offset;
	}

	const void*
	p() const {
		return m_p;
	}

	bool
	isEqual(const Match& match) {
		return MatchPos::isEqual(match) && m_codec == match.m_codec && m_p == match.m_p;
	}

	bool
	hasText() const {
		ASSERT((m_p != NULL) == (m_codec != NULL));
		return m_p != NULL;
	}

	sl::StringRef
	getText() const {
		ASSERT(m_p && m_codec);
 		return !m_text.isEmpty() ? m_text : cacheText();
	}

protected:
	sl::StringRef
	cacheText() const {
		return m_text = m_codec == enc::getCharCodec(enc::CharCodecKind_Utf8) ?
			sl::StringRef((char*)m_p, getSize()) :
			m_codec->decode_utf8(m_p, getSize());
	}
};

//..............................................................................

} // namespace re
} // namespace axl
