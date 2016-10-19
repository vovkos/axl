// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_FSM_REGEXP_H

#include "axl_fsm_Dfa.h"

namespace axl {
namespace fsm {

//..............................................................................

class RegExpNameMgr
{
public:
	virtual
	sl::StringRef
	findName (const sl::StringRef& name) = 0;
};

//..............................................................................

class RegExp
{
	friend class RegExpCompiler;

protected:
	sl::StdList <NfaState> m_nfaStateList;
	sl::Array <NfaState*> m_nfaStateArray;

	sl::StdList <DfaState> m_dfaStateList;
	sl::Array <DfaState*> m_dfaStateArray;

public:
	bool
	isEmpty () const
	{
		return m_nfaStateList.isEmpty ();
	}

	sl::ConstList <NfaState>
	getNfaStateList () const
	{
		return m_nfaStateList;
	}

	sl::Array <NfaState*>
	getNfaStateArray () const
	{
		return m_nfaStateArray;
	}

	sl::ConstList <DfaState>
	getDfaStateList () const
	{
		return m_dfaStateList;
	}

	sl::Array <DfaState*>
	getDfaStateArray () const
	{
		return m_dfaStateArray;
	}

	void
	clear ();

	bool
	compile (const sl::StringRef& regexp);

	bool
	match (const sl::StringRef& string);

	void
	print () const;
};

//..............................................................................

class RegExpCompiler
{
protected:
	enum TokenKind
	{
		TokenKind_Undefined,
		TokenKind_Char,
		TokenKind_SpecialChar,
		TokenKind_Literal,
		TokenKind_Identifier,
	};

	struct Token
	{
		TokenKind m_tokenKind;
		uchar_t m_char;
		sl::String m_string;

		bool
		isSpecialChar (char c)
		{
			return m_tokenKind == TokenKind_SpecialChar && m_char == c;
		}

		bool
		isValidSingle ();
	};

protected:
	RegExp* m_regExp;
	RegExpNameMgr* m_nameMgr;

	const char* m_p;
	Token m_lastToken;
	size_t m_captureId;

public:
	RegExpCompiler (
		RegExp* regExp,
		RegExpNameMgr* nameMgr = NULL
		);

	bool
	compile (
		const sl::StringRef& source,
		void* acceptContext = NULL
		);

	bool
	incrementalCompile (
		const sl::StringRef& source,
		void* acceptContext = NULL
		);

	void
	finalize ();

protected:
	bool
	readHexEscapeSequence (uchar_t* c);

	bool
	readEscapeSequence (uchar_t* c);

	bool
	readLiteral (sl::String* string);

	bool
	readHexLiteral (sl::String* string);

	bool
	readIdentifier (sl::String* name);

	bool
	getToken (Token* token);

	bool
	expectSpecialChar (char c);

	bool
	expectEof ();

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

	NfaState*
	stdCharClass (uint_t c);

	NfaState*
	namedRegExp (const sl::StringRef& name);

	void
	stdCharClass (
		uint_t c,
		sl::BitMap* charSet
		);

	NfaState*
	literal (const sl::StringRef& string);

	NfaState*
	ch (uint_t c);

	void
	ch (
		uint_t c,
		NfaState* start
		);

	NfaState*
	any ();

	bool
	charClassItem (sl::BitMap* charSet);

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

//..............................................................................

} // namespace fsm
} // namespace axl

