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
#include "axl_re2_Regex.h"

namespace axl {
namespace re2 {

using ::re2::RE2;

//..............................................................................

// convenient AXL <-> Abseil string conversions

const struct ToAbsl* toAbsl;
const struct ToAxl* toAxl;

absl::string_view
operator >> (const sl::StringRef& src, const ToAbsl*) {
	return absl::string_view(src.cp(), src.getLength());
}

sl::StringRef
operator >> (const absl::string_view& src, const ToAxl*) {
	return sl::StringRef(src.data(), src.length());
}

sl::StringRef
operator >> (const std::string& src, const ToAxl*) {
	return sl::StringRef(src.data(), src.length());
}

//..............................................................................

RE2::Options
calcRe2OptionsFromRegexFlags(uint_t flags) {
	RE2::Options options;
	options.set_longest_match(true);
	options.set_dot_nl(true);

	if (flags & RegexFlag_OneLine)
		options.set_one_line(true);
	else
		options.set_multi_line(true);

	if (flags & RegexFlag_Latin1)
		options.set_encoding(RE2::Options::EncodingLatin1);

	if (flags & RegexFlag_CaseInsensitive)
		options.set_case_sensitive(false);

	return options;
}

inline
RE2::Anchor
calcRe2AnchorFromRegexFlags(uint_t flags) {
	return (RE2::Anchor)((flags & RegexFlag_AnchorMask) >> 4);
}

//..............................................................................

class Regex::Impl: public RE2::SM {
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Regex::Regex()	{
	m_impl = new Impl;
	m_flags = 0;
}

Regex::~Regex()	{
	delete m_impl;
}

RegexKind
Regex::getRegexKind() const {
	return (RegexKind)m_impl->kind();
}

size_t
Regex::getSwitchCaseCount() const {
	return m_impl->switch_case_count();
}

size_t
Regex::getCaptureCount() const {
	return m_impl->capture_count();
}

size_t
Regex::getCaptureCount(uint_t switchCaseId) const {
	return m_impl->capture_count(switchCaseId);
}

void
Regex::clear() {
	m_impl->clear();
	m_flags = 0;
}

bool
Regex::compile(
	uint_t flags,
	const sl::StringRef& source
) {
	bool result = m_impl->create(
		source >> toAbsl,
		calcRe2OptionsFromRegexFlags(flags),
		calcRe2AnchorFromRegexFlags(flags)
	);

	if (!result)
		return err::fail(m_impl->error() >> toAxl);

	m_flags = flags;
	return true;
}

void
Regex::createSwitch(uint_t flags) {
	m_impl->create_switch(
		calcRe2OptionsFromRegexFlags(flags),
		calcRe2AnchorFromRegexFlags(flags)
	);

	m_flags = flags;
}

uint_t
Regex::compileSwitchCase(const sl::StringRef& source) {
	int id = m_impl->add_switch_case(source >> toAbsl);
	return id != -1 ? id : err::fail<uint_t>(-1, m_impl->error() >> toAxl);
}

bool
Regex::finalizeSwitch() {
	bool result = m_impl->finalize_switch();
	return result ? true : err::fail(m_impl->error() >> toAxl);
}

ExecResult
Regex::exec(
	State* state,
	const void* p,
	size_t size
) const {
	if (state->isMatch())
		state->reset(state->getMatchEndOffset(), state->getMatchLastChar());

	if (!(m_flags & RegexFlag_Stream))
		state->setEof(state->getBaseOffset() + size);

	return (ExecResult)m_impl->exec((RE2::SM::State*)state->m_impl, absl::string_view((char*)p, size));
}

ExecResult
Regex::eof(
	State* state,
	int eofChar
) const {
	return (ExecResult)m_impl->eof((RE2::SM::State*)state->m_impl, eofChar);
}

bool
Regex::captureSubmatchesImpl(
	uint_t switchCaseId,
	const void* p,
	size_t size,
	sl::StringRef* submatchArray0,
	size_t count
) const {
	char buffer[256];
	sl::Array<absl::string_view> submatchArray(rc::BufKind_Stack, buffer, sizeof(buffer));
	submatchArray.setCount(count);

	absl::string_view text((char*)p, size);

	bool result = m_impl->kind() == RE2::SM::kRegexpSwitch ?
		m_impl->capture_submatches(switchCaseId, text, submatchArray, count) :
		m_impl->capture_submatches(text, submatchArray, count);

	if (!result)
		return false;

	for (size_t i = 0; i < count; i++)
		submatchArray0[i] = submatchArray[i] >> toAxl;

	return true;
}

//..............................................................................

} // namespace re2
} // namespace axl
