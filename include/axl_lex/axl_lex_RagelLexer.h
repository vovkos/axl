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

namespace axl {
namespace lex {

//..............................................................................

class RagelTokenPos: public LineCol
{
public:
	size_t m_offset;
	size_t m_length;

	char* m_p;

public:
	RagelTokenPos()
	{
		m_offset = 0;
		m_length = 0;
		m_p = NULL;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename Enum,
	typename GetName,
	typename Data = StdTokenData
	>
class RagelToken: public Token<Enum, GetName, Data, RagelTokenPos>
{
public:
	sl::StringRef
	getText() const
	{
		return sl::StringRef(this->m_pos.m_p, this->m_pos.m_length);
	}

	static
	sl::String
	getTokenListString(const sl::ConstBoxList<RagelToken>& list)
	{
		if (list.isEmpty())
			return sl::String();

		sl::ConstBoxIterator<RagelToken> token = list.getHead();
		sl::String string(token->m_pos.m_p, token->m_pos.m_length);
		for (token++; token; token++)
		{
			string.append(' ');
			string.append(token->m_pos.m_p, token->m_pos.m_length);
		}

		return string;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef RagelToken<int, StdTokenData> StdRagelToken;

//..............................................................................

template <
	typename T,
	typename Token = StdRagelToken
	>
class RagelLexer: public Lexer<T, Token>
{
	friend class Lexer<T, Token>;

protected:
	size_t m_tokenizeLimit;
	size_t m_tokenizeCount;

	sl::StringRef m_filePath;

	char* m_begin;
	int m_line;
	size_t m_lineOffset;

	sl::Array<int> m_stack; // stack of states

	// ragel interface variables

	char* p;
	char* pe;
	char* eof;
	char* ts;
	char* te;
	int act;
	int cs;
	int top;
	int* stack;

public:
	RagelLexer()
	{
		onReset();
	}

	size_t
	getTokenizeLimit()
	{
		return m_tokenizeLimit;
	}

	void
	setTokenizeLimit(size_t tokenizeLimit)
	{
		ASSERT(tokenizeLimit);
		m_tokenizeLimit = tokenizeLimit;
	}

	bool
	create(
		int state,
		const sl::StringRef& filePath,
		const sl::StringRef& source
		)
	{
		create(filePath, source);
		cs = state;
		return true;
	}

	bool
	create(
		const sl::StringRef& filePath,
		const sl::StringRef& source
		)
	{
		this->reset();

		static_cast<T*> (this)->init();

		p   = (char*)source.cp();
		eof = (char*)source.getEnd();

		m_filePath = filePath;
		m_begin = p;

		return true;
	}

	const Token*
	expectToken(int tokenKind)
	{
		const Token* token = this->getToken();
		if (token->m_token == tokenKind)
			return token;

		lex::setExpectedTokenError(Token::getName(tokenKind), token->getName());
		return NULL;
	}

	void
	ensureSrcPosError()
	{
		if (!lex::isLastSrcPosError())
			lex::pushSrcPosError(m_filePath, this->m_lastTokenPos);
	}

	enum GotoStateKind
	{
		GotoStateKind_ReparseToken,
		GotoStateKind_EatToken,
	};

	void
	gotoState(
		int state,
		const Token* token,
		GotoStateKind kind
		)
	{
		p = m_begin + token->m_pos.m_offset;

		if (kind == GotoStateKind_EatToken)
			p += token->m_pos.m_length;

		m_lineOffset = token->m_pos.m_offset - token->m_pos.m_col;
		m_line = token->m_pos.m_line;
		cs = state;
		this->m_tokenList.clear();
	}

	void
	callState(int state)
	{
		int oldState = cs;

		gotoState(state);

		stack = prePush();
		stack[top++] = oldState;
	}

	void
	setLineCol(
		int line,
		int col
		)
	{
		m_line = line;
		m_lineOffset = p - m_begin - col;
	}

	void
	setLineCol(const LineCol& lineCol)
	{
		setLineCol(lineCol.m_line, lineCol.m_col);
	}

protected:
	// these are to be called from withing ragel scanner (*.rl)

	void
	newLine(char* line)
	{
		ASSERT(line[-1] == '\n');

		m_lineOffset = line - m_begin;
		m_line++;
	}

	int*
	prePush()
	{
		size_t count = m_stack.getCount();
		m_stack.setCount(count + 1);
		stack = m_stack;
		return stack;
	}

	void
	postPop()
	{
		size_t count = m_stack.getCount();
		if (!count)
			return;

		m_stack.setCount(count - 1);
	}

	void
	stop()
	{
		pe = p + 1;
	}

	void
	onLexerError()
	{
	}

	Token*
	preCreateToken(
		int tokenKind,
		uint_t channelMask = TokenChannelMask_Main
		)
	{
		size_t offset = ts - m_begin;
		size_t length = te - ts;

		Token* token = this->allocateToken();
		token->m_token = tokenKind;
		token->m_channelMask = channelMask;
		token->m_pos.m_offset = offset;
		token->m_pos.m_line = m_line;
		token->m_pos.m_col = offset - m_lineOffset;
		token->m_pos.m_length = length;
		token->m_pos.m_p = ts;
		return token;
	}

	void
	postCreateToken()
	{
		m_tokenizeCount++;
		if (m_tokenizeCount >= m_tokenizeLimit)
			stop();
	}

	Token*
	createToken(
		int tokenKind,
		uint_t channelMask = TokenChannelMask_Main
		)
	{
		Token* token = preCreateToken(tokenKind, channelMask);
		postCreateToken();
		return token;
	}

	Token*
	createErrorToken(int c)
	{
		Token* token = createToken(-1);
		token->m_data.m_integer = c;
		postCreateToken();
		return token;
	}

	void
	onReset()
	{
		p = NULL;
		pe = NULL;
		eof = NULL;
		ts = NULL;
		te = NULL;
		act = 0;
		cs = 0;
		top = 0;
		stack = NULL;

		m_begin = NULL;
		m_line = 0;
		m_lineOffset = 0;
		m_tokenizeLimit = 8;
		m_tokenizeCount = 0;

		m_filePath.clear();
		m_stack.clear();
	}

	void
	tokenize()
	{
		if (p != eof)
		{
			pe = eof;
			m_tokenizeCount = 0;
			static_cast<T*> (this)->exec();
		}
		else
		{
			// add eof token
			ts = te = p;
			createToken(0);
		}
	}

	// to be implemented in lexer instance:

	// void
	// init ()
	// {
	// }

	// void
	// exec ()
	// {
	// }
};

//..............................................................................

} // namespace lex
} // namespace axl
