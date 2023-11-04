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
		uint64_t baseOffset,
		int baseChar,
		uint64_t eofOffset,
		int eofChar
	):
		RE2::SM::State(baseOffset, baseChar, eofOffset, eofChar) {}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

State::State(const State& src) {
	m_impl = new Impl(*src.m_impl);
}

#if (_AXL_CPP_HAS_RVALUE_REF)
State::State(State&& src) {
	operator = (std::move(src));
}
#endif

void
State::init(
	uint64_t baseOffset,
	int baseChar,
	uint64_t eofOffset,
	int eofChar
) {
	m_impl = new Impl(baseOffset, baseChar, eofOffset, eofChar);
}

State::~State()	{
	delete m_impl;
}

#if (_AXL_CPP_HAS_RVALUE_REF)
State&
State::operator = (State&& src) {
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

uint64_t
State::getBaseOffset() const {
	return m_impl->base_offset();
}

uint64_t
State::getEofOffset() const {
	return m_impl->eof_offset();
}

uint64_t
State::getMatchOffset() const {
	return m_impl->match_offset();
}

uint64_t
State::getMatchEndOffset() const {
	return m_impl->match_end_offset();
}

uint64_t
State::getMatchSize() const {
	return m_impl->match_length();
}

uint_t
State::getMatchId() const {
	return m_impl->match_id();
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
	uint64_t baseOffset,
	int baseChar,
	uint64_t eofOffset,
	int eofChar
) {
	m_impl->reset(baseOffset, baseChar, eofOffset, eofChar);
}

void
State::setEof(
	uint64_t offset,
	int eofChar
) {
	m_impl->set_eof(offset, eofChar);
}

//..............................................................................

} // namespace re2
} // namespace axl
