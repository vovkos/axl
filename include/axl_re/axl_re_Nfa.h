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

#define _AXL_RE_NFA_H

#include "axl_re_CharSet.h"

namespace axl {
namespace re {

//..............................................................................

enum Anchor {
	Anchor_Begin  = 0x01,
	Anchor_End    = 0x02,
	Anchor_Word   = 0x04,
	Anchor__Count = 3,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum NfaStateKind {
	NfaStateKind_Undefined = 0,
	NfaStateKind_Accept,
	NfaStateKind_Epsilon,
	NfaStateKind_Split,
	NfaStateKind_OpenCapture,
	NfaStateKind_CloseCapture,
	NfaStateKind_MatchAnchor,
	NfaStateKind_MatchChar,
	NfaStateKind_MatchCharSet,
	NfaStateKind_MatchAnyChar,

	NfaStateKind_LastNonConsuming = NfaStateKind_MatchAnchor,
	NfaStateKind_FirstConsuming   = NfaStateKind_MatchChar,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct NfaState: sl::ListLink {
	NfaStateKind m_stateKind;
	uint_t m_id;

	union {
		utf32_t m_char;
		Anchor m_anchor;
		size_t m_captureId;
		CharSet* m_charSet;
		void* m_acceptContext;
	};

	NfaState* m_nextState;
	NfaState* m_splitState;

	NfaState();
	~NfaState();

	bool
	isConsuming() const {
		return m_stateKind >= NfaStateKind_FirstConsuming;
	}

	bool
	isMatchChar(utf32_t c) const;

	bool
	isMatchAnchor(uint_t anchors) const {
		ASSERT(m_stateKind == NfaStateKind_MatchAnchor);
		return (anchors & m_anchor) != 0;
	}

	void
	createAccept(void* context);

	void
	createEpsilon(NfaState* nextState);

	void
	createSplit(
		NfaState* nextState,
		NfaState* splitState
	);

	void
	createMatchChar(
		uint_t c,
		NfaState* nextState
	);

	void
	createMatchCharSet(NfaState* nextState);

	void
	createMatchAnyChar(NfaState* nextState);

	void
	createMatchAnchor(
		Anchor anchor,
		NfaState* nextState
	);

	void
	createOpenCapture(
		size_t captureId,
		NfaState* nextState
	);

	void
	createCloseCapture(
		size_t captureId,
		NfaState* nextState
	);

	void
	resolveOutStates();

	NfaState*
	resolveEpsilon();

	void
	addChar(utf32_t c);

	void
	addCharRange(
		utf32_t from,
		utf32_t to
	);

	void
	copy(NfaState& src);
};

//..............................................................................

} // namespace re
} // namespace axl
