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

#define _AXL_RE_REGEXSTATEINIT_H

#include "axl_re_Nfa.h"

namespace axl {
namespace re {

//..............................................................................

struct StateInit {
	uint_t m_execFlags;
	enc::CharCodecKind m_codecKind;
	enc::DecoderState m_decoderState;
	uint_t m_prevCharFlags;
	size_t m_offset;
	uint_t m_baseCharFlags;
	size_t m_baseOffset;
	size_t m_dfaStateId;
	size_t m_matchAcceptId;

	StateInit() {
		setup(0, 0, enc::CharCodecKind_Utf8);
	}

	StateInit(
		uint_t execFlags,
		size_t offset = 0,
		enc::CharCodecKind codecKind = enc::CharCodecKind_Utf8
	) {
		setup(execFlags, offset, codecKind);
	}

	void
	setup(
		uint_t execFlags,
		size_t offset = 0,
		enc::CharCodecKind codecKind = enc::CharCodecKind_Utf8
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
StateInit::setup(
	uint_t execFlags,
	size_t offset,
	enc::CharCodecKind codecKind
) {
	m_execFlags = execFlags;
	m_codecKind = codecKind;
	m_decoderState = 0;
	m_prevCharFlags = Anchor_BeginLine | Anchor_BeginText | Anchor_WordBoundary;
	m_offset = offset;
	m_baseCharFlags = Anchor_BeginLine | Anchor_BeginText | Anchor_WordBoundary;
	m_baseOffset = offset;
	m_dfaStateId = -1;
	m_matchAcceptId = -1;
}

//..............................................................................

} // namespace re
} // namespace axl
