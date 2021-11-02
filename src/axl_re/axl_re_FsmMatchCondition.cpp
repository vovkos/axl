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

#include "pch.h"
#include "axl_re_FsmMatchCondition.h"

namespace axl {
namespace re {

//..............................................................................

FsmMatchCondition::FsmMatchCondition() {
	m_matchKind = FsmMatchKind_Undefined;
	m_charSet = NULL;
}

FsmMatchCondition::~FsmMatchCondition() {
	if (m_matchKind == FsmMatchKind_CharSet) {
		ASSERT(m_charSet);
		AXL_MEM_DELETE(m_charSet);
	}
}

bool
FsmMatchCondition::isMatchChar(utf32_t c) const {
	switch (m_matchKind) {
	case FsmMatchKind_Char:
		return c == m_char;

	case FsmMatchKind_CharSet:
		ASSERT(m_charSet);
		return m_charSet->isSet(c);

	case FsmMatchKind_AnyChar:
		return true;

	default:
		ASSERT(false);
		return false;
	}
}

void
FsmMatchCondition::copy(const FsmMatchCondition& src) {
	m_matchKind = src.m_matchKind;
	m_char = src.m_char;

	if (m_matchKind == FsmMatchKind_CharSet) {
		ASSERT(!m_charSet && src.m_charSet);
		m_charSet = AXL_MEM_NEW(CharSet);
		m_charSet->copy(*src.m_charSet);
	}
}

void
FsmMatchCondition::addChar(utf32_t c) {
	switch (m_matchKind) {
	case FsmMatchKind_Char:
		if (m_char == c)
			break; // no change

		m_matchKind = FsmMatchKind_CharSet;
		ASSERT(!m_charSet);
		m_charSet = AXL_MEM_NEW(CharSet);
		m_charSet->add(m_char);

		// and fall through

	case FsmMatchKind_CharSet:
		m_charSet->add(c);
		break;

	case FsmMatchKind_AnyChar:
		break; // no change

	default:
		ASSERT(false);
	}
}

void
FsmMatchCondition::addCharRange(
	utf32_t from,
	utf32_t to
) {
	switch (m_matchKind) {
	case FsmMatchKind_Char:
		if (from == to && from == m_char)
			break; // no change

		m_matchKind = FsmMatchKind_CharSet;
		ASSERT(!m_charSet);
		m_charSet = AXL_MEM_NEW(CharSet);
		m_charSet->add(m_char);

		// and fall through

	case FsmMatchKind_CharSet:
		ASSERT(m_charSet);
		m_charSet->add(from, to);
		break;

	case FsmMatchKind_AnyChar:
		break; // no change

	default:
		ASSERT(false);
	}
}

//..............................................................................

} // namespace re
} // namespace axl
