// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LEX_LEXER_H

#include "axl_lex_Token.h"
#include "axl_rtl_BoxList.h"
#include "axl_err_ParseError.h"

namespace axl {
namespace lex {

//.............................................................................

template <
	typename T,
	typename TToken
	>
class CLexerT
{
public:
	typedef TToken CToken;
	typedef typename TToken::EToken EToken;
	typedef typename TToken::CPos CPos;

protected:
	typedef rtl::CBoxListT <CToken, const CToken&> CTokenList;
	typedef rtl::CBoxListEntryT <CToken> CTokenEntry;

protected:
	CTokenList m_tokenList;
	CTokenList m_freeTokenList;
	size_t m_channelMask;
	
	CPos m_lastTokenPos;

public:
	void
	reset ()
	{
		m_freeTokenList.insertListTail (&m_tokenList);
		m_channelMask = 1;
		m_lastTokenPos.clear ();

		static_cast <T*> (this)->onReset ();
	}

	size_t 
	getChannelMask ()
	{
		return m_channelMask;
	}	

	void
	setChannelMask (size_t mask)
	{
		m_channelMask = mask;
	}	

	const CToken*
	getChannelToken (
		size_t channelMask, 
		size_t index = 0
		)
	{
		channelMask |= 1; // ensure channel 0 is ALWAYS in the mask

		size_t i = 0;
		rtl::CBoxIteratorT <CToken> it = m_tokenList.getHead ();

		for (;;)
		{
			// check prefetched list first ...

			for (; it; it++)
			{
				if (it->m_token <= 0)
					return &*it;
				
				bool isMatch = (channelMask & ((size_t) 1 << it->m_channel)) != 0;
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

			rtl::CBoxIteratorT <CToken> tail = m_tokenList.getTail ();

			size_t oldCount = m_tokenList.getCount ();
			do 
			{			
				((T*) (this))->tokenize ();
			} while (m_tokenList.getCount () == oldCount);

			it = tail ? tail + 1 : m_tokenList.getHead ();
		}
	}

	void 
	nextChannelToken (
		size_t channelMask,
		size_t count = 1
		)
	{
		channelMask |= 1; // ensure channel 0 is ALWAYS in the mask

		for (size_t i = 0; i < count; i++)
		{
			while (!m_tokenList.isEmpty ())
			{
				rtl::CBoxIteratorT <CToken> it = m_tokenList.getHead ();

				if (it->m_token <= 0) // done! but don't remove it!
					return;

				bool isMatch = (channelMask & ((size_t) 1 << it->m_channel)) != 0;

				CTokenEntry* entry = it.getEntry ();
				
				m_tokenList.removeEntry (entry);
				m_freeTokenList.insertHeadEntry (entry);

				if (isMatch)
					break;	
			}
		}
	}

	const CToken*
	getToken (size_t index = 0)
	{
		return getChannelToken (m_channelMask, index);
	}

	void
	nextToken (size_t count = 1)
	{
		return nextChannelToken (m_channelMask, count);
	}

protected:
	void
	onReset ()
	{
	}

	CToken*
	allocateToken ()
	{
		CTokenEntry* entry = !m_freeTokenList.isEmpty () ? 
			m_freeTokenList.removeHeadEntry () :
			AXL_MEM_NEW (CTokenEntry);

		m_tokenList.insertTailEntry (entry);
		return &entry->m_value;
	}
};

//.............................................................................

} // namespace lex
} // namespace axl
