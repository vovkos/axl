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

#include "axl_re_Regex.h"
#include "axl_re_RegexNameMgr.h"

#define _AXL_RE_QUANTIFY    0
#define _AXL_RE_NAMED_REGEX 0

namespace axl {
namespace re {

//..............................................................................

class RegexCompiler {
protected:
	enum {
		MaxQuantifier = 32,
	};

	enum TokenKind {
		TokenKind_Undefined,
		TokenKind_Char,
		TokenKind_SpecialChar,
		TokenKind_Literal,
		TokenKind_Identifier,
		TokenKind_WordBoundary,
		TokenKind_Quantifier,
	};

	struct Token {
		TokenKind m_tokenKind;
		utf32_t m_char;
		sl::String m_string;

		bool
		isSpecialChar(char c) {
			return m_tokenKind == TokenKind_SpecialChar && m_char == c;
		}

		bool
		isValidSingle();
	};

protected:
	Regex* m_regex;
	RegexNameMgr* m_nameMgr;
	uint_t m_flags;

	const char* m_p;
	const char* m_end;
	Token m_lastToken;

public:
	RegexCompiler(
		Regex* regex,
		RegexNameMgr* nameMgr = NULL
	) {
		construct(0, regex, nameMgr);
	}

	RegexCompiler(
		uint_t flags,
		Regex* regex,
		RegexNameMgr* nameMgr = NULL
	) {
		construct(flags, regex, nameMgr);
	}

	NfaState*
	compile(
		const sl::StringRef& source,
		size_t acceptId = 0
	);

	NfaState*
	compileSwitchCase(
		const sl::StringRef& source,
		size_t acceptId = 0
	);

protected:
	void
	construct(
		uint_t flags,
		Regex* regex,
		RegexNameMgr* nameMgr
	);

	NfaState*
	compileImpl(
		const NfaState* preStart,
		const sl::StringRef& source,
		size_t acceptId
	);

	bool
	readChar(utf32_t* c);

	bool
	readEscapeSequence(utf32_t* c);

	bool
	readLiteral(sl::String* string);

	bool
	readHexLiteral(sl::String* string);

	bool
	readIdentifier(sl::String* name);

#if (_AXL_RE_QUANTIFY)
	bool
	readQuantifier(size_t* count);
#endif

	bool
	getToken(Token* token);

	bool
	expectSpecialChar(char c);

	bool
	expectEof();

	NfaState*
	addState(const NfaState* preStart);

	NfaState*
	addMatchState(const NfaState* preStart);

	NfaState*
	insertSplitState(
		NfaState* beforeState,
		NfaState* splitState
	);

	NfaState*
	expression(const NfaState* preStart);

	NfaState*
	concat(const NfaState* preStart);

	NfaState*
	repeat(const NfaState* preStart);

	NfaState*
	question(NfaState* start);

	NfaState*
	star(NfaState* start);

	NfaState*
	plus(NfaState* start);

	NfaState*
	single(const NfaState* preStart);

	NfaState*
	charClass(const NfaState* preStart);

	NfaState*
	stdCharClass(
		const NfaState* preStart,
		char c
	);

	void
	stdCharClass(
		char c,
		CharSet* charSet
	);

	NfaState*
	literal(
		const NfaState* preStart,
		const sl::StringRef& string
	);

	NfaState*
	anchor(
		const NfaState* preStart,
		Anchor anchor
	);

	NfaState*
	ch(
		const NfaState* preStart,
		uint_t c
	);

	NfaState*
	any(const NfaState* preStart);

#if (_AXL_RE_QUANTIFY)
	NfaState*
	quantify(
		NfaState* start,
		size_t count
	);

	NfaState*
	clone(
		NfaState* first,
		NfaState* last
	);
#endif

	bool
	charClassItem(CharSet* charSet);

	NfaState*
	capturingGroup(const NfaState* preStart);

	NfaState*
	nonCapturingGroup(const NfaState* preStart);

#if (_AXL_RE_NAMED_REGEX)
	NfaState*
	namedRegex(const sl::StringRef& name);
#endif
};

//..............................................................................

} // namespace re
} // namespace axl
