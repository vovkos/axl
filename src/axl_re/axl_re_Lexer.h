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

#define _AXL_RE_LEXER_H

#include "axl_re_Pch.h"

namespace axl {
namespace re {

//..............................................................................

enum TokenKind {
	TokenKind_Undefined,
	TokenKind_Error,
	TokenKind_Eof,

	TokenKind_Question,
	TokenKind_Star,
	TokenKind_Plus,
	TokenKind_NonGreedyQuestion,
	TokenKind_NonGreedyStar,
	TokenKind_NonGreedyPlus,
	TokenKind_Pipe,
	TokenKind_Quantifier,
	TokenKind_EndGroup,

	TokenKind_Dash,
	TokenKind_EndCharClass,

	TokenKind_Number,
	TokenKind_Comma,
	TokenKind_EndQuantifier,

	TokenKind_Char,
	TokenKind_AnyChar,
	TokenKind_CharClass,
	TokenKind_NegatedCharClass,
	TokenKind_StdCharClassDigit,
	TokenKind_StdCharClassHex,
	TokenKind_StdCharClassWord,
	TokenKind_StdCharClassSpace,
	TokenKind_StdCharClassNonDigit,
	TokenKind_StdCharClassNonHex,
	TokenKind_StdCharClassNonWord,
	TokenKind_StdCharClassNonSpace,
	TokenKind_AnchorBeginLine,
	TokenKind_AnchorEndLine,
	TokenKind_AnchorBeginText,
	TokenKind_AnchorEndText,
	TokenKind_AnchorWordBoundary,
	TokenKind_AnchorNotWordBoundary,
	TokenKind_Group,
	TokenKind_NonCapturingGroup,

	TokenKind_FirstValid       = TokenKind_Question,
	TokenKind_FirstValidSingle = TokenKind_Char,
	TokenKind_FirstStdCharClass = TokenKind_StdCharClassDigit,
	TokenKind_LastStdCharClass = TokenKind_StdCharClassNonSpace,
};

const char*
getTokenString(TokenKind tokenKind);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// token is so small we can pass it by value everywhere

struct Token {
	TokenKind m_tokenKind;
	union {
		uint32_t m_data;
		utf32_t m_char;
		uint32_t m_number;
	};

	Token(TokenKind tokenKind = TokenKind_Undefined) {
		m_tokenKind = tokenKind;
	}

	Token(
		TokenKind tokenKind,
		uint32_t data
	) {
		m_tokenKind = tokenKind;
		m_data = data;
	}

	operator TokenKind () const {
		return m_tokenKind;
	}

	bool
	isValidSingle() const {
		return m_tokenKind >= TokenKind_FirstValidSingle;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// lex::RagelLexer would be an overkill

class Lexer: public lex::Ragel {
public:
	enum {
		TokenBufferSize = 8,
	};

protected:
	Token m_tokenBuffer[TokenBufferSize];
	size_t m_readIdx;
	size_t m_writeIdx;
	size_t m_tokenCount;

public:
	Lexer() {
		m_readIdx = m_writeIdx = m_tokenCount = 0;
	}

	void
	create(const sl::StringRef& source);

	void
	reset();

	Token
	getToken();

	Token
	getNextToken();

	Token
	nextToken();

private:
	size_t
	getNextReadIdx() {
		return m_readIdx < TokenBufferSize - 1 ? m_readIdx + 1 : 0;
	}

	size_t
	getNextWriteIdx() {
		return m_writeIdx < TokenBufferSize - 1 ? m_writeIdx + 1 : 0;
	}

	void
	tokenize();

	// these are only to be called from withing ragel scanner (*.rl)

	void
	createToken(Token token);

	void
	createCharToken(utf32_t c) {
		createToken(Token(TokenKind_Char, c));
	}

	void
	createNumberToken(uint_t n) {
		createToken(Token(TokenKind_Number, n));
	}

	void
	createUtf8CharToken_2(const char* p) {
		createCharToken(
			((p[0] & 0x1f) << 6)  |
			(p[1] & 0x3f)
		);
	}

	void
	createUtf8CharToken_3(const char* p) {
		createCharToken(
			((p[0] & 0x0f) << 12) |
			((p[1] & 0x3f) << 6)  |
			(p[2] & 0x3f)
		);
	}

	void
	createUtf8CharToken_4(const char* p) {
		createCharToken(
			((p[0] & 0x07) << 18) |
			((p[1] & 0x3f) << 12) |
			((p[2] & 0x3f) << 6)  |
			(p[3] & 0x3f)
		);
	}

	void
	createHexCharToken_2(const char* p) {
		createCharToken(
			enc::HexEncoding::getHexCharValue(p[0]) << 4 |
			enc::HexEncoding::getHexCharValue(p[1])
		);
	}

	void
	createHexCharToken_4(const char* p) {
		createCharToken(
			enc::HexEncoding::getHexCharValue(p[0]) << 12 |
			enc::HexEncoding::getHexCharValue(p[1]) << 8 |
			enc::HexEncoding::getHexCharValue(p[2]) << 4 |
			enc::HexEncoding::getHexCharValue(p[3])
		);
	}

	void
	createHexCharToken_8(const char* p) {
		createCharToken(
			enc::HexEncoding::getHexCharValue(p[0]) << 28 |
			enc::HexEncoding::getHexCharValue(p[1]) << 24 |
			enc::HexEncoding::getHexCharValue(p[2]) << 20 |
			enc::HexEncoding::getHexCharValue(p[3]) << 16 |
			enc::HexEncoding::getHexCharValue(p[4]) << 12 |
			enc::HexEncoding::getHexCharValue(p[5]) << 8 |
			enc::HexEncoding::getHexCharValue(p[6]) << 4 |
			enc::HexEncoding::getHexCharValue(p[7])
		);
	}

	void
	createOctCharToken(const char* p) {
		createCharToken(
			(p[0] - '0') << 6 |
			(p[1] - '0') << 3 |
			(p[2] - '0')
		);
	}

	// implemented in lexer.rl

	void
	init();

	bool
	exec();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
Token
Lexer::getToken() {
	if (!m_tokenCount)
		tokenize();

	return m_tokenBuffer[m_readIdx];
}

inline
Token
Lexer::getNextToken() {
	if (m_tokenCount < 2)
		tokenize();

	return m_tokenBuffer[getNextReadIdx()];
}

inline
Token
Lexer::nextToken() {
	ASSERT(m_tokenCount);

	Token token = m_tokenBuffer[m_readIdx];
	if (token.m_tokenKind > TokenKind_FirstValid)
		m_readIdx = getNextReadIdx();

	return token;
}

inline
void
Lexer::tokenize() {
	ASSERT(m_tokenCount < 2); // otherwise, why are we tokenizing?

	if (p == eof)
		createToken(TokenKind_Eof);
	else if (!exec())
		createToken(TokenKind_Error);
	else
		ASSERT(m_tokenCount >= 2);
}

//..............................................................................

} // namespace re
} // namespace axl
