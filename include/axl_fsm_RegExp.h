// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_FSM_REGEXP_H

#include "axl_fsm_Dfa.h"
#include "axl_rtl_String.h"

namespace axl {
namespace fsm {

//.............................................................................

class RegExp
{
	friend class RegExpCompiler;

protected:
	rtl::StdList <NfaState> m_nfaStateList;
	rtl::Array <NfaState*> m_nfaStateArray;

	rtl::StdList <DfaState> m_dfaStateList;
	rtl::Array <DfaState*> m_dfaStateArray;

public:
	bool
	isEmpty () const
	{
		return m_nfaStateList.isEmpty ();
	}

	rtl::ConstList <NfaState> 
	getNfaStateList () const
	{
		return m_nfaStateList;
	}

	rtl::Array <NfaState*> 
	getNfaStateArray () const
	{
		return m_nfaStateArray;
	}

	rtl::ConstList <DfaState> 
	getDfaStateList () const
	{		
		return m_dfaStateList;
	}

	rtl::Array <DfaState*> 
	getDfaStateArray () const
	{
		return m_dfaStateArray;
	}

	void
	clear ();

	bool 
	compile (char const* regexp);

	bool 
	match (char const* string);

	void
	print () const;
};

//.............................................................................

class RegExpCompiler
{
protected:
	enum TokenKind
	{
		TokenKind_Undefined,
		TokenKind_Char,
		TokenKind_SpecialChar,
		TokenKind_Literal,
	};

	struct Token
	{
		TokenKind m_tokenKind;
		char m_char;
		rtl::String m_literal;

		bool 
		isSpecialChar (char c)
		{
			return m_tokenKind == TokenKind_SpecialChar && m_char == c;
		}

		bool
		isValidSingle ()
		{
			return 
				m_tokenKind != TokenKind_SpecialChar || 
				m_char == '.' ||
				m_char == '[' || 
				m_char == '(' || 
				m_char == '{'; 
		}
	};
	
protected:
	RegExp* m_regExp;
	char const* m_p;
	Token m_lastToken;
	size_t m_captureId;

public:
	RegExpCompiler (RegExp* regExp);

	bool 
	compile (
		char const* source,
		void* acceptContext = NULL
		);

	bool 
	incrementalCompile (
		char const* source,
		void* acceptContext = NULL
		);

	void 
	finalize ();

protected:
	bool
	readEscapeSequence (char* c);

	bool
	readChar (char* c);

	bool
	readLiteral (rtl::String* literal);

	bool
	getToken (Token* token);

	bool
	expectSpecialChar (char c);

	NfaState*
	expression ();

	NfaState*
	concat ();

	NfaState*
	repeat ();

	NfaState*
	question (NfaState* start);

	NfaState*
	star (NfaState* start);

	NfaState*
	plus (NfaState* start);

	NfaState*
	single ();

	NfaState*
	charClass ();

	bool
	charClassItem (rtl::BitMap* charSet);

	NfaState*
	any ();

	NfaState*
	group ();

	NfaState*
	capturingGroup ();

	void 
	assignNfaIds ();

	void 
	assignDfaIds ();

	void
	makeDfa ();

	void
	minimizeDfa ();
};

//.............................................................................

} // namespace fsm
} // namespace axl

