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
class Lexer
{
public:
	typedef Token0 Token;
	typedef typename Token::TokenKind TokenKind;
	typedef typename Token::Pos Pos;

protected:
	typedef sl::BoxList<Token, const Token&> TokenList;
	typedef sl::BoxListEntry<Token> TokenEntry;

protected:
	TokenList m_tokenList;
	TokenList m_freeTokenList;

	Pos m_lastTokenPos;

public:
	uint_t m_channelMask;

public:
	Lexer()
	{
		m_channelMask = TokenChannelMask_Main;
	}

	void
	reset()
	{
		m_freeTokenList.insertListTail(&m_tokenList);
		m_lastTokenPos.clear();

		static_cast<T*> (this)->onReset();
	}

	const Token*
	getChannelToken(
		uint_t channelMask,
		size_t index = 0
		)
	{
		channelMask |= TokenChannelMask_Main; // make sure main channel is ALWAYS in the mask

		size_t i = 0;
		sl::BoxIterator<Token> it = m_tokenList.getHead();

		for (;;)
		{
			// check prefetched list first ...

			for (; it; it++)
			{
				if (it->m_token <= 0)
					return &*it;

				bool isMatch = (channelMask & it->m_channelMask) != 0;
				if (isMatch)
				{
					if (i >= index)
					{
						m_lastTokenPos = it->m_pos;
						return &*it;
					}

					i++;
				}
			}

			// ...nope, need to fetch more tokens

			sl::BoxIterator<Token> tail = m_tokenList.getTail();

			size_t oldCount = m_tokenList.getCount();
			do
			{
				((T*)(this))->tokenize();
			} while (m_tokenList.getCount() == oldCount);

			it = tail ? tail + 1 : m_tokenList.getHead();
		}
	}

	void
	nextChannelToken(
		uint_t channelMask,
		size_t count = 1
		)
	{
		channelMask |= TokenChannelMask_Main; // make sure main channel is ALWAYS in the mask

		for (size_t i = 0; i < count; i++)
		{
			while (!m_tokenList.isEmpty())
			{
				sl::BoxIterator<Token> it = m_tokenList.getHead();

				if (it->m_token <= 0) // done! but don't remove it!
					return;

				bool isMatch = (channelMask & it->m_channelMask) != 0;

				TokenEntry* entry = it.getEntry();

				m_tokenList.removeEntry(entry);
				m_freeTokenList.insertHeadEntry(entry);

				if (isMatch)
					break;
			}
		}
	}

	const Token*
	getToken(size_t index = 0)
	{
		return getChannelToken(m_channelMask, index);
	}

	void
	nextToken(size_t count = 1)
	{
		return nextChannelToken(m_channelMask, count);
	}

protected:
	void
	onReset()
	{
	}

	Token*
	allocateToken()
	{
		TokenEntry* entry = !m_freeTokenList.isEmpty() ?
			m_freeTokenList.removeHeadEntry() :
			AXL_MEM_NEW(TokenEntry);

		m_tokenList.insertTailEntry(entry);
		return &entry->m_value;
	}
};

//..............................................................................

} // namespace lex
} // namespace axl
