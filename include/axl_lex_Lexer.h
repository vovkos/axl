// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
	CTokenList m_TokenList;
	CTokenList m_FreeTokenList;
	size_t m_ChannelMask;
	
	CPos m_LastTokenPos;

public:
	void
	Reset ()
	{
		m_FreeTokenList.InsertListTail (&m_TokenList);
		m_ChannelMask = 1;
		m_LastTokenPos.Clear ();

		static_cast <T*> (this)->OnReset ();
	}

	size_t 
	GetChannelMask ()
	{
		return m_ChannelMask;
	}	

	void
	SetChannelMask (size_t Mask)
	{
		m_ChannelMask = Mask;
	}	

	const CToken*
	GetChannelToken (
		size_t ChannelMask, 
		size_t Index = 0
		)
	{
		ChannelMask |= 1; // ensure channel 0 is ALWAYS in the mask

		size_t i = 0;
		rtl::CBoxIteratorT <CToken> It = m_TokenList.GetHead ();

		for (;;)
		{
			// check prefetched list first ...

			for (; It; It++)
			{
				if (It->m_Token <= 0)
					return &*It;
				
				bool IsMatch = (ChannelMask & ((size_t) 1 << It->m_Channel)) != 0;
				if (IsMatch)
				{
					if (i >= Index)
					{
						m_LastTokenPos = It->m_Pos;
						return &*It;
					}

					i++;
				}
			}
			
			// ...nope, need to fetch more tokens

			rtl::CBoxIteratorT <CToken> Tail = m_TokenList.GetTail ();

			size_t OldCount = m_TokenList.GetCount ();
			do 
			{			
				((T*) (this))->Tokenize ();
			} while (m_TokenList.GetCount () == OldCount);

			It = Tail ? Tail + 1 : m_TokenList.GetHead ();
		}
	}

	void 
	NextChannelToken (
		size_t ChannelMask,
		size_t Count = 1
		)
	{
		ChannelMask |= 1; // ensure channel 0 is ALWAYS in the mask

		for (size_t i = 0; i < Count; i++)
		{
			while (!m_TokenList.IsEmpty ())
			{
				rtl::CBoxIteratorT <CToken> It = m_TokenList.GetHead ();

				if (It->m_Token <= 0) // done! but don't remove it!
					return;

				bool IsMatch = (ChannelMask & ((size_t) 1 << It->m_Channel)) != 0;

				CTokenEntry* pEntry = It.GetEntry ();
				
				m_TokenList.RemoveEntry (pEntry);
				m_FreeTokenList.InsertHeadEntry (pEntry);

				if (IsMatch)
					break;	
			}
		}
	}

	const CToken*
	GetToken (size_t Index = 0)
	{
		return GetChannelToken (m_ChannelMask, Index);
	}

	void
	NextToken (size_t Count = 1)
	{
		return NextChannelToken (m_ChannelMask, Count);
	}

protected:
	void
	OnReset ()
	{
	}

	CToken*
	AllocateToken ()
	{
		CTokenEntry* pEntry = !m_FreeTokenList.IsEmpty () ? 
			m_FreeTokenList.RemoveHeadEntry () :
			AXL_MEM_NEW (CTokenEntry);

		m_TokenList.InsertTailEntry (pEntry);
		return &pEntry->m_Value;
	}
};

//.............................................................................

} // namespace lex
} // namespace axl
