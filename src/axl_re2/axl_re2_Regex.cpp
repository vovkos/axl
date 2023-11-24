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
#include "axl_re2_Storage.h"

namespace axl {
namespace re2 {

using ::re2::RE2;
using ::re2::StringPiece;

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

//..............................................................................

class Regex::Impl: public RE2::SM {
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
Regex::init() {
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
Regex::getCaptureCount() const {
	return m_impl->capture_count();
}

sl::StringRef
Regex::getPattern() const {
	return m_impl->pattern() >> toAxl;
}

size_t
Regex::getSwitchCaseCount() const {
	return m_impl->switch_case_count();
}

size_t
Regex::getSwitchCaseCaptureCount(uint_t switchCaseId) const {
	return m_impl->capture_count(switchCaseId);
}

sl::StringRef
Regex::getSwitchCasePattern(uint_t switchCaseId) const {
	return m_impl->pattern(switchCaseId) >> toAxl;
}

// compilation

void
Regex::clear() {
	m_impl->clear();
	m_flags = 0;
}

bool
Regex::compile(
	const sl::StringRef& source,
	uint_t flags
) {
	bool result = m_impl->create(
		source >> toRe2,
		calcRe2OptionsFromRegexFlags(flags)
	);

	if (!result)
		return err::fail(m_impl->error() >> toAxl);

	m_flags = flags;
	return true;
}

void
Regex::createSwitch(uint_t flags) {
	m_impl->create_switch(calcRe2OptionsFromRegexFlags(flags));
	m_flags = flags;
}

uint_t
Regex::compileSwitchCase(const sl::StringRef& source) {
	int id = m_impl->add_switch_case(source >> toRe2);
	return id != -1 ? id : err::fail<uint_t>(-1, m_impl->error() >> toAxl);
}

bool
Regex::finalizeSwitch() {
	bool result = m_impl->finalize_switch();
	return result ? true : err::fail(m_impl->error() >> toAxl);
}

// serialization

size_t
Regex::load(
	const void* data,
	size_t size
) {
	if (size < sizeof(StorageHdr))
		return err::fail<size_t>(-1, "regex storage buffer too small");

	clear();

	const StorageHdr* hdr = (const StorageHdr*)data;
	if (hdr->m_signature != StorageSignature ||
		hdr->m_version != StorageVersion_Current ||
		hdr->m_dataSize > size - sizeof(StorageHdr)
	)
		return err::fail<size_t>(-1, "invalid regex storage");

	RE2::Options options = calcRe2OptionsFromRegexFlags(hdr->m_flags);
	switch (hdr->m_regexKind) {
	case RegexKind_Single: {
		if (hdr->m_switchCaseCount)
			return err::fail<size_t>(-1, "regex kind mismatch");

		StringPiece pattern((char*)(hdr + 1), hdr->m_dataSize);
		bool result = m_impl->create(pattern, options);
		if (!result)
			return err::fail<size_t>(-1, "invalid regex pattern syntax");

		if (hdr->m_captureCount != m_impl->capture_count())
			return err::fail<size_t>(-1, "regex capture count mismatch");

		return sizeof(StorageHdr) + hdr->m_dataSize;
		}

	case RegexKind_Switch:
		break;

	default:
		return err::fail<size_t>(-1, "invalid regex kind");
	}

	// load switch-case

	m_impl->create_switch(options);
	const char* p = (char*)(hdr + 1);
	const char* end = p + hdr->m_dataSize;

	for (size_t i = 0; i < hdr->m_switchCaseCount; i++) {
		size_t leftoverSize = end - p;
		const SwitchCaseHdr* hdr = (const SwitchCaseHdr*)p;
		if (leftoverSize < sizeof(SwitchCaseHdr) ||
			hdr->m_signature != SwitchCaseSignature ||
			leftoverSize < sizeof(SwitchCaseHdr) + hdr->m_length
		)
			return err::fail<size_t>(-1, "invalid regex storage");

		p = (char*)(hdr + 1);
		StringPiece pattern(p, hdr->m_length);
		int id = m_impl->add_switch_case(pattern);
		if (id == -1)
			return err::fail<size_t>(-1, "invalid regex pattern syntax");

		if (hdr->m_captureCount != m_impl->capture_count(id))
			return err::fail<size_t>(-1, "regex capture count mismatch");

		p += hdr->m_length;
	}

	bool result = m_impl->finalize_switch();
	if (!result)
		return err::fail<size_t>(-1, "can't compile final RE2 program");

	return sizeof(StorageHdr) + hdr->m_dataSize;
}

size_t
Regex::save(sl::Array<char>* buffer) const {
	buffer->clear();

	buffer->appendEmptySpace(sizeof(StorageHdr));
	StorageHdr* hdr = (StorageHdr*)buffer->p();
	hdr->m_signature = StorageSignature;
	hdr->m_version = StorageVersion_Current;
	hdr->m_regexKind = getRegexKind();
	hdr->m_flags = getFlags();

	switch (getRegexKind()) {
	case RegexKind_Single: {
		const std::string& pattern = m_impl->pattern();
		hdr->m_switchCaseCount = 0;
		hdr->m_captureCount = getCaptureCount();
		hdr->m_dataSize = pattern.length();
		return buffer->append(pattern.data(), pattern.length());
		}

	case RegexKind_Switch:
		break;

	default:
		return err::fail<size_t>(-1, "can't save uninitialized regex");
	}

	size_t count = getSwitchCaseCount();
	hdr->m_switchCaseCount = count;
	hdr->m_captureCount = 0;

	for (size_t i = 0; i < count; i++) {
		const std::string& pattern = m_impl->pattern(i);
		size_t offset = buffer->getCount();
		buffer->appendEmptySpace(sizeof(SwitchCaseHdr));
		SwitchCaseHdr* hdr = (SwitchCaseHdr*)(buffer->p() + offset);
		hdr->m_signature = SwitchCaseSignature;
		hdr->m_captureCount = m_impl->capture_count(i);
		hdr->m_length = pattern.length();
		buffer->append(pattern.data(), pattern.length());
	}

	((StorageHdr*)buffer->p())->m_dataSize = buffer->getCount() - sizeof(StorageHdr);
	return buffer->getCount();
}

// execution

ExecResult
Regex::exec(
	State* state,
	const sl::StringRef& chunk
) const {
	if (state->m_match.isValid())
		state->m_match.reset();
	return (ExecResult)m_impl->exec((RE2::SM::State*)state->m_impl, chunk >> toRe2);
}

ExecResult
Regex::execEof(
	State* state,
	const sl::StringRef& lastChunk,
	int eofChar
) const {
	if (state->m_match.isValid())
		state->m_match.reset();
	return (ExecResult)m_impl->exec_eof((RE2::SM::State*)state->m_impl, lastChunk >> toRe2, eofChar);
}

size_t
Regex::captureSubmatchesImpl(
	RegexKind kind,
	uint_t switchCaseId,
	uint64_t matchOffset,
	const sl::StringRef& matchText_axl,
	Capture* submatchArray_axl,
	size_t count
) const {
	char buffer[256];
	sl::Array<StringPiece> submatchArray_re2(rc::BufKind_Stack, buffer, sizeof(buffer));
	submatchArray_re2.setCount(count);

	StringPiece matchText_re2 = matchText_axl >> toRe2;

	count = kind == RegexKind_Switch ?
		m_impl->capture_submatches(switchCaseId, matchText_re2, submatchArray_re2, count) :
		m_impl->capture_submatches(matchText_re2, submatchArray_re2, count);

	if (count == -1)
		return -1;

	const char* p0 = matchText_re2.data();
	for (size_t i = 0; i < count; i++) {
		Capture* dst = &submatchArray_axl[i];
		const StringPiece& src = submatchArray_re2[i];
		if (!src.data()) {
			dst->reset();
			continue;
		}

		dst->m_offset = matchOffset + src.data() - p0;
		dst->m_endOffset = dst->m_offset + src.length();
		dst->m_text = src >> toAxl;
	}

	return count;
}

//..............................................................................

} // namespace re2
} // namespace axl
