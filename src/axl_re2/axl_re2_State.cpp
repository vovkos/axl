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
#include "axl_re2_State.h"

namespace axl {
namespace re2 {

using ::re2::RE2;

//..............................................................................

class State::Impl: public RE2::SM::State {
public:
	Impl() {}

	Impl(
		RE2::Anchor anchor,
		uint64_t baseOffset,
		int baseChar,
		uint64_t eofOffset,
		int eofChar
	):
		RE2::SM::State(anchor, baseOffset, baseChar, eofOffset, eofChar) {}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

State::State(const State& src) {
	m_impl = new Impl(*src.m_impl);
}

#if (_AXL_CPP_HAS_RVALUE_REF)
State::State(State&& src) {
	m_impl = src.m_impl;
	src.m_impl = NULL;
}
#endif

void
State::init(
	Anchor anchor,
	uint64_t baseOffset,
	int baseChar,
	uint64_t eofOffset,
	int eofChar
) {
	m_impl = new Impl((RE2::Anchor)anchor, baseOffset, baseChar, eofOffset, eofChar);
}

State::~State()	{
	delete m_impl;
}

#if (_AXL_CPP_HAS_RVALUE_REF)
State&
State::operator = (State&& src) {
	delete m_impl;
	m_impl = src.m_impl;
	src.m_impl = NULL;
	return *this;
}
#endif

State&
State::operator = (const State& src) {
	*m_impl = *src.m_impl;
	return *this;
}

bool
State::isMatch() const {
	return m_impl->is_match();
}

Anchor
State::getAnchor() const {
	return (Anchor)m_impl->anchor();
}

uint64_t
State::getBaseOffset() const {
	return m_impl->base_offset();
}

uint64_t
State::getEofOffset() const {
	return m_impl->eof_offset();
}

int
State::getBaseChar() const {
	return m_impl->base_char();
}

int
State::getEofChar() const {
	return m_impl->eof_char();
}

int
State::getMatchLastChar() const {
	return m_impl->match_last_char();
}

int
State::getMatchNextChar() const {
	return m_impl->match_next_char();
}

void
State::reset(
	Anchor anchor,
	uint64_t baseOffset,
	int baseChar,
	uint64_t eofOffset,
	int eofChar
) {
	m_impl->reset((RE2::Anchor)anchor, baseOffset, baseChar, eofOffset, eofChar);
	m_match.reset();
}

void
State::setEof(
	uint64_t offset,
	int eofChar
) {
	m_impl->set_eof(offset, eofChar);
}

void
State::prepareMatch() const {
	ASSERT(m_match.m_offset == -1 && m_match.m_endOffset == -1);

	m_match.m_id = m_impl->match_id();
	m_match.m_offset = m_impl->match_offset();
	m_match.m_endOffset = m_impl->match_end_offset();
	if (m_impl->has_match_text())
		m_match.m_text = m_impl->match_text() >> toAxl;
}

//..............................................................................

} // namespace re2
} // namespace axl
