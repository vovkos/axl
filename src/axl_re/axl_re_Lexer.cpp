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
#include "axl_re_Lexer.h"
#include "axl_re_Lexer.rl.cpp"

namespace axl {
namespace re {

//..............................................................................

const char*
getTokenString(TokenKind tokenKind) {
	static const char* stringTable[] = {
		"undefined",  // TokenKind_Undefined,
		"error",      // TokenKind_Error,
		"eof",        // TokenKind_Eof,
		"?",          // TokenKind_Question,
		"*",          // TokenKind_Star,
		"+",          // TokenKind_Plus,
		"??",         // TokenKind_NonGreedyQuestion,
		"*?",         // TokenKind_NonGreedyStar,
		"+?",         // TokenKind_NonGreedyPlus,
		"|",          // TokenKind_Pipe,
		"{",          // TokenKind_Quantifier,
		")",          // TokenKind_EndGroup,
		"-",          // TokenKind_Dash,
		"]",          // TokenKind_EndCharClass,
		"number",     // TokenKind_Number,
		",",          // TokenKind_Comma,
		"}",          // TokenKind_EndQuantifier,
		"char",       // TokenKind_Char,
		".",          // TokenKind_AnyChar,
		"[",          // TokenKind_CharClass,
		"[^",         // TokenKind_NegatedCharClass,
		"\\d",        // TokenKind_StdCharClassDigit,
		"\\h",        // TokenKind_StdCharClassHex,
		"\\w",        // TokenKind_StdCharClassWord,
		"\\s",        // TokenKind_StdCharClassSpace,
		"\\D",        // TokenKind_StdCharClassNonDigit,
		"\\H",        // TokenKind_StdCharClassNonHex,
		"\\W",        // TokenKind_StdCharClassNonWord,
		"\\S",        // TokenKind_StdCharClassNonSpace,
		"^",          // TokenKind_AnchorBeginLine,
		"$",          // TokenKind_AnchorEndLine,
		"\\A",        // TokenKind_AnchorBeginText,
		"\\z",        // TokenKind_AnchorEndText,
		"\\b",        // TokenKind_AnchorWordBoundary,
		"\\B",        // TokenKind_AnchorNotWordBoundary,
		"(",          // TokenKind_Group,
		"(?:",        // TokenKind_NonCapturingGroup,
	};

	return (size_t)tokenKind < countof(stringTable) ?
		stringTable[tokenKind] :
		"invalid-token";
}

//..............................................................................

void
Lexer::create(const sl::StringRef& source) {
	reset();
	init();
	setSource(source, true);
}

void
Lexer::reset() {
	lex::Ragel::clear();

	m_readIdx = 0;
	m_writeIdx = 0;
	m_tokenCount = 0;
}

void
Lexer::createToken(Token token) {
	ASSERT(m_tokenCount < TokenBufferSize); // we should stop before that happens

	m_tokenBuffer[m_writeIdx] = token;
	if (m_writeIdx == TokenBufferSize - 1)
		m_writeIdx = 0;
	else
		m_writeIdx++;

	if (++m_tokenCount == TokenBufferSize) {
		ASSERT(m_writeIdx == m_readIdx);
		stop();
	}
}

//..............................................................................

} // namespace re
} // namespace axl
