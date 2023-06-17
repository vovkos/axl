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

#define _AXL_LEX_LEXER_H

#include "axl_lex_Token.h"
#include "axl_lex_ParseError.h"

namespace axl {
namespace lex {

//..............................................................................

template <
	typename T,
	typename Token0
>
class Lexer {
public:
	typedef Token0 Token;
	typedef typename Token::TokenKind TokenKind;
	typedef typename Token::Pos Pos;

protected:
	sl::List<Token> m_tokenList;
	mem::Pool<Token>* m_tokenPool;

public:
	Lexer() {
		// the same lexer is normally never shared among threads
		// if it does, be sure to update the token pool

		m_tokenPool = mem::getCurrentThreadPool<Token>();
	}

	static
	void
	clearTokenPool() {
		mem::getCurrentThreadPool<Token>()->clear();
	}

	void
	reset() {
		m_tokenPool->put(&m_tokenList);
		static_cast<T*>(this)->onReset();
	}

	const Token*
	getToken() {
		while (m_tokenList.isEmpty())
			static_cast<T*>(this)->tokenize();

		return *m_tokenList.getHead();
	}

	const Token*
	expectToken(int tokenKind) {
		const Token* token = getToken();
		if (token->m_token == tokenKind)
			return token;

		lex::setExpectedTokenError(Token::getName(tokenKind), token->getName());
		return NULL;
	}

	Token*
	takeToken() {
		while (m_tokenList.isEmpty())
			static_cast<T*>(this)->tokenize();

		Token* token = *m_tokenList.getHead();
		if (token->m_token > 0) // not EOF or ERROR
			return m_tokenList.removeHead();

		Token* clone = m_tokenPool->get();
		*clone = *token;
		return clone;
	}

	void
	nextToken() {
		if (!m_tokenList.isEmpty() && m_tokenList.getHead()->m_token > 0) // not EOF or ERROR
			m_tokenPool->put(m_tokenList.removeHead());
	}

	const Token*
	getToken(size_t index) {
		// check prefetched list first ...

		size_t i = 0;

		sl::Iterator<Token> it = m_tokenList.getHead();
		for (; it; it++, i++)
			if (i >= index || it->m_token <= 0) // EOF or ERROR
				return *it;

		// nope, need to tokenize more

		sl::Iterator<Token> tail = m_tokenList.getTail();

		do {
			static_cast<T*>(this)->tokenize();
		} while (
			m_tokenList.isEmpty() ||
			m_tokenList.getCount() < index && m_tokenList.getTail()->m_token > 0 // not EOF or ERROR
		);

		// start with the first new token

		it = tail ? tail.getNext() : m_tokenList.getHead();
		for (; it; it++, i++)
			if (i >= index || it->m_token <= 0) // EOF or ERROR
				return *it;

		ASSERT(false && "tokenize loop didn't add enough tokens");
		return NULL;
	}

	void
	nextToken(size_t count) {
		for (size_t i = 0; i < count; i++) {
			if (m_tokenList.isEmpty() || m_tokenList.getHead()->m_token <= 0) // EOF or ERROR
				break;

			m_tokenPool->put(m_tokenList.removeHead());
		}
	}

protected:
	void
	onReset() {}
};

//..............................................................................

} // namespace lex
} // namespace axl
