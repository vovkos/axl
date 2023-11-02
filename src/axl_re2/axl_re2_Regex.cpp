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

class Regex::Impl: public RE2::SM {
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Regex::Regex()	{
	m_impl = new Regex::Impl;
	m_regexKind = RegexKind_Undefined;
}

Regex::~Regex()	{
	delete m_impl;
}

void
Regex::clear() {
	m_regexKind = RegexKind_Undefined;
	m_impl->clear();
}

bool
Regex::compile(
	uint_t flags,
	const sl::StringRef& source
) {
	RE2::Options options;
	RE2::Anchor anchor = RE2::UNANCHORED;

	bool result = m_impl->create(
		absl::string_view(source.cp(), source.getLength()),
		options,
		anchor
	);

	if (!result) {
		err::setError(sl::StringRef(m_impl->error().data(), m_impl->error().length()));
		return false;
	}

	return true;
}

//..............................................................................

} // namespace re2
} // namespace axl
