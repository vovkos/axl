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

#define _AXL_RE_REGEX_H

#include "axl_re_RegexState.h"

namespace axl {
namespace re {

//..............................................................................

class Regex {
	friend class RegexCompiler;

protected:
	sl::List<NfaState> m_stateList;
	sl::Array<NfaState*> m_stateArray;
	size_t m_groupCount;

public:
	Regex() {
		m_groupCount = 0;
	}

	bool
	isEmpty() const {
		return m_stateList.isEmpty();
	}

	size_t
	getGroupCount() const {
		return m_groupCount;
	}

	size_t
	getStateCount() const {
		ASSERT(m_stateList.getCount() == m_stateArray.getCount());
		return m_stateList.getCount();
	}

	NfaState*
	getState(size_t i) const {
		return m_stateArray[i];
	}

	void
	clear();

	bool
	compile(const sl::StringRef& regexp);

	bool
	match(
		enc::CharCodec* codec,
		const void* p,
		size_t size
	);

	bool
	match(
		enc::CharCodecKind codecKind,
		const void* p,
		size_t size
	) {
		return match(enc::getCharCodec(codecKind), p, size);
	}

	bool
	match(const sl::StringRef& string) {
		return match(enc::CharCodecKind_Utf8, string.cp(), string.getLength());
	}

#if (_AXL_DEBUG)
	void
	print() const;
#endif
};

//..............................................................................

} // namespace re
} // namespace axl
