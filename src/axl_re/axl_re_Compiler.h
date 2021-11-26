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

#include "axl_re_Lexer.h"
#include "axl_re_Nfa.h"
#include "axl_re_NameMgr.h"

namespace axl {
namespace re {

//..............................................................................

class Compiler: protected Lexer {
protected:
	NfaProgram* m_program;
	RegexNameMgr* m_nameMgr;
	uint_t m_flags;

public:
	Compiler(
		NfaProgram* program,
		uint_t flags
	) {
		construct(NULL, program, flags);
	}

	Compiler(
		RegexNameMgr* nameMgr,
		NfaProgram* program,
		uint_t flags
	) {
		construct(nameMgr, program, flags);
	}

	NfaState*
	compile(
		const sl::StringRef& source,
		size_t acceptId
	);

	NfaState*
	compileSwitchCase(
		const sl::StringRef& source,
		size_t acceptId
	);

protected:
	void
	construct(
		RegexNameMgr* nameMgr,
		NfaProgram* program,
		uint_t flags
	);

	bool
	expectToken(TokenKind tokenKind);

	NfaState*
	expression();

	NfaState*
	concat();

	NfaState*
	repeat();

	template <typename IsNonGreedy>
	NfaState*
	question(NfaState* start);

	template <typename IsNonGreedy>
	NfaState*
	star(NfaState* start);

	template <typename IsNonGreedy>
	NfaState*
	plus(NfaState* start);

	NfaState*
	single();

	template <typename IsNegated>
	NfaState*
	charClass();

	template <typename StdCharClass>
	NfaState*
	stdCharClass();

	NfaState*
	anchor(Anchor anchor);

	NfaState*
	ch(utf32_t c);

	NfaState*
	anyChar();

	NfaState*
	quantifier(NfaState* start);

	NfaState*
	clone(
		NfaState* first,
		NfaState* last
	);

	NfaState*
	capturingGroup();

	NfaState*
	nonCapturingGroup();
};

//..............................................................................

} // namespace re
} // namespace axl
