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
		uint_t execFlags,
		uint64_t baseOffset,
		int baseChar,
		uint64_t eofOffset,
		int eofChar
	):
		RE2::SM::State(execFlags, baseOffset, baseChar, eofOffset, eofChar) {}
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
	uint_t execFlags,
	uint64_t baseOffset,
	int baseChar,
	uint64_t eofOffset,
	int eofChar
) {
	m_impl = new Impl(execFlags, baseOffset, baseChar, eofOffset, eofChar);
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

uint_t
State::getExecFlags() const {
	return m_impl->exec_flags();
}

uint64_t
State::getBaseOffset() const {
	return m_impl->base_offset();
}

int
State::getBaseChar() const {
	return m_impl->base_char();
}

uint64_t
State::getEofOffset() const {
	return m_impl->eof_offset();
}

int
State::getEofChar() const {
	return m_impl->eof_char();
}

void
State::reset(
	uint_t execFlags,
	uint64_t baseOffset,
	int baseChar,
	uint64_t eofOffset,
	int eofChar
) {
	m_impl->reset(execFlags, baseOffset, baseChar, eofOffset, eofChar);
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
