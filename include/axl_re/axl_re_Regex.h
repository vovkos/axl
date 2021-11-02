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

enum RegexKind {
	RegexKind_Undefined,
	RegexKind_Normal,
	RegexKind_Switch,
	RegexKind_LexicalSwitch,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Regex {
	friend class RegexCompiler;

protected:
	struct SwitchCaseContext: sl::ListLink {
		size_t m_index;
		size_t m_firstGroupId;
		size_t m_groupCount;
		rc::Ptr<void> m_context;
	};

protected:
	RegexKind m_regexKind;
	sl::List<NfaState> m_nfaStateList;
	sl::Array<NfaState*> m_nfaStateArray;
	sl::List<DfaState> m_dfaStateList;
	NfaStateSetMap<DfaState*> m_dfaStateMap;
	sl::List<SwitchCaseContext> m_caseContextList;
	size_t m_groupCount;
	size_t m_maxSubMatchCount; // out of all switch-cases

public:
	Regex();

	bool
	isEmpty() const {
		return m_nfaStateList.isEmpty();
	}

	RegexKind
	getRegexKind() {
		return m_regexKind;
	}

	size_t
	getGroupCount() const {
		return m_groupCount;
	}

	size_t
	getMaxSubMatchCount() const {
		return m_maxSubMatchCount;
	}

	size_t
	getNfaStateCount() const {
		ASSERT(m_nfaStateList.getCount() == m_nfaStateArray.getCount());
		return m_nfaStateList.getCount();
	}

	const NfaState*
	getNfaState(size_t i) const {
		return m_nfaStateArray[i];
	}

	const DfaState*
	getStartDfaState() const {
		return *m_dfaStateList.getHead();
	}

	void
	prepareDfaState(DfaState* state);

	void
	clear();

	bool
	compile(const sl::StringRef& source);

	void
	createSwitch(RegexKind switchKind);

	void
	createSwitch() {
		createSwitch(RegexKind_Switch);
	}

	void
	createLexicalSwitch() {
		createSwitch(RegexKind_LexicalSwitch);
	}

	bool
	compileSwitchCase(
		const sl::StringRef& source,
		const rc::Ptr<void>& caseContext
		);

	void
	finalizeSwitch() {
		ASSERT(m_regexKind = RegexKind_Switch);
		finalize();
	}

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

protected:
	void
	finalize();

	DfaState*
	addDfaState(DfaState* state);
};

//..............................................................................

} // namespace re
} // namespace axl
