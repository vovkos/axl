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

#define _AXL_FSM_REGEX_H

#include "axl_fsm_Dfa.h"

namespace axl {
namespace fsm {

//..............................................................................

class RegexNameMgr
{
public:
	virtual
	sl::StringRef
	findName(const sl::StringRef& name) = 0;
};

//..............................................................................

class Regex
{
	friend class RegexCompiler;

protected:
	sl::List<NfaState> m_nfaStateList;
	sl::Array<NfaState*> m_nfaStateArray;

	sl::List<DfaState> m_dfaStateList;
	sl::Array<DfaState*> m_dfaStateArray;

	size_t m_groupCount;

public:
	Regex()
	{
		m_groupCount = 0;
	}

	bool
	isEmpty() const
	{
		return m_nfaStateList.isEmpty();
	}

	size_t
	getGroupCount() const
	{
		return m_groupCount;
	}

	sl::ConstList<NfaState>
	getNfaStateList() const
	{
		return m_nfaStateList;
	}

	sl::Array<NfaState*>
	getNfaStateArray() const
	{
		return m_nfaStateArray;
	}

	sl::ConstList<DfaState>
	getDfaStateList() const
	{
		return m_dfaStateList;
	}

	sl::Array<DfaState*>
	getDfaStateArray() const
	{
		return m_dfaStateArray;
	}

	void
	clear();

	bool
	compile(const sl::StringRef& regexp);

	bool
	match(const sl::StringRef& string);

	void
	print() const;
};

//..............................................................................

class RegexCompiler
{
public:
	enum Flag
	{
		Flag_NonCapturingGroups = 0x01,
		Flag_SparseSyntax       = 0x02,
	};

	enum Const
	{
		Const_MaxQuantifier = 32,
	};

protected:
	enum TokenKind
	{
		TokenKind_Undefined,
		TokenKind_Char,
		TokenKind_SpecialChar,
		TokenKind_Literal,
		TokenKind_Identifier,
		TokenKind_Quantifier,
	};

	struct Token
	{
		TokenKind m_tokenKind;
		uchar_t m_char;
		sl::String m_string;

		bool
		isSpecialChar(char c)
		{
			return m_tokenKind == TokenKind_SpecialChar && m_char == c;
		}

		bool
		isValidSingle();
	};

protected:
	Regex* m_regex;
	RegexNameMgr* m_nameMgr;
	uint_t m_flags;

	const char* m_p;
	const char* m_end;
	Token m_lastToken;

public:
	RegexCompiler(
		Regex* regex,
		RegexNameMgr* nameMgr = NULL
		)
	{
		construct(0, regex, nameMgr);
	}

	RegexCompiler(
		uint_t flags,
		Regex* regex,
		RegexNameMgr* nameMgr = NULL
		)
	{
		construct(flags, regex, nameMgr);
	}

	bool
	compile(
		const sl::StringRef& source,
		void* acceptContext = NULL
		);

	bool
	incrementalCompile(
		const sl::StringRef& source,
		void* acceptContext = NULL
		);

	void
	finalize();

protected:
	void
	construct(
		uint_t flags,
		Regex* regex,
		RegexNameMgr* nameMgr
		);

	bool
	readHexEscapeSequence(uchar_t* c);

	bool
	readEscapeSequence(uchar_t* c);

	bool
	readLiteral(sl::String* string);

	bool
	readHexLiteral(sl::String* string);

	bool
	readIdentifier(sl::String* name);

	bool
	readQuantifier(size_t* count);

	bool
	getToken(Token* token);

	bool
	expectSpecialChar(char c);

	bool
	expectEof();

	NfaState*
	expression();

	NfaState*
	concat();

	NfaState*
	repeat();

	NfaState*
	question(NfaState* start);

	NfaState*
	star(NfaState* start);

	NfaState*
	plus(NfaState* start);

	NfaState*
	quantify(
		NfaState* start,
		size_t count
		);

	NfaState*
	single();

	NfaState*
	charClass();

	NfaState*
	stdCharClass(uint_t c);

	NfaState*
	namedRegex(const sl::StringRef& name);

	void
	stdCharClass(
		uint_t c,
		sl::BitMap* charSet
		);

	NfaState*
	literal(const sl::StringRef& string);

	NfaState*
	ch(uint_t c);

	void
	ch(
		uint_t c,
		NfaState* start
		);

	NfaState*
	any();

	NfaState*
	clone(
		NfaState* first,
		NfaState* last
		);

	bool
	charClassItem(sl::BitMap* charSet);

	NfaState*
	capturingGroup();

	NfaState*
	nonCapturingGroup();

	void
	assignNfaIds();

	void
	assignDfaIds();

	void
	makeDfa();

	void
	minimizeDfa();
};

//..............................................................................

} // namespace fsm
} // namespace axl
