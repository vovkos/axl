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

#define _AXL_RE_FSMMATCHCONDITION_H

#include "axl_re_CharSet.h"

namespace axl {
namespace re {

//..............................................................................

enum FsmAnchor {
	FsmAnchor_Begin  = 0x01,
	FsmAnchor_End    = 0x02,
	FsmAnchor_Word   = 0x04,
	FsmAnchor__Count = 3,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum FsmMatchKind {
	FsmMatchKind_Undefined,
	FsmMatchKind_Anchor,
	FsmMatchKind_Char,
	FsmMatchKind_CharSet,
	FsmMatchKind_AnyChar,

	FsmMatchKind_FirstConsuming = FsmMatchKind_Char,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct FsmMatchCondition {
	FsmMatchKind m_matchKind;

	union {
		utf32_t m_char;
		FsmAnchor m_anchor;
		CharSet* m_charSet;
	};

	FsmMatchCondition();
	~FsmMatchCondition();

	bool
	isConsuming() const {
		return m_matchKind >= FsmMatchKind_FirstConsuming;
	}

	bool
	isMatchChar(utf32_t c) const;

	bool
	isMatchAnchor(uint_t anchors) const {
		ASSERT(m_matchKind == FsmMatchKind_Anchor);
		return (anchors & m_anchor) != 0;
	}

	void
	copy(const FsmMatchCondition& src);

	void
	addChar(utf32_t c);

	void
	addCharRange(
		utf32_t from,
		utf32_t to
	);
};

//..............................................................................

} // namespace re
} // namespace axl
