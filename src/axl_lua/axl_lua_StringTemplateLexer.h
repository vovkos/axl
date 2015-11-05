// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_lex_RagelLexer.h"

namespace axl {
namespace lua {

//.............................................................................

enum TokenKind
{
	TokenKind_Eof = 0,
	TokenKind_Error = -1,
	TokenKind_OpenCode = 256,
	TokenKind_OpenData,
	TokenKind_Data,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_LEX_BEGIN_TOKEN_NAME_MAP (TokenName)
	AXL_LEX_TOKEN_NAME (TokenKind_Eof,       "eof")
	AXL_LEX_TOKEN_NAME (TokenKind_Error,     "error")
	AXL_LEX_TOKEN_NAME (TokenKind_OpenCode,  "%{")
	AXL_LEX_TOKEN_NAME (TokenKind_OpenData,  "%(")
	AXL_LEX_TOKEN_NAME (TokenKind_Data,      "user-data")
AXL_LEX_END_TOKEN_NAME_MAP ();

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef lex::RagelToken <TokenKind, TokenName> Token;

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum LexerMachineKind
{
	LexerMachineKind_Main,
	LexerMachineKind_UserCode,
	LexerMachineKind_UserData,
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Lexer: public lex::RagelLexer <Lexer, Token>
{
	friend class lex::RagelLexer <Lexer, Token>;

public:
	static
	int
	getMachineState (LexerMachineKind machine);

protected:
	Token*
	createStringToken (
		int tokenKind,
		int left = 0,
		int right = 0
		)
	{
		Token* token = createToken (tokenKind);
		token->m_data.m_string.copy (ts + left, token->m_pos.m_length - (left + right));
		return token;
	}

	// implemented in *.rl

	void 
	init ();

	void
	exec ();
};

//.............................................................................

} // namespace lua
} // namespace axl
