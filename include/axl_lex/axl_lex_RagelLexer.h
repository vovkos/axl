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

#define _AXL_LEX_RAGELLEXER_H

#include "axl_lex_Lexer.h"
#include "axl_lex_Ragel.h"

namespace axl {
namespace lex {

//..............................................................................

class RagelTokenPos: public LineColOffset {
public:
	char* m_p;
	size_t m_length;

public:
	RagelTokenPos() {
		m_p = NULL;
		m_length = 0;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Enum,
	typename GetName,
	typename Data = StdTokenData
>
class RagelToken: public Token<Enum, GetName, Data, RagelTokenPos> {
public:
	sl::StringRef
	getText() const {
		return sl::StringRef(this->m_pos.m_p, this->m_pos.m_length);
	}

	static
	sl::StringRef
	getText(const sl::List<RagelToken>& list) {
		if (list.isEmpty())
			return sl::String();

		const char* p = list.getHead()->m_pos.m_p;
		const RagelToken* tail = *list.getTail();
		size_t length = tail->m_pos.m_p + tail->m_pos.m_length - p;
		return sl::StringRef(p, length);
	}

protected:
	static
	bool
	isSpaceNeededStd(
		int token1,
		int token2
	) {
		return token1 != '.' && token2 != '.' && token2 != ',' && token2 != ';';
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef RagelToken<int, StdTokenData> StdRagelToken;

//..............................................................................

template <
	typename T,
	typename Token = StdRagelToken
>
class RagelLexer:
	public Lexer<T, Token>,
	public Ragel {
	friend class Lexer<T, Token>;

protected:
	size_t m_tokenizeLimit;
	size_t m_tokenizeCount;

	int m_line;
	size_t m_lineOffset;

public:
	RagelLexer() {
		onReset();
	}

	size_t
	getTokenizeLimit() {
		return m_tokenizeLimit;
	}

	void
	setTokenizeLimit(size_t tokenizeLimit) {
		ASSERT(tokenizeLimit);
		m_tokenizeLimit = tokenizeLimit;
	}

	void
	create(
		int state,
		const sl::StringRef& source,
		RagelBomMode bomMode = RagelBomMode_Skip
	) {
		create(source, bomMode);
		cs = state;
	}

	void
	create(
		const sl::StringRef& source,
		RagelBomMode bomMode = RagelBomMode_Skip
	) {
		this->reset();
		static_cast<T*>(this)->init();
		Ragel::setSource(source, bomMode);
	}

	enum GotoStateKind {
		GotoStateKind_ReparseToken,
		GotoStateKind_EatToken,
	};

	void
	gotoState(
		int state,
		const Token* token,
		GotoStateKind kind
	) {
		p = m_begin + token->m_pos.m_offset;

		if (kind == GotoStateKind_EatToken)
			p += token->m_pos.m_length;

		m_lineOffset = token->m_pos.m_offset - token->m_pos.m_col;
		m_line = token->m_pos.m_line;
		cs = state;
		this->m_tokenList.clear();
	}

	void
	setLineCol(
		int line,
		int col
	) {
		m_line = line;
		m_lineOffset = p - m_begin - col;
	}

	void
	setLineColOffset(
		int line,
		int col,
		size_t offset
	) {
		setLineCol(line, col);
		m_begin -= offset;
	}

	void
	setLineCol(const LineCol& pos) {
		setLineCol(pos.m_line, pos.m_col);
	}

	void
	setLineColOffset(const LineColOffset& pos) {
		setLineColOffset(pos.m_line, pos.m_col, pos.m_offset);
	}

protected:
	// these are to be called from withing ragel scanner (*.rl)

	void
	newLine(char* line) {
		ASSERT(line[-1] == '\n');

		m_lineOffset = line - m_begin;
		m_line++;
	}

	void
	onLexerError() {}

	Token*
	preCreateToken(int tokenKind) {
		size_t offset = ts - m_begin;
		size_t length = te - ts;

		Token* token = this->m_tokenPool->get();
		token->m_token = tokenKind;
		token->m_pos.m_offset = offset;
		token->m_pos.m_line = m_line;
		token->m_pos.m_col = offset - m_lineOffset;
		token->m_pos.m_length = length;
		token->m_pos.m_p = ts;
		this->m_tokenList.insertTail(token);
		return token;
	}

	void
	postCreateToken() {
		m_tokenizeCount++;
		if (m_tokenizeCount >= m_tokenizeLimit)
			stop();
	}

	Token*
	createToken(int tokenKind) {
		Token* token = preCreateToken(tokenKind);
		postCreateToken();
		return token;
	}

	Token*
	createErrorToken(int c) {
		Token* token = createToken(-1);
		token->m_data.m_integer = c;
		stop();
		return token;
	}

	void
	onReset() {
		Ragel::clear();

		m_line = 0;
		m_lineOffset = 0;
		m_tokenizeLimit = 8;
		m_tokenizeCount = 0;
	}

	void
	tokenize() {
		if (p != eof) {
			pe = eof;
			m_tokenizeCount = 0;
			static_cast<T*>(this)->exec();
		} else {
			// add eof token
			ts = te = p;
			createToken(0);
		}
	}

	// to be implemented in lexer instance:

	// void
	// init () {
	// }

	// void
	// exec () {
	// }
};

//..............................................................................

} // namespace lex
} // namespace axl
