// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_LEX_RAGELLEXER_H

#include "axl_lex_Lexer.h"

namespace axl {
namespace lex {

//.............................................................................
	
class CRagelTokenPos: public CLineCol
{
public:
	size_t m_Offset;
	size_t m_Length;

	char* m_p;

public:
	CRagelTokenPos ()
	{
		m_Offset = 0;
		m_Length = 0;
		m_p = NULL;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename TEnum,
	typename TGetName,
	typename TData = CStdTokenData
	>
class CRagelTokenT: public CTokenT <TEnum, TGetName, TData, CRagelTokenPos>
{
public:
	rtl::CString
	GetText () const
	{
		return rtl::CString (this->m_Pos.m_p, this->m_Pos.m_Length); // thanks a lot gcc
	}

	static
	rtl::CString 
	GetTokenListString (const rtl::CConstBoxListT <CRagelTokenT>& List)
	{
		if (List.IsEmpty ())
			return rtl::CString ();

		rtl::CBoxIteratorT <CRagelTokenT> Token = List.GetHead ();
		rtl::CString String (Token->m_Pos.m_p, Token->m_Pos.m_Length);
		for (Token++; Token; Token++)
		{
			String.Append (' ');
			String.Append (Token->m_Pos.m_p, Token->m_Pos.m_Length);
		}

		return String;
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef CRagelTokenT <int, CStdTokenData> CStdRagelToken;

//.............................................................................

template <
	typename T,
	typename TToken = CStdRagelToken
	>
class CRagelLexerT: public CLexerT <T, TToken>
{
	friend class CLexerT <T, TToken>;

protected:
	size_t m_TokenizeLimit;
	size_t m_TokenizeCount;

	rtl::CString m_FilePath;

	char* m_pBegin;
	int m_Line;
	size_t m_LineOffset;

	rtl::CArrayT <int> m_Stack; // stack of states

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
	CRagelLexerT ()
	{
		OnReset ();
	}

	size_t 
	GetTokenizeLimit ()
	{
		return m_TokenizeLimit;
	}

	void
	SetTokenizeLimit (size_t TokenizeLimit)
	{
		ASSERT (TokenizeLimit);
		m_TokenizeLimit = TokenizeLimit;
	}

	bool
	Create (
		int State,
		const rtl::CString& FilePath,
		const rtl::CString& Source
		)
	{
		return Create (State, FilePath, Source, Source.GetLength ());
	}

	bool
	Create (
		const rtl::CString& FilePath,
		const rtl::CString& Source
		)
	{
		return Create (FilePath, Source, Source.GetLength ());
	}

	bool
	Create (
		int State,
		const rtl::CString& FilePath,
		const char* pSource,
		size_t Length = -1
		)
	{
		Create (FilePath, pSource, Length);
		cs = State;
		return true;
	}

	bool
	Create (
		const rtl::CString& FilePath,
		const char* pSource,
		size_t Length = -1
		)
	{
		this->Reset (); 
		
		static_cast <T*> (this)->Init ();

		p   = (char*) pSource;
		eof = (char*) pSource + Length;

		m_FilePath = FilePath;
		m_pBegin = p;

		return true;
	}

	const TToken*
	ExpectToken (int Token)
	{
		const TToken* pToken = this->GetToken (); 
		if (pToken->m_Token == Token)
			return pToken;

		err::SetExpectedTokenError (TToken::GetName (Token), pToken->GetName ());
		return NULL;
	}

	void
	EnsureSrcPosError ()
	{
		err::CError Error = err::GetError ();
		if (!Error->IsKind (err::GUID_ParseError, err::EParseError_SrcPos))
			err::PushSrcPosError (
				m_FilePath, 
				this->m_LastTokenPos.m_Line, 
				this->m_LastTokenPos.m_Col 
				);
	}

	enum EGotoState
	{
		EGotoState_ReparseToken,
		EGotoState_EatToken,
	};

	void 
	GotoState (
		int State,
		const TToken* pToken,
		EGotoState Kind
		)
	{
		p = m_pBegin + pToken->m_Pos.m_Offset;

		if (Kind == EGotoState_EatToken)
			p += pToken->m_Pos.m_Length;

		m_LineOffset = pToken->m_Pos.m_Offset - pToken->m_Pos.m_Col;
		m_Line = pToken->m_Pos.m_Line;
		cs = State;
		this->m_TokenList.Clear(); 
	}

	void
	CallState (int State)
	{
		int OldState = cs;

		GotoState (State);

		stack = PrePush ();
		stack [top++] = OldState; 
	}

	void
	SetLineCol (
		int Line,
		int Col
		)
	{
		m_Line = Line;
		m_LineOffset = p - m_pBegin - Col;
	}

	void
	SetLineCol (const CLineCol& LineCol)
	{
		SetLineCol (LineCol.m_Line, LineCol.m_Col);
	}

protected:
	// these are to be called from withing ragel scanner (*.rl)
	
	void 
	NewLine (char* pLine)
	{ 
		ASSERT (pLine [-1] == '\n');

		m_LineOffset = pLine - m_pBegin;
		m_Line++;
	}

	int* 
	PrePush ()
	{ 
		size_t Count = m_Stack.GetCount ();
		m_Stack.SetCount (Count + 1);
		stack = m_Stack;
		return stack;
	}

	void 
	PostPop ()
	{ 
		size_t Count = m_Stack.GetCount ();
		if (!Count)
			return;

		m_Stack.SetCount (Count - 1);
	}

	void
	Break ()
	{
		pe = p + 1;
	}

	void 
	OnLexerError ()
	{	
	}
	
	TToken*
	PreCreateToken (
		int Token,
		size_t Channel = 0
		)
	{
		size_t Offset = ts - m_pBegin;
		size_t Length = te - ts;

		TToken* pToken = this->AllocateToken (); 
		pToken->m_Token = Token;
		pToken->m_Channel = Channel;
		pToken->m_Pos.m_Offset = Offset;
		pToken->m_Pos.m_Line = m_Line;
		pToken->m_Pos.m_Col = Offset - m_LineOffset;
		pToken->m_Pos.m_Length = Length;
		pToken->m_Pos.m_p = ts;
		return pToken;
	}

	void
	PostCreateToken ()
	{
		m_TokenizeCount++;
		if (m_TokenizeCount >= m_TokenizeLimit)
			Break ();
	}

	TToken*
	CreateToken (
		int Token,
		size_t Channel = 0
		)
	{
		TToken* pToken = PreCreateToken (Token, Channel);
		PostCreateToken ();
		return pToken;
	}

	TToken*
	CreateErrorToken (int Char)
	{
		TToken* pToken = CreateToken (-1);
		pToken->m_Data.m_Integer = Char;
		PostCreateToken ();
		return pToken;
	}
	
	void 
	OnReset ()
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

		m_pBegin = NULL;
		m_Line = 0;
		m_LineOffset = 0;
		m_TokenizeLimit = 8;
		m_TokenizeCount = 0;

		m_FilePath.Clear ();
		m_Stack.Clear ();
	}

	void
	Tokenize ()
	{
		if (p != eof) 
		{
			pe = eof;
			m_TokenizeCount = 0;
			static_cast <T*> (this)->Exec ();
		}
		else
		{
			// add eof token
			ts = te = p;
			CreateToken (0);
		}
	}


	// to be implemented in lexer instance:

	// void
	// Init ()
	// {
	// }

	// void
	// Exec ()
	// {
	// }
};

//.............................................................................

} // namespace lex 
} // namespace axl
